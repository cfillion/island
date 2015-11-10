#include "names.hpp"

#include "actions.hpp"

using namespace CommandOptions;

const CommandRegistry Island::ActionNames{

  {"back",       &Actions::history_back,    CO_FORCE},
  {"close",      &Actions::tab_close,       0},
  {"echo",       &Actions::echo,            CO_ARG},
  {"forward",    &Actions::history_forward, CO_FORCE},
  {"goto",       &Actions::tab_goto,        0},
  {"nohlsearch", &Actions::search_clear,    0},
  {"open",       &Actions::open,            CO_ARG},
  {"reload",     &Actions::tab_reload,      CO_FORCE},
  {"stop",       &Actions::tab_stop,        0},
  {"tabopen",    &Actions::tab_open,        CO_ARG},

};
