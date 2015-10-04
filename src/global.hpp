#ifndef ISLAND_GLOBAL_HPP
#define ISLAND_GLOBAL_HPP

#include <QString>
#include <utility>

class QKeyEvent;

namespace Island {
  enum Mode {
    NormalMode,
    InsertMode,
    CommandMode,
    SearchMode,
  };

  static constexpr int ModeCount = SearchMode+1;

  QString EncodeSequence(const QKeyEvent *);
  QString EncodeCharacter(const QKeyEvent *);

  typedef std::pair<int, Qt::KeyboardModifiers> KeyModPair;
  KeyModPair DecodeSequence(const QString &);
  KeyModPair DecodeCharacter(const QString &);
};

#endif
