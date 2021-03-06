#include "window.hpp"

#include "completer.hpp"
#include "page.hpp"
#include "prompt.hpp"
#include "statusbar.hpp"
#include "tabbar.hpp"
#include "viewport.hpp"

#include <QApplication>
#include <QStackedLayout>
#include <QTimer>
#include <QVBoxLayout>

using namespace Island;

Window::Window(const MappingArray &mappings, QWidget *parent)
  : QWidget(parent), m_current(0), m_mappings(mappings)
{
  m_tabs = new TabBar;
  m_stack = new QStackedLayout;
  m_status = new StatusBar(this);

  m_completer = m_status->prompt()->completer();
  m_completer->setParent(this);

  QVBoxLayout *main_layout = new QVBoxLayout(this);
  main_layout->setSpacing(0);
  main_layout->setContentsMargins(QMargins());
  main_layout->addWidget(m_tabs);
  main_layout->addLayout(m_stack);
  main_layout->addWidget(m_status);

  connect(m_tabs, &TabBar::triggered, this, &Window::setCurrentPageIndex);
  connect(m_tabs, &TabBar::closeRequested, this, &Window::closeTab);
  connect(m_tabs, &TabBar::wheelMotion, this, &Window::currentTabMotion);

  connect(this, &Window::bufferChanged, m_status, &StatusBar::setBuffer);
  connect(this, &Window::modeChanged, m_status, &StatusBar::setMode);
  connect(m_status, &StatusBar::promptFinished, this, &Window::execPrompt);
  connect(m_completer, &Completer::triggered, this, &Window::resizeCompleter);

  m_mappingTimer.setInterval(1000);
  m_mappingTimer.setSingleShot(true);
  connect(&m_mappingTimer, &QTimer::timeout, this, &Window::execMapping);

  addPage("http://cfillion.tk", NewTab);
  addPage("http://files.cfillion.tk");
  addPage("data:text/html,<h1>test</h1>", Split);
  addPage("http://google.com/");
  setCurrentPageIndex(0);

  setMode(NormalMode);
}

Prompt *Window::prompt() const
{
  return m_status->prompt();
}

int Window::addPage(const QString &url, const Window::OpenMode mode)
{
  int index = currentPageIndex();

  if(index == -1)
    index = m_pages.size();
  else
    index += 1; // insert after the current index

  Page *page = new Page(url, this);
  connect(page, &Page::triggered, this, &Window::setCurrentPage);

  const Page *current = 0;
  if(!m_pages.isEmpty())
    current = m_current ? m_current : m_pages.last();

  switch(mode) {
  case NewTab: {
    Viewport *vp = new Viewport(this);
    vp->addPage(page);
    m_stack->addWidget(vp);
    break;
  }
  case Split:
    current->viewport()->addPage(page);
    break;
  };

  m_pages.insert(index, page);
  m_tabs->insertLabel(index, page->label());
  shiftPageIndexes(index);

  m_status->setPageCount(m_pages.size());

  return index;
}

void Window::setCurrentPageIndex(const int index)
{
  if(Page *p = page(index))
    setCurrentPage(p);
}

void Window::setCurrentPage(Page *p)
{
  if(m_current == p)
    return;

  if(m_current) {
    m_current->setCurrent(false);
    m_current->disconnect(this);
    connect(m_current, &Page::triggered, this, &Window::setCurrentPage);
  }

  p->disconnect(this);
  connect(p, &Page::displayTitleChanged, this, &Window::setWindowTitle);
  connect(p, &Page::iconChanged, this, &Window::setWindowIcon);

  p->setCurrent(true);
  m_current = p;

  m_stack->setCurrentWidget(m_current->viewport());
  m_status->setPage(m_current);

  setWindowTitle(m_current->displayTitle());
  setWindowIcon(m_current->icon());

  clearBuffer();
}

void Window::movePage(Page *page, const int to)
{
  const int oldIndex = page->index();
  const int newIndex = std::max(0, std::min(to, pageCount() - 1));

  if(oldIndex == newIndex)
    return;

  m_pages.move(oldIndex, newIndex);

  TabLabel *label = page->label();
  m_tabs->removeLabel(label);
  m_tabs->insertLabel(newIndex, label);

  // newIndex may be smaller than oldIndex (eg. :tabmove -1)
  shiftPageIndexes(std::min(newIndex, oldIndex));
}

void Window::closeTab(const int index)
{
  if(Page *p = page(index))
    closePage(p);
}

void Window::closePage(Page *page)
{
  if(m_pages.size() == 1) {
    close();
    return;
  }

  const int index = page->index();

  if(index == currentPageIndex()) {
    if(index == 0)
      setCurrentPageIndex(index+1);
    else
      setCurrentPageIndex(index-1);
  }

  page->destroyComponents();
  page->deleteLater();

  m_pages.removeAt(index);
  m_status->setPageCount(m_pages.size());

  shiftPageIndexes();
}

void Window::currentTabMotion(const bool polarity, const int size)
{
  int index = currentPageIndex();
  index += polarity ? -size : size;

  // TODO: add a setting to enable or disable infinite scrolling
  if(index < 0)
    index = m_pages.size() - abs(index);

  index %= m_pages.size();
  setCurrentPage(page(index));
}

int Window::currentPageIndex() const
{
  return m_current ? m_current->index() : -1;
}

void Window::shiftPageIndexes(const int start)
{
  const int pageCount = m_pages.size();
  for(int i = start; i < pageCount; i++)
    page(i)->setIndex(i);
}

bool Window::handleInput(const KeyPress &kp)
{
  const bool eatKey = m_mode == NormalMode;
  const QString text = kp.toString();

  // ignore invalid keystrokes (including modifiers)
  if(text.isEmpty())
    return eatKey;

  m_buffer << text;

  // disable range for non-normal modes
  if(!eatKey)
    m_buffer.resetRange();

  Q_EMIT bufferChanged(m_buffer);

  // don't execute counter keystrokes
  if(m_buffer.empty())
    return eatKey;

  m_mappingTimer.stop();

  const auto match = m_mapping->match(text);

  if(match.mapping) {
    m_mapping = match.mapping;

    if(!match.ambiguous)
      execMapping();
    else if(m_mapping->isBound())
      m_mappingTimer.start();

    return true;
  }
  else {
    if(m_mapping->isBound())
      execMapping();
    else if(!m_buffer.empty())
      clearBuffer();

    rewindMapping();

    return eatKey;
  }
}

void Window::simulateInput(const Buffer &buf)
{
  std::vector<KeyPress> keys;

  // do string expansion before simulating any keypresses
  for(const QString &seq : buf) {
    if(seq.startsWith('%') && seq.size() > 1) {
      for(const QChar &c : sprintf(seq))
        keys.push_back(KeyPress(c));
    }
    else
      keys.push_back(KeyPress(seq));
  }

  for(const KeyPress &kp : keys)
    simulateInput(kp);
}

void Window::simulateInput(const KeyPress &kp)
{
  const bool eaten = handleInput(kp);

  if(!eaten && m_mode >= CommandMode)
    prompt()->insert(kp.displayString());
}

void Window::setMode(const Mode mode)
{
  m_mode = mode;
  m_mapping = m_mappings[std::min(m_mode, CommandMode)];

  Q_EMIT modeChanged(m_mode);
}

void Window::execPrompt(const QString &input)
{
  const QString &prompt = sprintf(input);

  // don't execute the prompt at the same time the keymapping is handled
  // executing this later prevents <CR> from being added to the input buffer
  // and the statusbar to be overridden by the mapping's action
  QTimer::singleShot(0, this, [=] {
    switch(m_mode) {
    case CommandMode:
      execCommandPrompt(prompt);
      break;
    case SearchForwardMode:
    case SearchBackwardMode:
      execSearchPrompt(prompt);
      break;
    default:
      break;
    }

    // mode-switching commands cannot be ran from the prompt
    // since 93f05a0a0c921148adc9479695f4b1b0891d1f26
    setMode(NormalMode);
  });
}

void Window::execCommandPrompt(const QString &input)
{
  if(input.simplified().isEmpty())
    return;

  Command cmd(input);
  execCommand(cmd);
}

void Window::execSearchPrompt(const QString &input)
{
  currentPage()->findText(input, m_mode == SearchForwardMode);
}

void Window::execMapping()
{
  const Mapping *mapping = m_mapping;

  rewindMapping();

  if(mapping->bindingType() == Mapping::CommandBinding) {
    Command cmd = *mapping->boundCommand();
    cmd.setRange(m_buffer.range());
    execCommand(cmd);
  }

  if(mapping->bindingType() == Mapping::BufferBinding) {
    const Buffer &buf = *mapping->boundBuffer();

    setUpdatesEnabled(false);
    simulateInput(buf);
    QTimer::singleShot(0, this, [=] { setUpdatesEnabled(true); });
  }

  clearBuffer();
}

void Window::execCommand(Command &cmd)
{
  cmd.setData(this);

  const CommandResult res = cmd.exec();

  QString message = res.message;
  if(!res.ok)
    message.prepend("Error: ");

  m_status->setStatus(message);
}

void Window::rewindMapping()
{
  if(m_mapping->root())
    m_mapping = m_mapping->root();
}

void Window::clearBuffer()
{
  m_buffer.clear();
  Q_EMIT bufferChanged(m_buffer);
}

QString Window::sprintf(const QString &input) const
{
  QString str(input);

  static const QRegularExpression regex("%([a-zA-Z%])");
  auto it = regex.globalMatch(str);
  int offset = 0;

  while(it.hasNext()) {
    const auto match = it.next();
    const QChar &c = match.captured(1)[0];
    QString replacement;

    if(!expandFormat(c, replacement))
      continue;

    const int start = match.capturedStart(0) + offset;
    const int length = match.capturedLength(0);
    offset += replacement.size() - length;

    str.replace(start, length, replacement);
  }

  return str;
}

bool Window::expandFormat(const QChar &c, QString &val) const
{
  if(c == '%')
    val = c;
  else if(c == 'u')
    val = m_current->displayUrl();
  else if(c == 'r')
    val = m_buffer.range().toString();
  else if(c == 'c')
    val = m_buffer.range().toString(Range::Counter);
  else
    return false;

  return true;
}

void Window::resizeEvent(QResizeEvent *)
{
  resizeCompleter();
}

void Window::resizeCompleter()
{
  const QRect statusGeometry = m_status->geometry();
  const int completerHeight = m_completer->sizeHint().height();

  m_completer->setGeometry(0, statusGeometry.y() - completerHeight,
    width(), std::min(completerHeight, height() - statusGeometry.height()));
}
