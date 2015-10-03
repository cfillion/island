#include "names.hpp"

#include "actions.hpp"

#define ACTION(x) {#x, Actions::x}

using namespace CommandOptions;

const CommandRegistry Island::ActionNames{

  {"insert",    &Actions::insert_mode,     ARG_OFF},
  {"back",      &Actions::history_back,    ARG_OFF},
  {"forward",   &Actions::history_forward, ARG_OFF},
  {"close",     &Actions::tab_close,       ARG_OFF},
  {"tabclose",  &Actions::tab_close,       ARG_OFF},
  {"open",      &Actions::open,            ARG_ALL},
  {"tabopen",   &Actions::tab_open,        ARG_ALL},

};
