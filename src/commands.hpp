#ifndef ISLAND_COMMANDS_HPP
#define ISLAND_COMMANDS_HPP

#include <functional>
#include <map>
#include <QStringList>

class Window;

typedef std::function<void(Window *)> Command;

namespace Commands {
  void insert(Window *);
  void normal(Window *);
  void prompt(Window *);
};

#endif
