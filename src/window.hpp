#include <QWidget>

#include "page_fwd.hpp"

class Page;
class QHBoxLayout;
class QStackedLayout;
class QWebEngineView;
class TabLabel;

class Window : public QWidget {
public:
  Window(QWidget *parent = 0);

  int addTab(const QUrl &url);
  void setCurrentPage(PagePtr page);

private:
  QList<PagePtr> m_pages;
  QStackedLayout *m_stack;
  QHBoxLayout *m_tabs;
};
