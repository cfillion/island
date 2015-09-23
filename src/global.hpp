#ifndef ISLAND_GLOBAL_HPP
#define ISLAND_GLOBAL_HPP

#include <QStringList>

namespace Island {
  enum Mode {
    Normal,
    Insert,
    Prompt,
    Search,
  };

  static constexpr int ModeCount = Prompt+1;

  typedef QStringList Buffer;

  Buffer InputToBuffer(const QString &);
};

#endif
