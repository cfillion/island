#include "names.hpp"

#include "actions.hpp"

#define ACTION(x) {#x, Actions::x}

using namespace CommandOptions;

const CommandRegistry Island::ActionNames{

  {"insert",    &Actions::insert_mode},
  {"back",      &Actions::history_back},
  {"forward",   &Actions::history_forward},
  {"close",     &Actions::tab_close},
  {"tabclose",  &Actions::tab_close},
  {"open",      &Actions::open,          EN_ARG},
  {"tabopen",   &Actions::tab_open,      EN_ARG},
  {"reload",    &Actions::tab_reload,    EN_FORCE},
  {"tabreload", &Actions::tab_reload,    EN_FORCE},

};
