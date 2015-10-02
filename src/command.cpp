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

Command::Command(const CommandFunc &func, const QString &arg, const int counter)
  : m_isValid(true), m_data(0), m_counter(counter), m_func(func), m_arg(arg)
{
}

Command::Command(const QString &input)
  : m_isValid(false), m_data(0), m_counter(-1), m_func(0)
{
  static const QRegularExpression pattern(
    "\\A(\\d*)\\s*(\\S+)(?:\\s+(.+))?\\z",
    QRegularExpression::OptimizeOnFirstUsageOption);

  const auto match = pattern.match(input);

  const QString counter = match.captured(1);
  const QString name = match.captured(2);
  const QString args = match.captured(3);

  CommandEntry *entry = 0;

  if(!match.hasMatch() || !matchCommand(name, &entry)) {
    m_error = "Not a command: " + input;
    return;
  }
  else if(!args.isEmpty() && !entry->acceptArgument) {
    m_error = "Trailing characters";
    return;
  }

  if(!counter.isEmpty())
    m_counter = counter.toInt();

  m_isValid = true;
  m_func = entry->func;
  m_arg = args;
}

bool Command::matchCommand(const QString &name, CommandEntry **entry)
{
  assert(s_registry);

  const auto match = s_registry->lower_bound({name});

  // no match
  if(match == s_registry->end())
    return false;

  // exact match
  if(match->name == name) {
    *entry = const_cast<CommandEntry *>(&*match);
    return true;
  }

  // ambiguous match
  const auto next = std::next(match);
  if(next != s_registry->end() && next->name.startsWith(name))
    return false;

  // partial match
  if(match->name.startsWith(name)) {
    *entry = const_cast<CommandEntry *>(&*match);
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
