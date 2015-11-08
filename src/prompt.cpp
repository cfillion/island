#include "prompt.hpp"

#include "completer.hpp"

#include <QKeyEvent>

Prompt::Prompt(QWidget *parent)
  : QLineEdit(parent), m_promptSize(0)
{
  m_completer = new Completer(this);

  setFrame(false);
  setAttribute(Qt::WA_MacShowFocusRect, false);

  connect(this, &QLineEdit::textChanged, this, &Prompt::check);
  connect(this, &QLineEdit::editingFinished, this, &Prompt::send);
  connect(this, &QLineEdit::cursorPositionChanged, this, &Prompt::cursorMoved);
  connect(this, &QLineEdit::selectionChanged, this, &Prompt::selectionChanged);

  connect(m_completer, &Completer::wordChanged, this, &Prompt::replaceWord);
}

void Prompt::setPrompt(const QString &prompt)
{
  m_promptSize = prompt.size();

  QLineEdit::setText(prompt);
}

QString Prompt::prompt() const
{
  return QLineEdit::text().left(m_promptSize);
}

void Prompt::setText(const QString &userText)
{
  QLineEdit::setText(prompt() + userText);
}

QString Prompt::text() const
{
  return QLineEdit::text().remove(0, m_promptSize);
}

void Prompt::check()
{
  if(prompt().size() < m_promptSize)
    send();
  else
    m_completer->dismiss();
}

void Prompt::send()
{
  Q_EMIT editingFinished(hasFocus() ? text() : QString());
}

void Prompt::cursorMoved(const int, const int after)
{
  if(after < m_promptSize)
    setCursorPosition(m_promptSize);
}

void Prompt::selectionChanged()
{
  if(!hasSelectedText() || selectionStart() >= m_promptSize)
    return;

  if(cursorPosition() < m_promptSize)
    cursorForward(true);
  else
    setSelection(m_promptSize, text().size());
}

void Prompt::complete(const int movement)
{
  const Word word = currentWord();

  m_completer->trigger(word.index, word.text);
  m_completer->move(movement);
}

Word Prompt::currentWord() const
{
  QString text = this->text();
  text.remove(cursorPosition() - m_promptSize, text.size());

  Word word;
  word.index = text.count("\x20");
  word.start = text.lastIndexOf("\x20")+1;

  text.remove(0, word.start);
  word.text = text;

  return word;
}

void Prompt::replaceWord(const QString &newWord)
{
  const Word word = currentWord();

  blockSignals(true);
  setText(text().replace(word.start, word.text.size(), newWord));
  setCursorPosition(word.start + newWord.size() + 1);
  blockSignals(false);
}

void Prompt::hideEvent(QHideEvent *)
{
  m_completer->dismiss();
  QLineEdit::setText(QString());
}
