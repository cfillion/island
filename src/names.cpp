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
  {"open",      &Actions::open,          OPT_ARG},
  {"tabopen",   &Actions::tab_open,      OPT_ARG},
  {"reload",    &Actions::tab_reload,    OPT_FORCE},
  {"tabreload", &Actions::tab_reload,    OPT_FORCE},

};
