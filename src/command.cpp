#include "command.hpp"

#include <cassert>
#include <QRegularExpression>

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
  : m_isValid(false), m_data(0), m_counter(-1), m_func(0), m_input(input)
{
  assert(s_registry);

  static const QRegularExpression pattern("\\A(\\d*)(\\S+)\\z");
  const auto match = pattern.match(input);

  const QString counter = match.captured(1);
  if(!counter.isEmpty())
    m_counter = counter.toInt();

  const QString name = match.captured(2);
  const auto lower = s_registry->lower_bound(name);
  const bool partialMatch = lower != s_registry->end()
    && lower->first.startsWith(name);

  if(!name.isEmpty() && partialMatch) {
    m_isValid = true;
    m_func = lower->second;
  }
}

CommandResult Command::exec() const
{
  if(m_isValid)
    return m_func(*this);
  else
    return CommandResult{false, "Not a command: " + m_input};
}
