#ifndef ISLAND_GLOBAL_HPP
#define ISLAND_GLOBAL_HPP

#include <QString>

class QKeyEvent;

namespace Island {
  enum Mode {
    NormalMode,
    InsertMode,
    CommandMode,
    SearchMode,
  };

  static constexpr int ModeCount = SearchMode+1;

  QString KeyEventToSequence(const QKeyEvent *);
  QString GetKeyEventCharacter(const QKeyEvent *);
};

#endif
