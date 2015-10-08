#include "keypress.hpp"

#include <boost/bimap.hpp>
#include <QDebug>
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

QDebug operator<<(QDebug debug, const KeyPress &kp)
{
  QDebugStateSaver saver(debug);
  debug.nospace()
    << "KeyPress("
    << kp.toString()
    << ", key: " << static_cast<Qt::Key>(kp.key())
    << ", modifiers: " << kp.modifiers()
    << ')';

  return debug;
}

KeyPress::KeyPress(const QKeyEvent *e)
  : m_key(e->key()), m_mods(e->modifiers()), m_charTextOverride(e->text())
{}

KeyPress::KeyPress(const QString &seq)
  : m_key(0), m_mods(Qt::NoModifier)
{
  static const QRegularExpression pattern("\\A\\<([^\\>]+)\\>\\z");
  const auto match = pattern.match(seq);

  if(!match.hasMatch()) {
    decodeCharacter(seq);
    return;
  }

  QStringList parts = match.captured(1).split('-');
  decodeCharacter(parts.takeLast());

  for(const QString &part : parts) {
    if(part == "C")
      m_mods |= Qt::ControlModifier;
    else if(part == "A")
      m_mods |= Qt::AltModifier;
    else if(part == "M")
      m_mods |= Qt::MetaModifier;
    else if(part == "S")
      m_mods |= Qt::ShiftModifier;
  }
}

bool KeyPress::operator==(const KeyPress &o) const
{
  return m_key == o.key() && m_mods == o.modifiers();
}

bool KeyPress::operator!=(const KeyPress &o) const
{
  return !(*this == o);
}

QString KeyPress::toString() const
{
  const QString charStr = encodeCharacter();
  const bool isSpecial = charStr.size() > 1 && charStr[0].isUpper();

  if(charStr.isEmpty())
    return QString();

  QStringList parts;
  if(m_mods & Qt::ControlModifier)
    parts << "C";
  if(m_mods & Qt::AltModifier)
    parts << "A";
  if(m_mods & Qt::MetaModifier)
    parts << "M";
  if(m_mods & Qt::ShiftModifier && isSpecial)
    parts << "S";

  parts << charStr;

  if(parts.size() > 1 || charStr.size() > 1)
    return QString("<%1>").arg(parts.join("-"));
  else
    return parts[0];
}

QString KeyPress::displayString() const
{
  if(m_mods != Qt::NoModifier)
    return QString();

  const QString charStr = encodeCharacter(false);
  return charStr;
}

QString KeyPress::encodeCharacter(const bool lookupSpecials) const
{
  const bool aliased = KeyAliases.count(m_key);
  const int key = aliased ? KeyAliases.at(m_key) : m_key;

  if(lookupSpecials && SpecialKeys.left.count(key))
    return SpecialKeys.left.at(key);

  if(key >= Qt::Key_F1 && key <= Qt::Key_F35)
    return QString("F%1").arg(key - Qt::Key_F1 + 1);

  if(!m_charTextOverride.isEmpty())
    return m_charTextOverride;

  const QChar c(key);

  if(!isLetter(key) && (lookupSpecials || !c.isPrint()))
    return QString();

  if(m_mods & Qt::ShiftModifier)
    return c.toUpper();
  else
    return c.toLower();
}

void KeyPress::decodeCharacter(const QString &seq)
{
  if(SpecialKeys.right.count(seq)) {
    m_key = SpecialKeys.right.at(seq);
    return;
  }

  if(seq.indexOf('F') == 0) {
    const int fkey = seq.right(seq.size()-1).toInt();

    if(fkey >= 1 && fkey <= 35) {
      m_key = Qt::Key_F1 + (fkey-1);
      return;
    }
  }

  const QChar c = seq[0];
  int code = c.unicode();

  if(c.isLower())
    code -= code - c.toUpper().unicode();

  if(!isLetter(code))
    return;

  m_key = code;

  if(c.isUpper())
    m_mods = Qt::ShiftModifier;
}
