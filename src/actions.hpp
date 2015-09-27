#ifndef ISLAND_ACTIONS_HPP
#define ISLAND_ACTIONS_HPP

#include "command.hpp"

namespace Actions {
  CommandResult normal(const Command &);
  CommandResult insert(const Command &);
  CommandResult prompt(const Command &);
};

#endif
