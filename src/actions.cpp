#include "actions.hpp"

#include "global.hpp"
#include "window.hpp"

using namespace Island;

#define WIN cmd.ptr<Window *>()

CommandResult Actions::normal(const Command &cmd)
{
  WIN->setMode(Normal);
  return CommandResult();
}

CommandResult Actions::insert(const Command &cmd)
{
  WIN->setMode(Insert);
  return CommandResult();
}

CommandResult Actions::prompt(const Command &cmd)
{
  WIN->setMode(Prompt);
  return CommandResult();
}
