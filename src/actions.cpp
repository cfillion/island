#include "actions.hpp"

#include "global.hpp"
#include "page.hpp"
#include "window.hpp"

using namespace Island;

#define WIN cmd.data<Window *>()

static const QString E_INVALID_TAB =
  QStringLiteral("Invalid tab identifier: %1");

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

CommandResult Actions::open(const Command &cmd)
{
  if(!cmd.hasArgument())
    return {false, "No url or search query"};

  if(!cmd.hasCounter())
    WIN->currentPage()->load(cmd.arg());
  else if(cmd.counter() <= WIN->pageCount())
    WIN->page(cmd.counter()-1)->load(cmd.arg());
  else
    return {false, E_INVALID_TAB.arg(cmd.counter())};

  return {};
}

CommandResult Actions::tab_open(const Command &cmd)
{
  const int index = WIN->addPage(cmd.arg());
  WIN->setCurrentTab(index);

  return {};
}

CommandResult Actions::tab_close(const Command &cmd)
{
  if(!cmd.hasCounter())
    WIN->closePage(WIN->currentPage());
  else if(cmd.counter() <= WIN->pageCount())
    WIN->closeTab(cmd.counter()-1);
  else
    return {false, E_INVALID_TAB.arg(cmd.counter())};

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
