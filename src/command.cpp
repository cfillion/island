#include "command.hpp"

Command::Command(const CommandCallback &func)
  : m_ptr(0), m_func(func)
{
}

Command::Command(const QString &input)
  : m_ptr(0)
{
}

CommandResult Command::exec(void *ptr, const int repeat)
{
  m_ptr = ptr;
  const auto result = m_func(*this);
  m_ptr = 0;

  return result;
}
