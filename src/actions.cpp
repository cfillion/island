#include "actions.hpp"

#include "global.hpp"
#include "window.hpp"

using namespace Island;

#define WIN cmd.ptr<Window *>()

CommandResult Actions::normal_mode(const Command &cmd)
{
  WIN->setMode(Normal);
  return CommandResult();
}

CommandResult Actions::insert_mode(const Command &cmd)
{
  WIN->setMode(Insert);
  return CommandResult();
}

CommandResult Actions::command_mode(const Command &cmd)
{
  WIN->setMode(Prompt);
  return CommandResult();
}

CommandResult Actions::close_tab(const Command &cmd)
{
  WIN->closePage(WIN->currentPage());
  return CommandResult();
}
