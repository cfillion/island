#include "names.hpp"

#include "actions.hpp"

using namespace CommandOptions;

const CommandRegistry Island::ActionNames{

  {"back",       &Actions::history_back},
  {"close",      &Actions::tab_close},
  {"forward",    &Actions::history_forward},
  {"goto",       &Actions::tab_goto},
  {"nohlsearch", &Actions::search_clear},
  {"open",       &Actions::open,          EN_ARG},
  {"reload",     &Actions::tab_reload,    EN_FORCE},
  {"stop",       &Actions::tab_stop},
  {"tabopen",    &Actions::tab_open,      EN_ARG},

};
