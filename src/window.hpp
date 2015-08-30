#include <QWidget>

class Page;
class QStackedLayout;
class TabBar;
class TabLabel;

class Window : public QWidget {
public:
  enum OpenMode {
    Fullscreen,
    VSplit,
    HSplit
  };

  Window(QWidget *parent = 0);

  int addPage(const QUrl &url, const OpenMode mode = Fullscreen);
  void setCurrentPage(Page *);
  void setCurrentTab(const int index);
  void currentTabMotion(const bool polarity, const int size);
  int currentIndex() const;

private:
  void updateTitle(Page *);
  void updatePageIndex(const int start = 0);

  QList<Page *> m_pages;
  QStackedLayout *m_stack;
  TabBar *m_tabs;
  Page *m_current;
};
