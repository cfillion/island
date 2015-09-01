#ifndef ISLAND_STATUSBAR_HPP
#define ISLAND_STATUSBAR_HPP

#include <QWidget>

class Page;
class QLabel;
class QProgressBar;

class StatusBar : public QWidget {
public:
  StatusBar(QWidget *parent = 0);

  void setPage(Page *);
  void setPageCount(const int);

private:
  void updateLabels();

  Page *m_page;
  int m_pageCount;
  QLabel *m_url;
  QProgressBar *m_progress;
  QLabel *m_tabPosition;
};

#endif
