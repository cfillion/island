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

  void showProgress();
  void setProgress(const int);
  void hideProgress();

  QString text() const;

Q_SIGNALS:
  void textChanged(const QString &);

protected:
  void resizeEvent(QResizeEvent *) override;

private:
  int m_index;

  QLabel *m_icon;
  QLabel *m_prefix;
  QLabel *m_title;
  QLabel *m_progress;
};

#endif
