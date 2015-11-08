#ifndef ISLAND_PROMPT_HPP
#define ISLAND_PROMPT_HPP

#include <QLineEdit>

class Completer;

struct Word {
  int index;
  int start;
  QString text;
};

class Prompt : public QLineEdit
{
  Q_OBJECT

public:
  Prompt(QWidget *parent = 0);

  void setPrompt(const QString &, const bool commandPrompt = false);
  QString prompt() const;

  void setText(const QString &);
  QString text() const;

  Completer *completer() const { return m_completer; }

  void send();
  void complete(const int movement = 1);

Q_SIGNALS:
  void editingFinished(const QString &);

protected:
  void hideEvent(QHideEvent *) override;

private:
  void check();
  void cursorMoved(const int, const int);
  void selectionChanged();
  Word currentWord() const;
  void replaceWord(const QString &);

  int m_promptSize;
  bool m_isCommandPrompt;
  Completer *m_completer;
};

#endif
