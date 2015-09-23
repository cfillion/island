#include "commands.hpp"

#include "window.hpp"

using namespace Island;

void Commands::insert(Window *w)
{
  w->setMode(Insert);
}

void Commands::normal(Window *w)
{
  w->setMode(Normal);
}

void Commands::prompt(Window *w)
{
  w->setMode(Prompt);
}
