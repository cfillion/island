#ifndef ISLAND_COMMAND_HPP
#define ISLAND_COMMAND_HPP

#include <QRegularExpressionMatch>
#include <QString>
#include <set>
#include <vector>

class Command;
class Window;

struct CommandResult {
  CommandResult(const bool _ok = true, const QString &msg = QString())
    : ok(_ok), message(msg) {}

  bool ok;
  QString message;
};

typedef CommandResult (*CommandFunc)(const Command &);

namespace CommandOptions {
  enum Flag {
    EN_ARG   = 1<<0,
    EN_FORCE = 1<<1,
    EN_TABDO = 1<<2,
  };

  enum Variant {
    VA_DEFAULT,
    VA_FORCE,
  };
};

struct CommandEntry {
  CommandEntry(const QString &n, const CommandFunc fp = 0, int fl = 0)
    : name(n), func(fp), flags(fl) {}

  QString name;
  CommandFunc func;
  int flags;

  bool hasFlag(const CommandOptions::Flag) const;
  bool operator<(const CommandEntry &) const;
};

typedef std::set<CommandEntry> CommandRegistry;
typedef std::vector<const CommandEntry *> CommandList;

class UseCommandRegistry {
public:
  UseCommandRegistry(const CommandRegistry *);
  ~UseCommandRegistry();

private:
  CommandRegistry *m_backup;
};

#define ISLAND_PARSER_COMPONENT(x) \
  QString x() const { return m_match.captured(#x); } \
  int x ## Start() const { return m_match.capturedStart(#x); } \
  int x ## End() const { return m_match.capturedEnd(#x); }

class CommandParser {
public:
  CommandParser(const QString &input);

  bool isValid() const { return m_match.hasMatch(); }

  ISLAND_PARSER_COMPONENT(counter);
  ISLAND_PARSER_COMPONENT(name);
  ISLAND_PARSER_COMPONENT(variant);
  ISLAND_PARSER_COMPONENT(argument);

private:
  QRegularExpressionMatch m_match;
};

#undef ISLAND_PARSER_COMPONENT

class Command {
public:
  static CommandList findCommands(const QString &);

  Command(const CommandFunc &func, const QString &arg = QString(),
      const CommandOptions::Variant va = CommandOptions::VA_DEFAULT,
      const int counter = -1, void *data = 0);
  Command(const QString &cmd);

  bool isValid() const { return m_isValid; }
  const QString &errorString() const { return m_error; }

  CommandFunc func() const { return m_func; }
  const CommandEntry *entry() const { return m_entry; }
  bool hasFlag(const CommandOptions::Flag) const;

  bool hasArgument() const { return !m_arg.isEmpty(); }
  const QString &argument() const { return m_arg; }

  void setCounter(const int n) { m_counter = n; }
  bool hasCounter() const { return m_counter > 0; }
  int counter() const { return m_counter; }

  void setData(void *ptr) { m_data = ptr; }
  template<class T> T data() const { return static_cast<T>(m_data); }

  CommandOptions::Variant variant() const { return m_variant; }

  CommandResult exec() const;

private:
  static CommandRegistry *s_registry;
  friend UseCommandRegistry;

  static bool findCommand(const QString &, const CommandEntry ** = 0);
  bool checkVariant() const;

  bool m_isValid;
  void *m_data;
  int m_counter;
  CommandFunc m_func;
  QString m_arg;
  QString m_error;
  CommandOptions::Variant m_variant;
  const CommandEntry *m_entry;
};

#endif
