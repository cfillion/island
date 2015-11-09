#include "names.hpp"

#include "actions.hpp"

using namespace CommandOptions;

const CommandRegistry Island::ActionNames{

  {"back",       &Actions::history_back,    EN_TAB},
  {"close",      &Actions::tab_close,       EN_TAB},
  {"forward",    &Actions::history_forward, EN_TAB},
  {"goto",       &Actions::tab_goto,        0},
  {"nohlsearch", &Actions::search_clear,    EN_TAB},
  {"open",       &Actions::open,            EN_ARG | EN_TAB},
  {"reload",     &Actions::tab_reload,      EN_FORCE | EN_TAB},
  {"stop",       &Actions::tab_stop,        EN_TAB},
  {"tabdo",      &Actions::tab_do,          EN_ARG},
  {"tabopen",    &Actions::tab_open,        EN_ARG},

};
