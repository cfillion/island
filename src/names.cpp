#include "names.hpp"

#include "actions.hpp"

using namespace CommandOptions;

const CommandRegistry Island::ActionNames{

  {"back",       &Actions::history_back,    CO_ARG | CO_FORCE},
  {"close",      &Actions::tab_close,       0},
  {"echo",       &Actions::echo,            CO_ARG},
  {"forward",    &Actions::history_forward, CO_ARG | CO_FORCE},
  {"goto",       &Actions::tab_goto,        0},
  {"nohlsearch", &Actions::search_clear,    0},
  {"open",       &Actions::open,            CO_ARG},
  {"reload",     &Actions::tab_reload,      CO_FORCE},
  {"stop",       &Actions::tab_stop,        0},
  {"tablmove",   &Actions::tab_move_left,   CO_ARG},
  {"tabmove",    &Actions::tab_move,        CO_ARG},
  {"tabnext",    &Actions::tab_next,        0},
  {"tabopen",    &Actions::tab_open,        CO_ARG},
  {"tabprev",    &Actions::tab_prev,        0},
  {"tabrmove",   &Actions::tab_move_right,  CO_ARG},

};
