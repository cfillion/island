#include <QWidget>

class QLabel;

class TabLabel : public QWidget {
  Q_OBJECT

public:
  TabLabel(QWidget *parent = 0);

  int index() const { return m_index; }
  void setIndex(const int);
  void setTitle(const QString &);

Q_SIGNALS:
  void triggered(const int index);

protected:
  void mousePressEvent(QMouseEvent *);

private:
  int m_index;

  QLabel *m_prefix;
  QLabel *m_title;
};
