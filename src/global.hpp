#ifndef ISLAND_GLOBAL_HPP
#define ISLAND_GLOBAL_HPP

#include <QString>

namespace Island {
  enum Mode {
    NormalMode,
    InsertMode,

    // prompt modes (>= CommandMode)
    CommandMode,
    SearchForwardMode,
    SearchBackwardMode,
  };
};

#endif
