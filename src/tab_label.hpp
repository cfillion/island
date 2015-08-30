#ifndef ISLAND_TAB_LABEL_HPP
#define ISLAND_TAB_LABEL_HPP

#include <QWidget>

class QLabel;

class TabLabel : public QWidget {
  Q_OBJECT

public:
  TabLabel(QWidget *parent = 0);

  int index() const { return m_index; }
  void setIndex(const int);
  void setTitle(const QString &);
  void setIcon(const QIcon &);

protected:
  void resizeEvent(QResizeEvent *) override;

private:
  int m_index;

  QLabel *m_icon;
  QLabel *m_prefix;
  QLabel *m_title;
};

#endif
