#include "command.hpp"

#include <cassert>
#include <QRegularExpression>

using namespace CommandOptions;

bool CommandEntry::hasFlag(const CommandOptions::Flag f) const
{
  return flags & f;
}

bool CommandEntry::operator<(const CommandEntry &o) const
{
  return name < o.name;
}

UseCommandRegistry::UseCommandRegistry(const CommandRegistry *reg)
  : m_backup(Command::s_registry)
{
  Command::s_registry = const_cast<CommandRegistry *>(reg);
}

UseCommandRegistry::~UseCommandRegistry()
{
  Command::s_registry = m_backup;
}

CommandRegistry *Command::s_registry = 0;

Command::Command(const CommandFunc &func, const QString &arg,
    const Variant va, const int counter, void *data)
  : m_isValid(true), m_data(data), m_counter(counter),
    m_func(func), m_arg(arg), m_variant(va)
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
    "(?<variant>[\\!])?"
    "(?:\\s+(?<args>.+))?"
    "\\z"
  );

  const auto match = pattern.match(input);

  const QString counter = match.captured("counter");
  const QString name = match.captured("name");
  const QString args = match.captured("args");
  const QChar variant = match.captured("variant")[0];

  m_variant = variant == '!' ? VA_FORCE : VA_DEFAULT;

  const CommandEntry *entry = 0;

  if(!match.hasMatch() || !findCommand(name, &entry) || !checkVariant(entry)) {
    m_error = "Not a command: " + input;
    return;
  }
  else if(!args.isEmpty() && !entry->hasFlag(EN_ARG)) {
    m_error = "Trailing characters";
    return;
  }

  if(!counter.isEmpty())
    m_counter = counter.toInt();

  m_isValid = true;
  m_func = entry->func;
  m_arg = args;
}

bool Command::findCommand(const QString &name, const CommandEntry **entry)
{
  const CommandList matches = findCommands(name);

  if(matches.empty() || (matches.size() > 1 && matches.front()->name != name))
    return false;

  if(entry)
    *entry = matches.front();

  return true;
};

CommandList Command::findCommands(const QString &prefix)
{
  assert(s_registry);

  auto it = s_registry->lower_bound({prefix});

  CommandList list;

  while(it != s_registry->end() && it->name.startsWith(prefix)) {
    list.push_back(&*it);
    it++;
  }

  return list;
}

bool Command::checkVariant(const CommandEntry *entry) const
{
  switch(m_variant) {
  case VA_DEFAULT:
    return true;
  case VA_FORCE:
    return entry->hasFlag(EN_FORCE);
  }

  return false;
}

CommandResult Command::exec() const
{
  if(m_isValid)
    return m_func(*this);
  else
    return CommandResult{false, m_error};
}
