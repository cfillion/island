#include "window.hpp"

#include "page.hpp"
#include "tabbar.hpp"
#include "viewport.hpp"
#include "statusbar.hpp"

#include <QApplication>
#include <QKeyEvent>
#include <QStackedLayout>
#include <QTimer>
#include <QUrl>
#include <QVBoxLayout>

using namespace Island;

Window::Window(Mapping *mapping, QWidget *parent)
  : QWidget(parent), m_mapping(mapping), m_current(0)
{
  m_tabs = new TabBar;
  m_stack = new QStackedLayout;
  m_status = new StatusBar;

  QVBoxLayout *main_layout = new QVBoxLayout(this);
  main_layout->setSpacing(0);
  main_layout->setContentsMargins(QMargins());
  main_layout->addWidget(m_tabs);
  main_layout->addLayout(m_stack);
  main_layout->addWidget(m_status);

  connect(m_tabs, &TabBar::triggered, this, &Window::setCurrentTab);
  connect(m_tabs, &TabBar::wheelMotion, this, &Window::currentTabMotion);

  connect(this, &Window::bufferChanged, m_status, &StatusBar::setBuffer);
  connect(this, &Window::modeChanged, m_status, &StatusBar::setMode);
  connect(m_status, &StatusBar::promptFinished, this, &Window::execPrompt);

  addPage(QUrl("http://cfillion.tk"), NewTab);
  addPage(QUrl("http://files.cfillion.tk"));
  addPage(QUrl("data:text/html,<h1>test</h1>"), Split);
  addPage(QUrl("http://google.com/"));
  setCurrentTab(0);

  m_status->setPrompt(StatusBar::Command);
  setMode(Prompt);
}

int Window::addPage(const QUrl &url, const Window::OpenMode mode)
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
  if(Page *page = m_pages.at(index))
    setCurrentPage(page);
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
}

void Window::currentTabMotion(const bool polarity, const int size)
{
  int index = currentPageIndex();
  index += polarity ? -size : size;

  // TODO: add a setting to enable or disable infinite scrolling
  if(index < 0)
    index = m_pages.size() - abs(index);

  index %= m_pages.size();
  setCurrentPage(m_pages[index]);
}

int Window::currentPageIndex() const
{
  return m_current ? m_current->index() : -1;
}

void Window::shiftPageIndexes(const int start)
{
  const int pageCount = m_pages.size();
  for(int i = start; i < pageCount; i++)
    m_pages[i]->setIndex(i);
}

bool Window::handleKeyEvent(const QKeyEvent *e)
{
  if(m_mode == Prompt)
    return false;

  const Qt::KeyboardModifiers mod = e->modifiers();

  const QString text = keyEventToString(e);
  const bool isSpecial = text.size() > 1 && text[0].isUpper();

  if(text.isEmpty())
    return true;

  QStringList parts;
  if(mod & Qt::ControlModifier)
    parts << "C";
  if(mod & Qt::AltModifier)
    parts << "A";
  if(mod & Qt::MetaModifier)
    parts << "M";
  if(mod & Qt::ShiftModifier && isSpecial)
    parts << "S";

  parts << text;

  if(parts.size() > 1 || text.size() > 1)
    m_buffer << QString("<%1>").arg(parts.join("-"));
  else
    m_buffer << parts.join(QChar());

  Q_EMIT bufferChanged(m_buffer);

  return true;
}

QString Window::keyEventToString(const QKeyEvent *e) const
{
  const int key = e->key();

  switch(key) {
  case Qt::Key_Escape:
    return "Esc";
  case Qt::Key_Return:
  case Qt::Key_Enter:
    return "CR";
  case Qt::Key_Space:
    return "Space";
  case Qt::Key_Up:
    return "Up";
  case Qt::Key_Down:
    return "Down";
  case Qt::Key_Left:
    return "Left";
  case Qt::Key_Right:
    return "Right";
  case Qt::Key_Home:
    return "Home";
  case Qt::Key_End:
    return "End";
  case Qt::Key_PageUp:
    return "PageUp";
  case Qt::Key_PageDown:
    return "PageDown";
  case Qt::Key_Help:
    return "Help";
  case Qt::Key_Undo:
    return "Undo";
  case Qt::Key_Insert:
    return "Insert";
  case Qt::Key_Clear:
    return "Clear";
  case Qt::Key_Delete:
    return "Del";
  case Qt::Key_Tab:
  case Qt::Key_Backtab:
    return "Tab";
  case Qt::Key_Backspace:
    return "BS";
  case Qt::Key_Bar:
    return "Bar";
  case Qt::Key_Less:
    return "lt";
  case Qt::Key_Greater:
    return "gt";
  }

  if(key >= Qt::Key_F1 && key <= Qt::Key_F35)
    return QString("F%1").arg(key - Qt::Key_F1 + 1);
  else if(key >= Qt::Key_Exclam && key <= Qt::Key_ydiaeresis) {
    const QString text = e->text();
    if(!text.isEmpty())
      return text;

    const QString seq = QString(QChar(key));

    if(e->modifiers() & Qt::ShiftModifier)
      return seq.toUpper();
    else
      return seq.toLower();
  }

  return "";
}

void Window::setMode(const Mode mode)
{
  m_mode = mode;
  Q_EMIT modeChanged(m_mode);
}

void Window::execPrompt(const QString &input)
{
  // prevents <CR> from being added to the input buffer
  QTimer::singleShot(0, this, [=] { setMode(Normal); });

  if(input.isEmpty())
    return;
}
