#include "actions.hpp"

#include "global.hpp"
#include "page.hpp"
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

CommandResult Actions::tab_close(const Command &cmd)
{
  if(!cmd.hasCounter())
    WIN->closePage(WIN->currentPage());
  else if(cmd.counter() <= WIN->pageCount())
    WIN->closeTab(cmd.counter()-1);
  else {
    return CommandResult{false, "Invalid tab identifier: "
      + QString::number(cmd.counter())};
  }

  return CommandResult();
}

CommandResult Actions::history_back(const Command &cmd)
{
  WIN->currentPage()->historyMotion(-1);
  return CommandResult();
}

CommandResult Actions::history_forward(const Command &cmd)
{
  WIN->currentPage()->historyMotion(1);
  return CommandResult();
}
