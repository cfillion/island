#include "actions.hpp"

#include "global.hpp"
#include "page.hpp"
#include "window.hpp"

using namespace Island;

#define WIN cmd.data<Window *>()

static Page *GetPage(const Command &cmd, CommandResult *res)
{
  if(!cmd.hasCounter())
    return WIN->currentPage();
  else if(cmd.counter() <= WIN->pageCount())
    return WIN->page(cmd.counter() - 1);

  res->ok = false;
  res->message = QString("Invalid tab identifier: %1").arg(cmd.counter());

  return 0;
};

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

  CommandResult res;

  if(Page *p = GetPage(cmd, &res))
    p->load(cmd.arg());

  return res;
}

CommandResult Actions::tab_open(const Command &cmd)
{
  const int index = WIN->addPage(cmd.arg());
  WIN->setCurrentTab(index);

  return {};
}

CommandResult Actions::tab_reload(const Command &cmd)
{
  CommandResult res;

  if(Page *p = GetPage(cmd, &res))
    p->reload(!cmd.force());

  return res;
}

CommandResult Actions::tab_close(const Command &cmd)
{
  CommandResult res;

  if(Page *p = GetPage(cmd, &res))
    WIN->closePage(p);

  return res;
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
