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

Command::Command(const CommandFunc &func,
  const std::vector<QString> &argv, const int counter)
  : m_isValid(true), m_data(0), m_counter(counter), m_func(func), m_argv(argv)
{
}

Command::Command(const QString &input)
  : m_isValid(false), m_data(0), m_counter(-1), m_func(0)
{
  assert(s_registry);

  static const QRegularExpression pattern(
    "\\A(\\d*)([^\x20]+)(?:\x20(.+))?\\z",
    QRegularExpression::OptimizeOnFirstUsageOption);

  const auto match = pattern.match(input);

  const QString counter = match.captured(1);
  if(!counter.isEmpty())
    m_counter = counter.toInt();

  const QString name = match.captured(2);

  const auto lower = s_registry->lower_bound(name);
  const bool partialMatch = lower != s_registry->end()
    && lower->first.startsWith(name);

  if(match.hasMatch() && partialMatch) {
    m_isValid = true;
    m_func = lower->second;
  }
  else
    m_error = "Not a command: " + input;

  const QString args = match.captured(3);
  if(!args.isEmpty()) {
    static const QRegularExpression argp(
      "[^\x20]+",
      QRegularExpression::OptimizeOnFirstUsageOption);

    auto it = argp.globalMatch(args);

    while(it.hasNext())
      m_argv.push_back(it.next().captured(0));
  }
}

CommandResult Command::exec() const
{
  if(m_isValid)
    return m_func(*this);
  else
    return CommandResult{false, m_error};
}
