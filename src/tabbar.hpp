#ifndef ISLAND_TABBAR_HPP
#define ISLAND_TABBAR_HPP

#include <QWidget>

class QHBoxLayout;
class TabLabel;

class TabBar : public QWidget {
  Q_OBJECT

public:
  TabBar(QWidget *parent = 0);
  void insertLabel(const int index, TabLabel *);
  void removeLabel(TabLabel *);

Q_SIGNALS:
  void triggered(const int index);
  void closeRequested(const int index);
  void wheelMotion(const bool polarity, int);

protected:
  void mousePressEvent(QMouseEvent *) override;
  void wheelEvent(QWheelEvent * event) override;

private:
  double m_wheelDelta;
  bool m_wheelPolarity;

  QHBoxLayout *m_layout;
};

#endif
