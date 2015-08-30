#include <QWidget>

class Page;
class QHBoxLayout;
class QStackedLayout;
class QWebEngineView;
class TabLabel;

class Window : public QWidget {
public:
  Window(QWidget *parent = 0);

  int addTab(const QUrl &url);
  void setCurrentPage(Page *);

private:
  void updateTitle(Page *);

  QList<Page *> m_pages;
  QStackedLayout *m_stack;
  QHBoxLayout *m_tabs;
  Page *m_current;
};
