#include "global.hpp"

#include <QKeyEvent>

QString Island::KeyEventToSequence(const QKeyEvent *e)
{
  const Qt::KeyboardModifiers mod = e->modifiers();

  const QString charStr = GetKeyEventCharacter(e);
  const bool isSpecial = charStr.size() > 1 && charStr[0].isUpper();

  if(charStr.isEmpty())
    return QString();

  QStringList parts;
  if(mod & Qt::ControlModifier)
    parts << "C";
  if(mod & Qt::AltModifier)
    parts << "A";
  if(mod & Qt::MetaModifier)
    parts << "M";
  if(mod & Qt::ShiftModifier && isSpecial)
    parts << "S";

  parts << charStr;

  if(parts.size() > 1 || charStr.size() > 1)
    return QString("<%1>").arg(parts.join("-"));
  else
    return parts.join(QString());
}

QString Island::GetKeyEventCharacter(const QKeyEvent *e)
{
  const int key = e->key();

  switch(key) {
  case Qt::Key_Escape:
    return "Esc";
  case Qt::Key_Return:
  case Qt::Key_Enter:
    return "CR";
  case Qt::Key_Space:
    return "Space";
  case Qt::Key_Up:
    return "Up";
  case Qt::Key_Down:
    return "Down";
  case Qt::Key_Left:
    return "Left";
  case Qt::Key_Right:
    return "Right";
  case Qt::Key_Home:
    return "Home";
  case Qt::Key_End:
    return "End";
  case Qt::Key_PageUp:
    return "PageUp";
  case Qt::Key_PageDown:
    return "PageDown";
  case Qt::Key_Help:
    return "Help";
  case Qt::Key_Undo:
    return "Undo";
  case Qt::Key_Insert:
    return "Insert";
  case Qt::Key_Clear:
    return "Clear";
  case Qt::Key_Delete:
    return "Del";
  case Qt::Key_Tab:
  case Qt::Key_Backtab:
    return "Tab";
  case Qt::Key_Backspace:
    return "BS";
  case Qt::Key_Bar:
    return "Bar";
  case Qt::Key_Less:
    return "lt";
  case Qt::Key_Greater:
    return "gt";
  }

  if(key >= Qt::Key_F1 && key <= Qt::Key_F35)
    return QString("F%1").arg(key - Qt::Key_F1 + 1);
  else if(key >= Qt::Key_Exclam && key <= Qt::Key_ydiaeresis) {
    const QString text = e->text();

    if(!text.isEmpty())
      return text;

    const QString seq = QString(QChar(key));

    if(e->modifiers() & Qt::ShiftModifier)
      return seq.toUpper();
    else
      return seq.toLower();
  }

  return QString();
}
