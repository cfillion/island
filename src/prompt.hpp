#ifndef ISLAND_PROMPT_HPP
#define ISLAND_PROMPT_HPP

#include <QLineEdit>

class Prompt : public QLineEdit
{
  Q_OBJECT

public:
  Prompt(QWidget *parent = 0);
  void setPrompt(const QString &);

Q_SIGNALS:
  void editingFinished(const QString &);

private:
  void check();
  void send();
  void cursorMoved(const int, const int);
  void selectionChanged();

  int m_promptSize;
};

#endif
