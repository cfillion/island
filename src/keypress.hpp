#ifndef ISLAND_KEYPRESS_HPP
#define ISLAND_KEYPRESS_HPP

#include <QString>

class QKeyEvent;

class KeyPress {
public:
  KeyPress(const int c, Qt::KeyboardModifiers m = Qt::NoModifier)
    : m_key(c), m_mods(m) {}
  KeyPress(const QKeyEvent *);
  KeyPress(const QString &);

  int key() const { return m_key; }
  Qt::KeyboardModifiers modifiers() const { return m_mods; }

  QString toString() const;
  QString displayString() const;

  bool operator==(const KeyPress &) const;
  bool operator!=(const KeyPress &) const;

private:
  QString encodeCharacter(const bool lookupSpecials = true) const;
  void decodeCharacter(const QString &);

  int m_key;
  Qt::KeyboardModifiers m_mods;
  QString m_charTextOverride;
};

QDebug operator<<(QDebug, const KeyPress &);

#endif
