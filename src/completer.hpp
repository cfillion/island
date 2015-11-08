#ifndef ISLAND_COMPLETER_HPP
#define ISLAND_COMPLETER_HPP

#include <QListWidget>

class CommandParser;

class Completer : public QListWidget {
  Q_OBJECT

public:
  Completer(QWidget *parent = 0);

  QSize sizeHint() const;

  void trigger(const QString &text);
  void move(const int movement);
  void dismiss();

Q_SIGNALS:
  void triggered();
  void wordChanged(const int start, const int end, const QString &);

protected:
  void completeName(const CommandParser &);
  void completeArgument(const CommandParser &);
  void replaceWord(const QString &);

  int m_startIndex;
  int m_endIndex;

  bool event(QEvent *);
};

#endif
