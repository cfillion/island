#include <QWidget>

class Page;
class QStackedLayout;
class QWebEngineView;
class TabBar;
class TabLabel;

class Window : public QWidget {
public:
  Window(QWidget *parent = 0);

  int addTab(const QUrl &url);
  void setCurrentPage(Page *);
  void setCurrentTab(const int index);
  void currentTabMotion(const bool polarity, const int size);

private:
  void updateTitle(Page *);

  QList<Page *> m_pages;
  QStackedLayout *m_stack;
  TabBar *m_tabs;
  Page *m_current;
};
