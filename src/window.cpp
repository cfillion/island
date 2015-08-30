#include "window.hpp"

#include "page.hpp"
#include "tab_label.hpp"

#include <QHBoxLayout>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QWebEngineView>

using namespace std;

Window::Window(QWidget *parent)
  : QWidget(parent)
{
  m_tabs = new QHBoxLayout;
  m_stack = new QStackedLayout;

  QVBoxLayout *main_layout = new QVBoxLayout(this);
  main_layout->setContentsMargins(QMargins());
  main_layout->addLayout(m_tabs);
  main_layout->addLayout(m_stack);

  addTab(QUrl("http://cfillion.tk"));
  addTab(QUrl("http://files.cfillion.tk"));
}

int Window::addTab(const QUrl &url)
{
  const int index = m_stack->count();

  TabLabel *label = new TabLabel;
  label->setIndex(index);

  QWebEngineView *view = new QWebEngineView;
  view->load(url);

  Page *page = new Page(label, view, view, this);
  connect(page, &Page::triggered, this, &Window::setCurrentPage);
  connect(page, &Page::titleChanged, this, &Window::updateTitle);

  m_pages.append(page);
  m_tabs->addWidget(page->label());
  m_stack->addWidget(page->viewport());
  setCurrentPage(page);

  return index;
}

void Window::setCurrentPage(Page *p)
{
  m_current = p;
  m_stack->setCurrentWidget(m_current->viewport());

  updateTitle(m_current);
}

void Window::updateTitle(Page *p)
{
  if(p != m_current)
    return;

  setWindowTitle(m_current->engine()->title());
}
