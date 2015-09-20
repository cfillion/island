#ifndef ISLAND_GLOBAL_HPP
#define ISLAND_GLOBAL_HPP

#include <QStringList>

namespace Island {
  enum Mode {
    Normal = 0,
    Prompt = 1<<0,
  };

  typedef QStringList Buffer;

  Buffer InputToBuffer(const QString &);
};

#endif
