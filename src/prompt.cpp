#include "prompt.hpp"

#include <QKeyEvent>

Prompt::Prompt(QWidget *parent)
  : QLineEdit(parent), m_promptSize(0)
{
  setFrame(false);
  setAttribute(Qt::WA_MacShowFocusRect, false);

  connect(this, &QLineEdit::textChanged, this, &Prompt::check);
  connect(this, &QLineEdit::editingFinished, this, &Prompt::send);
  connect(this, &QLineEdit::cursorPositionChanged, this, &Prompt::cursorMoved);
  connect(this, &QLineEdit::selectionChanged, this, &Prompt::selectionChanged);
}

void Prompt::setPrompt(const QString &prompt)
{
  m_promptSize = prompt.size();

  QLineEdit::setText(prompt);
}

void Prompt::setText(const QString &userText)
{
  QLineEdit::setText(text().left(m_promptSize) + userText);
}

void Prompt::check()
{
  if(text().size() < m_promptSize)
    send();
}

void Prompt::send()
{
  if(hasFocus())
    Q_EMIT editingFinished(text().remove(0, m_promptSize).simplified());
  else
    Q_EMIT editingFinished(QString());
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
    setSelection(m_promptSize, text().size() - m_promptSize);
}

void Prompt::keyPressEvent(QKeyEvent *e)
{
#ifdef Q_OS_OSX
  switch(e->key()) {
  case Qt::Key_Home:
    home(false);
    return;
  case Qt::Key_End:
    end(false);
    return;
  };
#endif

  QLineEdit::keyPressEvent(e);
}
