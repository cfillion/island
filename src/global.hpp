#ifndef ISLAND_GLOBAL_HPP
#define ISLAND_GLOBAL_HPP

#include <QString>

class QKeyEvent;

namespace Island {
  enum Mode {
    Normal,
    Insert,
    Prompt,
    Search,
  };

  static constexpr int ModeCount = Prompt+1;

  QString KeyEventToSequence(const QKeyEvent *);
  QString GetKeyEventCharacter(const QKeyEvent *);
};

#endif
