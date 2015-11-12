#include "actions.hpp"

#include "global.hpp"
#include "page.hpp"
#include "prompt.hpp"
#include "window.hpp"

using namespace CommandOptions;
using namespace Island;

#define WIN cmd.data<Window *>()

static Page *GetPage(const Window *win, Range *range, CommandResult *res)
{
  const int pageCount = win->pageCount();
  range->resolve(win->currentPageIndex() + 1, pageCount);

  if(!range->hasNext())
    return 0;

  const int current = range->next();

  if(current <= pageCount)
    return win->page(current - 1);

  res->ok = false;
  res->message = QString("Invalid tab identifier: %1").arg(current);

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

  Range range = cmd.range();
  while(Page *p = GetPage(WIN, &range, &res)) {
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

  Range range = cmd.range();
  while(Page *p = GetPage(WIN, &range, &res))
    p->reload(cmd.variant() == VA_DEFAULT);

  return res;
}

CommandResult Actions::tab_close(const Command &cmd)
{
  CommandResult res;

  // Storing the matching pages first to fix closing a range of page.
  // Otherwise the end of the range would no longer be a valid tab id
  // by the time we get there.
  std::vector<Page *> pages;

  Range range = cmd.range();
  while(Page *p = GetPage(WIN, &range, &res))
    pages.push_back(p);

  for(Page *p : pages)
    WIN->closePage(p);

  return res;
}

CommandResult Actions::tab_stop(const Command &cmd)
{
  CommandResult res;

  Range range = cmd.range();
  while(Page *p = GetPage(WIN, &range, &res))
    p->stop();

  return res;
}

CommandResult Actions::tab_goto(const Command &cmd)
{
  CommandResult res;

  Range range = cmd.range();
  while(Page *p = GetPage(WIN, &range, &res))
    WIN->setCurrentPage(p);

  return res;
}

CommandResult Actions::tab_next(const Command &cmd)
{
  Range r(cmd.range());
  r.resolve();

  const int index = WIN->currentPageIndex() + r.counter();

  WIN->setCurrentPageIndex(index % WIN->pageCount());

  return {};
}

CommandResult Actions::tab_prev(const Command &cmd)
{
  Range r(cmd.range());
  r.resolve();

  int index = WIN->currentPageIndex() - r.counter();

  while(index < 0)
    index += WIN->pageCount();

  WIN->setCurrentPageIndex(index);

  return {};
}

CommandResult Actions::history_back(const Command &cmd)
{
  const int jumpSize = cmd.variant() == VA_DEFAULT
    ? std::max(1, cmd.argument().toInt()) * -1
    : std::numeric_limits<int>::min();

  CommandResult res;

  Range range = cmd.range();
  while(Page *p = GetPage(WIN, &range, &res)) {
    if(!p->historyMotion(jumpSize) && cmd.variant() != VA_FORCE)
      return {false, "End of history"};
  }

  return res;
}

CommandResult Actions::history_forward(const Command &cmd)
{
  const int jumpSize = cmd.variant() == VA_DEFAULT
    ? std::max(1, cmd.argument().toInt())
    : std::numeric_limits<int>::max();

  CommandResult res;

  Range range = cmd.range();
  while(Page *p = GetPage(WIN, &range, &res)) {
    if(!p->historyMotion(jumpSize) && cmd.variant() != VA_FORCE)
      return {false, "Start of history"};
  }

  return res;
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

CommandResult Actions::search_prev(const Command &)
{
  return {false, "Not implemented"};
}

CommandResult Actions::search_clear(const Command &cmd)
{
  CommandResult res;

  Range range = cmd.range();
  while(Page *p = GetPage(WIN, &range, &res))
    p->findText(QString());

  return res;
}
