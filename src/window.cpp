#include "window.hpp"

#include "page.hpp"
#include "statusbar.hpp"
#include "tabbar.hpp"
#include "viewport.hpp"

#include <QApplication>
#include <QStackedLayout>
#include <QTimer>
#include <QUrl>
#include <QVBoxLayout>

using namespace Island;

Window::Window(const MappingArray &mappings, QWidget *parent)
  : QWidget(parent), m_current(0), m_mappings(mappings)
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
  connect(m_tabs, &TabBar::closeRequested, this, &Window::closeTab);
  connect(m_tabs, &TabBar::wheelMotion, this, &Window::currentTabMotion);

  connect(this, &Window::bufferChanged, m_status, &StatusBar::setBuffer);
  connect(this, &Window::modeChanged, m_status, &StatusBar::setMode);
  connect(m_status, &StatusBar::promptFinished, this, &Window::execPrompt);

  m_mappingTimer.setInterval(1000);
  m_mappingTimer.setSingleShot(true);
  connect(&m_mappingTimer, &QTimer::timeout, this, &Window::execDelayedMapping);

  addPage(QUrl("http://cfillion.tk"), NewTab);
  addPage(QUrl("http://files.cfillion.tk"));
  addPage(QUrl("data:text/html,<h1>test</h1>"), Split);
  addPage(QUrl("http://google.com/"));
  setCurrentTab(0);

  setMode(Normal);
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
  if(Page *page = m_pages.value(index))
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

void Window::closeTab(const int index)
{
  if(Page *page = m_pages.value(index))
    closePage(page);
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

#include <QDebug>
bool Window::handleKeyEvent(const QKeyEvent *e)
{
  const bool eatKey = m_mode == Normal;
  const QString seq = KeyEventToSequence(e);

  if(seq.isEmpty())
    return eatKey;

  m_mappingTimer.stop();
  m_buffer << seq;

  auto match = m_mappings[m_mode]->match(m_buffer);
  qDebug() << match;

  if(!match.ambiguous)
    m_buffer.truncate(match.index+1);

  Q_EMIT bufferChanged(m_buffer);

  if(!match.mapping)
    return eatKey;

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

void Window::execDelayedMapping()
{
  m_buffer.clear();
  Q_EMIT bufferChanged(m_buffer);

  execMapping(m_delayedMapping);
  m_delayedMapping = 0;
}

void Window::execMapping(const Mapping *mapping)
{
  qDebug() << "executing" << mapping;

  Command cmd = *mapping->command();
  cmd.setCounter(m_buffer.counter());
  cmd.setData(this);
  cmd.exec();

  m_buffer.clear();
  Q_EMIT bufferChanged(m_buffer);
}
