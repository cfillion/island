#include "names.hpp"

#include "actions.hpp"

#define ACTION(x) {#x, Actions::x}

const CommandRegistry Island::ActionNames{
  ACTION(normal),
  ACTION(insert),
  ACTION(prompt),
};
