#include "names.hpp"

#include "actions.hpp"

#define ACTION(x) {#x, Actions::x}

using namespace CommandOptions;

const CommandRegistry Island::ActionNames{

  {"back",       &Actions::history_back},
  {"close",      &Actions::tab_close},
  {"forward",    &Actions::history_forward},
  {"nohlsearch", &Actions::search_clear},
  {"open",       &Actions::open,          EN_ARG},
  {"reload",     &Actions::tab_reload,    EN_FORCE},
  {"tabclose",   &Actions::tab_close},
  {"tabopen",    &Actions::tab_open,      EN_ARG},
  {"tabreload",  &Actions::tab_reload,    EN_FORCE},

};
