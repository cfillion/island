#ifndef ISLAND_STATUSBAR_HPP
#define ISLAND_STATUSBAR_HPP

#include <QWidget>

#include "buffer.hpp"
#include "global.hpp"

class Page;
class QLabel;
class QLineEdit;
class QProgressBar;

class StatusBar : public QWidget {
  Q_OBJECT

public:
  StatusBar(QWidget *parent = 0);

  void setPage(Page *);
  void setPageCount(const int);

  void setBuffer(const Buffer &);

  void setMode(const Island::Mode);

Q_SIGNALS:
  void promptFinished(const QString &);

protected:
  void resizeEvent(QResizeEvent *) override;

private:
  void updateLabels();
  void checkPrompt();
  void sendPrompt();

  Page *m_page;
  int m_pageCount;

  QLabel *m_status;
  QLineEdit *m_prompt;
  QLabel *m_buffer;
  QLabel *m_url;
  QProgressBar *m_progress;
  QLabel *m_tabPosition;
};

#endif
