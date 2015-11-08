#include "prompt.hpp"

#include "completer.hpp"

#include <QKeyEvent>

Prompt::Prompt(QWidget *parent)
  : QLineEdit(parent), m_promptSize(0), m_isCommandPrompt(false)
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

void Prompt::setPrompt(const QString &prompt, const bool isCommandPrompt)
{
  m_promptSize = prompt.size();
  m_isCommandPrompt = isCommandPrompt;

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
}

void Prompt::send()
{
  Q_EMIT editingFinished(hasFocus() ? text() : QString());
}

void Prompt::cursorMoved(const int, const int after)
{
  m_completer->dismiss();

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
  if(!m_isCommandPrompt) {
    insert("\t");
    return;
  }

  QString input = text();
 input.remove(cursorPosition() - m_promptSize, input.size());

  m_completer->trigger(input);
  m_completer->move(movement);
}

void Prompt::replaceWord(const int start, const int end, const QString &newText)
{
  blockSignals(true);
  setText(text().replace(start, end, newText));
  setCursorPosition(m_promptSize + start + newText.size());
  blockSignals(false);
}

void Prompt::hideEvent(QHideEvent *)
{
  m_completer->dismiss();
  QLineEdit::setText(QString());
}
