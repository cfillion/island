#ifndef ISLAND_WINDOW_HPP
#define ISLAND_WINDOW_HPP

#include <QWidget>

class Page;
class QStackedLayout;
class StatusBar;
class TabBar;

class Window : public QWidget {
public:
  enum OpenMode {
    NewTab,
    Split,
  };

  Window(QWidget *parent = 0);

  int addPage(const QUrl &url, const OpenMode mode = NewTab);
  void setCurrentPage(Page *);
  void setCurrentTab(const int index);
  void currentTabMotion(const bool polarity, const int size);
  int currentPageIndex() const;

private:
  void pageChanged(Page *);
  void updatePageIndex(const int start = 0);

  QList<Page *> m_pages;
  QStackedLayout *m_stack;
  TabBar *m_tabs;
  Page *m_current;
  StatusBar *m_status;
};

#endif
