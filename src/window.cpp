#include "window.hpp"

#include "page.hpp"
#include "tab_bar.hpp"
#include "tab_label.hpp"

#include <QStackedLayout>
#include <QUrl>
#include <QVBoxLayout>
#include <QWebEngineView>

using namespace std;

Window::Window(QWidget *parent)
  : QWidget(parent), m_current(0)
{
  m_tabs = new TabBar;
  m_stack = new QStackedLayout;

  QVBoxLayout *main_layout = new QVBoxLayout(this);
  main_layout->setSpacing(0);
  main_layout->setContentsMargins(QMargins());
  main_layout->addWidget(m_tabs);
  main_layout->addLayout(m_stack);

  connect(m_tabs, &TabBar::triggered, this, &Window::setCurrentTab);
  connect(m_tabs, &TabBar::wheelMotion, this, &Window::currentTabMotion);

  addPage(QUrl("http://cfillion.tk"));
  addPage(QUrl("http://files.cfillion.tk"));
  addPage(QUrl("data:text/html,<h1>test</h1>"));
  setCurrentTab(0);
}

int Window::addPage(const QUrl &url, const Window::OpenMode mode)
{
  int index = currentIndex();

  if(index == -1)
    index = m_pages.size();
  else
    index += 1; // insert after the current index

  Page *page = new Page(url, this);
  connect(page, &Page::titleChanged, this, &Window::updateTitle);
  connect(page, &Page::iconChanged, this, &Window::updateTitle);

  m_pages.insert(index, page);
  m_tabs->insertLabel(index, page->label());
  updatePageIndex(index);

  switch(mode) {
  case Fullscreen:
    break;
  case VSplit:
    break;
  case HSplit:
    break;
  };

  m_stack->addWidget(page->engine());

  return index;
}

void Window::setCurrentTab(const int index)
{
  if(Page *page = m_pages.at(index))
    setCurrentPage(page);
}

void Window::setCurrentPage(Page *p)
{
  m_current = p;
  m_stack->setCurrentWidget(m_current->engine());

  updateTitle(m_current);
}

void Window::currentTabMotion(const bool polarity, const int size)
{
  // TODO: switch from viewport to viewport instead of page to page
  int index = currentIndex();
  index += polarity ? -size : size;

  // TODO: add a setting to enable or disable infinite scrolling
  if(index < 0)
    index = m_pages.size() - abs(index);

  index %= m_pages.size();
  setCurrentPage(m_pages[index]);
}

void Window::updateTitle(Page *p)
{
  if(p != m_current)
    return;

  setWindowTitle(m_current->engine()->title());
  setWindowIcon(m_current->icon());
}

int Window::currentIndex() const
{
  return m_current ? m_current->label()->index() : -1;
}

void Window::updatePageIndex(const int start)
{
  const int pageCount = m_pages.size();
  for(int i = start; i < pageCount; i++)
    m_pages[i]->label()->setIndex(i);
}
