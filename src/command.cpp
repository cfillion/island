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

Command::Command(const CommandFunc &func, const QString &arg,
    const bool force, const int counter, void *data)
  : m_isValid(true), m_data(data), m_counter(counter),
    m_func(func), m_arg(arg), m_force(force)
{
}

Command::Command(const QString &input)
  : m_isValid(false), m_data(0), m_counter(-1), m_func(0)
{
  static const QRegularExpression pattern(
    "\\A"
    "(?<counter>\\d*)"
    "\\s*"
    "(?<name>[a-zA-Z0-9_]+)"
    "(?<force>\\!)?"
    "(?:\\s+(?<args>.+))?"
    "\\z"
  );

  const auto match = pattern.match(input);

  const QString counter = match.captured("counter");
  const QString name = match.captured("name");
  m_force = !match.captured("force").isEmpty();
  const QString args = match.captured("args");

  const CommandEntry *entry = 0;

  if(!match.hasMatch() || !matchCommand(name, &entry)) {
    m_error = "Not a command: " + input;
    return;
  }
  else if(!args.isEmpty() && !entry->testFlag(CommandOptions::OPT_ARG)) {
    m_error = "Trailing characters";
    return;
  }

  if(!counter.isEmpty())
    m_counter = counter.toInt();

  m_isValid = true;
  m_func = entry->func;
  m_arg = args;
}

bool Command::matchCommand(const QString &name, const CommandEntry **entry)
{
  assert(s_registry);

  const auto match = s_registry->lower_bound({name});

  // no match
  if(match == s_registry->end())
    return false;

  // exact match
  if(match->name == name) {
    *entry = &*match;
    return true;
  }

  // ambiguous match
  const auto next = std::next(match);
  if(next != s_registry->end() && next->name.startsWith(name))
    return false;

  // partial match
  if(match->name.startsWith(name)) {
    *entry = &*match;
    return true;
  }

  return false;
};

CommandResult Command::exec() const
{
  if(m_isValid)
    return m_func(*this);
  else
    return CommandResult{false, m_error};
}
