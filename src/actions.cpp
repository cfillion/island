#include "actions.hpp"

#include "global.hpp"
#include "page.hpp"
#include "prompt.hpp"
#include "window.hpp"

using namespace CommandOptions;
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

CommandResult Actions::echo(const Command &cmd)
{
  return {true, cmd.argument()};
}

CommandResult Actions::open(const Command &cmd)
{
  if(!cmd.hasArgument())
    return {false, "No url or search query"};

  CommandResult res;

  if(Page *p = GetPage(cmd, &res)) {
    if(!p->load(cmd.argument()))
      res = {false, "Invalid url"};
  }

  return res;
}

CommandResult Actions::tab_open(const Command &cmd)
{
  const int index = WIN->addPage(cmd.argument());
  WIN->setCurrentPageIndex(index);

  return {};
}

CommandResult Actions::tab_reload(const Command &cmd)
{
  CommandResult res;

  if(Page *p = GetPage(cmd, &res))
    p->reload(cmd.variant() == VA_DEFAULT);

  return res;
}

CommandResult Actions::tab_close(const Command &cmd)
{
  CommandResult res;

  if(Page *p = GetPage(cmd, &res))
    WIN->closePage(p);

  return res;
}

CommandResult Actions::tab_stop(const Command &cmd)
{
  CommandResult res;

  if(Page *p = GetPage(cmd, &res))
    p->stop();

  return res;
}

CommandResult Actions::tab_goto(const Command &cmd)
{
  CommandResult res;

  if(Page *p = GetPage(cmd, &res))
    WIN->setCurrentPage(p);

  return res;
}

CommandResult Actions::tab_do(const Command &cmd)
{
  if(!cmd.hasArgument())
    return {false, "Argument required"};

  Command tabCmd(cmd.argument());
  tabCmd.setData(cmd.data<void *>());

  if(!tabCmd.isValid())
    return {false, tabCmd.errorString()};
  else if(!tabCmd.hasFlag(CO_TAB))
    return {false, "Unsupported command"};
  else if(tabCmd.hasFlag(CO_CURPG))
    tabCmd.setCounter(cmd.counter());

  CommandResult res;

  Page *current = WIN->currentPage();
  const int pageCount = WIN->pageCount();

  // iterating over tabs in reverse is required for :tabdo close to work
  for(int i = pageCount; i > 0; i--) {
    if(tabCmd.hasFlag(CO_CURPG))
      WIN->setCurrentPageIndex(i-1);
    else
      tabCmd.setCounter(i);

    res = tabCmd.exec();

    if(!res.message.isEmpty())
      res.message.prepend(QString("(tab %1) ").arg(i));

    if(!res.ok)
      break;
  }

  WIN->setCurrentPage(current);

  return res;
}

CommandResult Actions::history_back(const Command &cmd)
{
  if(cmd.variant() == VA_FORCE) {
    WIN->currentPage()->historyMotion(std::numeric_limits<int>::min());
    return {};
  }

  const int motionSize = std::max(1, cmd.counter());

  if(WIN->currentPage()->historyMotion(motionSize * -1))
    return {};
  else
    return {false, "End of history"};
}

CommandResult Actions::history_forward(const Command &cmd)
{
  if(cmd.variant() == VA_FORCE) {
    WIN->currentPage()->historyMotion(std::numeric_limits<int>::max());
    return {};
  }

  const int motionSize = std::max(1, cmd.counter());

  if(WIN->currentPage()->historyMotion(motionSize))
    return {};
  else
    return {false, "Start of history"};
}

CommandResult Actions::prompt_execute(const Command &cmd)
{
  WIN->prompt()->send();
  return {};
}

CommandResult Actions::prompt_home(const Command &cmd)
{
  WIN->prompt()->home(false);
  return {};
}

CommandResult Actions::prompt_end(const Command &cmd)
{
  WIN->prompt()->end(false);
  return {};
}

CommandResult Actions::prompt_left(const Command &cmd)
{
  WIN->prompt()->cursorBackward(false);
  return {};
}

CommandResult Actions::prompt_right(const Command &cmd)
{
  WIN->prompt()->cursorForward(false);
  return {};
}

CommandResult Actions::prompt_clear(const Command &cmd)
{
  WIN->prompt()->home(true);
  WIN->prompt()->del();
  return {};
}

CommandResult Actions::prompt_complete(const Command &cmd)
{
  WIN->prompt()->complete(1);
  return {};
}

CommandResult Actions::prompt_complete_reverse(const Command &cmd)
{
  WIN->prompt()->complete(-1);
  return {};
}

CommandResult Actions::search_forward(const Command &cmd)
{
  WIN->setMode(SearchForwardMode);
  return {};
}

CommandResult Actions::search_backward(const Command &cmd)
{
  WIN->setMode(SearchBackwardMode);
  return {};
}

CommandResult Actions::search_next(const Command &)
{
  return {false, "Not implemented"};
}

CommandResult Actions::search_previous(const Command &)
{
  return {false, "Not implemented"};
}

CommandResult Actions::search_clear(const Command &cmd)
{
  CommandResult res;

  if(Page *p = GetPage(cmd, &res))
    p->findText(QString());

  return res;
}
