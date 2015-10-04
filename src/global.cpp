#include "global.hpp"

#include <boost/bimap.hpp>
#include <QKeyEvent>
#include <QRegularExpression>

typedef boost::bimap<int, QString> KeyBimap;

static KeyBimap make_keymap(std::initializer_list<KeyBimap::value_type> list)
{
  return KeyBimap(list.begin(), list.end());
}

static const KeyBimap SpecialKeys = make_keymap({
  {Qt::Key_Escape,    "Esc"      },
  {Qt::Key_Return,    "CR"       },
  {Qt::Key_Space,     "Space"    },
  {Qt::Key_Up,        "Up"       },
  {Qt::Key_Down,      "Down"     },
  {Qt::Key_Left,      "Left"     },
  {Qt::Key_Right,     "Right"    },
  {Qt::Key_Home,      "Home"     },
  {Qt::Key_End,       "End"      },
  {Qt::Key_PageUp,    "PageUp"   },
  {Qt::Key_PageDown,  "PageDown" },
  {Qt::Key_Help,      "Help"     },
  {Qt::Key_Undo,      "Undo"     },
  {Qt::Key_Insert,    "Insert"   },
  {Qt::Key_Clear,     "Clear"    },
  {Qt::Key_Delete,    "Del"      },
  {Qt::Key_Tab,       "Tab"      },
  {Qt::Key_Backspace, "BS"       },
  {Qt::Key_Bar,       "Bar"      },

  {Qt::Key_Less,      "lt"       },
  {Qt::Key_Greater,   "gt"       },
});

static const std::map<int, int> KeyAliases{
  {Qt::Key_Enter,   Qt::Key_Return },
  {Qt::Key_Backtab, Qt::Key_Tab    },
};

static bool isLetter(const int key)
{
  return key >= Qt::Key_Exclam && key <= Qt::Key_Yacute;
}

QString Island::EncodeSequence(const QKeyEvent *e)
{
  const Qt::KeyboardModifiers mod = e->modifiers();

  const QString charStr = EncodeCharacter(e);
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
    return parts[0];
}

QString Island::EncodeCharacter(const QKeyEvent *e)
{
  const bool aliased = KeyAliases.count(e->key());
  const int key = aliased ? KeyAliases.at(e->key()) : e->key();

  if(SpecialKeys.left.count(key))
    return SpecialKeys.left.at(key);

  if(key >= Qt::Key_F1 && key <= Qt::Key_F35)
    return QString("F%1").arg(key - Qt::Key_F1 + 1);
  else if(isLetter(key)) {
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

Island::KeyModPair Island::DecodeSequence(const QString &text)
{
  static const QRegularExpression pattern("\\A\\<([^\\>]+)\\>\\z");
  const auto match = pattern.match(text);

  if(!match.hasMatch())
    return DecodeCharacter(text);

  Qt::KeyboardModifiers mod;

  QStringList parts = match.captured(1).split('-');
  const KeyModPair last = DecodeCharacter(parts.takeLast());

  for(const QString &part : parts) {
    if(part == "C")
      mod |= Qt::ControlModifier;
    else if(part == "A")
      mod |= Qt::AltModifier;
    else if(part == "M")
      mod |= Qt::MetaModifier;
    else if(part == "S")
      mod |= Qt::ShiftModifier;
  }

  mod |= std::get<Qt::KeyboardModifiers>(last);

  return {std::get<int>(last), mod};
}

Island::KeyModPair Island::DecodeCharacter(const QString &text)
{
  if(SpecialKeys.right.count(text))
    return {SpecialKeys.right.at(text), Qt::NoModifier};

  if(text.indexOf('F') == 0) {
    const int fkey = text.right(text.size()-1).toInt();
    if(fkey >= 1 && fkey <= 35)
      return {Qt::Key_F1 + (fkey-1), Qt::NoModifier};
  }

  const QChar c = text[0];
  int code = c.unicode();

  if(c.isLower())
    code -= code - c.toUpper().unicode();

  if(isLetter(code)) {
    if(c.isUpper())
      return {code, Qt::ShiftModifier};
    else
      return {code, Qt::NoModifier};
  }
  else
    return {0, Qt::NoModifier};
}
