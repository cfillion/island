#include "actions.hpp"

#include "global.hpp"
#include "window.hpp"

using namespace Island;

#define WIN cmd.data<Window *>()

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
  if(cmd.counter() == -1)
    WIN->closePage(WIN->currentPage());
  else
    WIN->closeTab(cmd.counter()-1);

  return CommandResult();
}
