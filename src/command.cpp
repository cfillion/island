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
  assert(s_registry);

  const QString cleanInput = input.simplified();

  static const QRegularExpression pattern(
    "\\A(\\d*)\\s*(\\S+)(?:\\s+(.+))?\\z",
    QRegularExpression::OptimizeOnFirstUsageOption);

  const auto match = pattern.match(cleanInput);

  const QString counter = match.captured(1);
  const QString name = match.captured(2);
  const QString args = match.captured(3);

  const auto lower = s_registry->lower_bound({name});
  const auto next = std::next(lower);

  const bool partialMatch = lower != s_registry->end()
    && std::get<QString>(*lower).startsWith(name);

  const bool nextMatch = next != s_registry->end()
    && std::get<QString>(*next).startsWith(name);

  const bool uniqueMatch = partialMatch
    && (!nextMatch || std::get<QString>(*lower) == name);

  if(!match.hasMatch() || !uniqueMatch) {
    m_error = "Not a command: " + cleanInput;
    return;
  }
  else if(!args.isEmpty() && !std::get<bool>(*lower)) {
    m_error = "Trailing characters";
    return;
  }

  if(!counter.isEmpty())
    m_counter = counter.toInt();

  m_isValid = true;
  m_func = std::get<CommandFunc>(*lower);
  m_arg = args;
}

CommandResult Command::exec() const
{
  if(m_isValid)
    return m_func(*this);
  else
    return CommandResult{false, m_error};
}
