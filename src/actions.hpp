#ifndef ISLAND_ACTIONS_HPP
#define ISLAND_ACTIONS_HPP

#include "command.hpp"

namespace Actions {
  CommandResult normal_mode(const Command &);
  CommandResult insert_mode(const Command &);
  CommandResult command_mode(const Command &);
  CommandResult open(const Command &);
  CommandResult tab_open(const Command &);
  CommandResult tab_reload(const Command &);
  CommandResult tab_close(const Command &);
  CommandResult history_back(const Command &);
  CommandResult history_forward(const Command &);
};

#endif
