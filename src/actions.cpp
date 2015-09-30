#include "actions.hpp"

#include "global.hpp"
#include "page.hpp"
#include "window.hpp"

using namespace Island;

#define WIN cmd.data<Window *>()

CommandResult Actions::normal_mode(const Command &cmd)
{
  WIN->setMode(Normal);
  return {};
}

CommandResult Actions::insert_mode(const Command &cmd)
{
  WIN->setMode(Insert);
  return {};
}

CommandResult Actions::command_mode(const Command &cmd)
{
  WIN->setMode(Prompt);
  return {};
}

CommandResult Actions::tab_close(const Command &cmd)
{
  if(!cmd.hasCounter())
    WIN->closePage(WIN->currentPage());
  else if(cmd.counter() <= WIN->pageCount())
    WIN->closeTab(cmd.counter()-1);
  else {
    return {false, "Invalid tab identifier: "
      + QString::number(cmd.counter())};
  }

  return {};
}

CommandResult Actions::history_back(const Command &cmd)
{
  if(WIN->currentPage()->historyMotion(-1))
    return{};
  else
    return {false, "End of history"};
}

CommandResult Actions::history_forward(const Command &cmd)
{
  if(WIN->currentPage()->historyMotion(1))
    return {};
  else
    return {false, "Start of history"};
}
