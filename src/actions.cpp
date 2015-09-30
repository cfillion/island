#include "actions.hpp"

#include "global.hpp"
#include "page.hpp"
#include "window.hpp"

using namespace Island;

#define WIN cmd.data<Window *>()

CommandResult Actions::normal_mode(const Command &cmd)
{
  WIN->setMode(NormalMode);
  return {};
}

CommandResult Actions::insert_mode(const Command &cmd)
{
  WIN->setMode(InsertMode);
  return {};
}

CommandResult Actions::command_mode(const Command &cmd)
{
  WIN->setMode(CommandMode);
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
  const int motionSize = std::max(1, cmd.counter());

  if(WIN->currentPage()->historyMotion(motionSize * -1))
    return{};
  else
    return {false, "End of history"};
}

CommandResult Actions::history_forward(const Command &cmd)
{
  const int motionSize = std::max(1, cmd.counter());

  if(WIN->currentPage()->historyMotion(motionSize))
    return {};
  else
    return {false, "Start of history"};
}
