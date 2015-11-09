#include "names.hpp"

#include "actions.hpp"

using namespace CommandOptions;

const CommandRegistry Island::ActionNames{

  {"back",       &Actions::history_back,    CO_FORCE | CO_TAB | CO_CURPG},
  {"close",      &Actions::tab_close,       CO_TAB},
  {"forward",    &Actions::history_forward, CO_FORCE | CO_TAB | CO_CURPG},
  {"goto",       &Actions::tab_goto,        0},
  {"nohlsearch", &Actions::search_clear,    CO_TAB},
  {"open",       &Actions::open,            CO_ARG | CO_TAB},
  {"reload",     &Actions::tab_reload,      CO_FORCE | CO_TAB},
  {"stop",       &Actions::tab_stop,        CO_TAB},
  {"tabdo",      &Actions::tab_do,          CO_ARG},
  {"tabopen",    &Actions::tab_open,        CO_ARG},

};
