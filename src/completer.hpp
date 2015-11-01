#ifndef ISLAND_COMPLETER_HPP
#define ISLAND_COMPLETER_HPP

#include <QListWidget>

class Completer : public QListWidget {
  Q_OBJECT

public:
  Completer(QWidget *parent = 0);

  QSize sizeHint() const;

  void trigger(const int index, const QString &word);
  void move(const int movement);
  void dismiss();

Q_SIGNALS:
  void triggered();
  void wordChanged(const QString &);

protected:
  bool event(QEvent *);
};

#endif
