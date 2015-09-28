#include "command.hpp"

CommandRegistry *Command::s_registry = 0;

UseCommandRegistry::UseCommandRegistry(const CommandRegistry *reg)
  : m_backup(Command::s_registry)
{
  Command::s_registry = const_cast<CommandRegistry *>(reg);
}

UseCommandRegistry::~UseCommandRegistry()
{
  Command::s_registry = m_backup;
}

Command::Command(const CommandFunc &func)
  : m_isValid(true), m_data(0), m_counter(-1), m_func(func)
{
}

Command::Command(const QString &input)
  : m_isValid(false), m_data(0), m_counter(-1), m_func(0)
{
  if(s_registry && s_registry->count(input)) {
    m_isValid = true;
    m_func = s_registry->at(input);
  }
}

CommandResult Command::exec() const
{
  return m_func(*this);
}
