#ifndef ISLAND_STATUSBAR_HPP
#define ISLAND_STATUSBAR_HPP

#include <QWidget>

class QLabel;

class StatusBar : public QWidget {
public:
  StatusBar(QWidget *parent = 0);
  void setUrl(const QUrl &);

private:
  QLabel *m_url;
};

#endif
