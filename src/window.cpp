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

  connect(m_tabs, &TabBar::triggered, this, &Window::setCurrentTab);
  connect(m_tabs, &TabBar::closeRequested, this, &Window::closeTab);
  connect(m_tabs, &TabBar::wheelMotion, this, &Window::currentTabMotion);

  connect(this, &Window::bufferChanged, m_status, &StatusBar::setBuffer);
  connect(this, &Window::modeChanged, m_status, &StatusBar::setMode);
  connect(m_status, &StatusBar::promptFinished, this, &Window::execPrompt);
  connect(m_completer, &Completer::triggered, this, &Window::resizeCompleter);

  m_mappingTimer.setInterval(1000);
  m_mappingTimer.setSingleShot(true);
  connect(&m_mappingTimer, &QTimer::timeout, this, &Window::execDelayedMapping);

  addPage("http://cfillion.tk", NewTab);
  addPage("http://files.cfillion.tk");
  addPage("data:text/html,<h1>test</h1>", Split);
  addPage("http://google.com/");
  setCurrentTab(0);

  setMode(NormalMode);
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

void Window::setCurrentTab(const int index)
{
  if(Page *p = page(index))
    setCurrentPage(p);
}

void Window::setCurrentPage(Page *p)
{
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
      setCurrentTab(index+1);
    else
      setCurrentTab(index-1);
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
  const QString seq = kp.toString();

  if(seq.isEmpty())
    return eatKey;

  m_mappingTimer.stop();
  m_buffer << seq;

  auto match = m_mappings[std::min(m_mode, CommandMode)]->match(m_buffer);

  if(!eatKey && match.index == -1)
    m_buffer.clear(); // clears the counter in insert and prompt modes
  else if(!match.ambiguous && !match.mapping)
    m_buffer.truncate(match.index+1);

  Q_EMIT bufferChanged(m_buffer);

  if(!match.mapping) {
    if(m_delayedMapping) {
      execDelayedMapping();
      return handleInput(kp);
    }

    return eatKey;
  }

  if(match.ambiguous) {
    m_delayedMapping = match.mapping;
    m_mappingTimer.start();
  }
  else {
    m_delayedMapping = 0;
    execMapping(match.mapping);
  }

  return true;
}

void Window::simulateInput(const Buffer &buf)
{
  for(const QString &seq : buf) {
    const KeyPress kp(seq);
    const bool eaten = handleInput(kp);

    if(!eaten && prompt()->hasFocus())
      prompt()->insert(kp.displayString());
  }
}

void Window::setMode(const Mode mode)
{
  m_mode = mode;
  Q_EMIT modeChanged(m_mode);
}

void Window::execPrompt(const QString &input)
{
  // executing this later prevents <CR> from being added to the input buffer
  QTimer::singleShot(0, this, [=] {
    switch(m_mode) {
    case CommandMode:
      execCommandPrompt(input);
      break;
    case SearchForwardMode:
    case SearchBackwardMode:
      execSearchPrompt(input);
      break;
    default:
      break;
    }

    setMode(NormalMode);
  });
}

void Window::execCommandPrompt(const QString &input)
{
  if(input.isEmpty())
    return;

  Command cmd(input);
  execCommand(cmd);
}

void Window::execSearchPrompt(const QString &input)
{
  currentPage()->findText(input, m_mode == SearchForwardMode);
}

void Window::execDelayedMapping()
{
  execMapping(m_delayedMapping);
  // m_delayedMapping is unset in clearBuffer which is called by execMapping
}

void Window::execMapping(const Mapping *mapping)
{
  if(mapping->type() == Mapping::Native) {
    Command cmd = *mapping->boundCommand();
    cmd.setCounter(m_buffer.counter());
    execCommand(cmd);
  }

  // this can't be done earlier because we need the counter to be set
  // and it can't be done later because it would re-trigger the mapping
  clearBuffer();

  if(mapping->type() == Mapping::User) {
    const Buffer &buf = *mapping->boundBuffer();
    simulateInput(buf);
  }
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

void Window::clearBuffer()
{
  m_delayedMapping = 0;
  m_buffer.clear();
  Q_EMIT bufferChanged(m_buffer);
}

Prompt *Window::prompt() const
{
  return m_status->prompt();
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
