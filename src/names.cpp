#include "names.hpp"

#include "actions.hpp"

#define ACTION(x) {#x, Actions::x}

const CommandRegistry Island::ActionNames{
  {"insert", &Actions::insert_mode},
  {"close", &Actions::tab_close},
  {"back", &Actions::history_back},
  {"forward", &Actions::history_forward},
};
