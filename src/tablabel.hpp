#ifndef ISLAND_TABLABEL_HPP
#define ISLAND_TABLABEL_HPP

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

  bool isLoading() const;
  void setLoading(const bool);
  int loadProgress() const { return m_loadProgress; }
  void setLoadProgress(const int);

  QString text() const;

Q_SIGNALS:
  void textChanged(const QString &);

protected:
  void resizeEvent(QResizeEvent *) override;

private:
  int m_index;
  int m_loadProgress;

  QLabel *m_icon;
  QLabel *m_prefix;
  QLabel *m_title;
  QLabel *m_progress;
};

#endif
