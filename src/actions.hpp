#ifndef ISLAND_ACTIONS_HPP
#define ISLAND_ACTIONS_HPP

#include "command.hpp"

namespace Actions {
  CommandResult normal_mode(const Command &);
  CommandResult insert_mode(const Command &);
  CommandResult command_mode(const Command &);
  CommandResult close_tab(const Command &);
};

#endif
