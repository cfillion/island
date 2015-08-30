#include "window.hpp"

#include "page.hpp"
#include "tab_bar.hpp"
#include "tab_label.hpp"

#include <QStackedLayout>
#include <QVBoxLayout>
#include <QWebEngineView>

using namespace std;

Window::Window(QWidget *parent)
  : QWidget(parent)
{
  m_tabs = new TabBar;
  m_stack = new QStackedLayout;

  QVBoxLayout *main_layout = new QVBoxLayout(this);
  main_layout->setContentsMargins(QMargins());
  main_layout->addWidget(m_tabs);
  main_layout->addLayout(m_stack);

  connect(m_tabs, &TabBar::triggered, this, &Window::setCurrentTab);
  connect(m_tabs, &TabBar::wheelMotion, this, &Window::currentTabMotion);

  addTab(QUrl("http://cfillion.tk"));
  addTab(QUrl("http://files.cfillion.tk"));
}

int Window::addTab(const QUrl &url)
{
  const int index = m_pages.size();

  TabLabel *label = new TabLabel;
  label->setIndex(index);

  QWebEngineView *view = new QWebEngineView;
  view->load(url);

  Page *page = new Page(label, view, view, this);
  connect(page, &Page::titleChanged, this, &Window::updateTitle);
  connect(page, &Page::iconChanged, this, &Window::updateTitle);

  m_pages.append(page);
  m_tabs->addLabel(page->label());
  m_stack->addWidget(page->viewport());
  setCurrentPage(page);

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
  m_stack->setCurrentWidget(m_current->viewport());

  updateTitle(m_current);
}

void Window::currentTabMotion(const bool polarity, const int size)
{
  // TODO: switch from viewport to viewport instead of page to page
  int index = m_current->label()->index();
  index += polarity ? size : -size;

  if(index < 0)
    index = m_pages.size() - index;

  // TODO: add a setting to enable or disable infinite scrolling
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
