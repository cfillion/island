#include "names.hpp"

#include "actions.hpp"

#define ACTION(x) {#x, Actions::x}

const CommandRegistry Island::ActionNames{
  {"insert", &Actions::insert_mode, false},
  {"close", &Actions::tab_close, false},
  {"tabclose", &Actions::tab_close, false},
  {"back", &Actions::history_back, false},
  {"forward", &Actions::history_forward, false},
  {"open", &Actions::open, true},
  {"tabopen", &Actions::tab_open, true},
};
