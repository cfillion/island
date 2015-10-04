#ifndef ISLAND_COMMAND_HPP
#define ISLAND_COMMAND_HPP

#include <QString>
#include <set>

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
    OPT_ARG   = 1<<1,
    OPT_FORCE = 1<<2,
  };
};

struct CommandEntry {
  CommandEntry(const QString &n, const CommandFunc fp = 0, int fl = 0)
    : name(n), func(fp), flags(fl) {}

  QString name;
  CommandFunc func;
  int flags;

  bool testFlag(const CommandOptions::Flag f) const
  {
    return flags & f;
  }

  bool operator<(const CommandEntry &o) const
  {
    return name < o.name;
  }
};

typedef std::set<CommandEntry> CommandRegistry;

class UseCommandRegistry {
public:
  UseCommandRegistry(const CommandRegistry *);
  ~UseCommandRegistry();

private:
  CommandRegistry *m_backup;
};

class Command {
public:
  Command(const CommandFunc &func, const QString &arg = QString(),
      const bool force = false, const int counter = -1, void *data = 0);
  Command(const QString &cmd);

  bool isValid() const { return m_isValid; }
  CommandFunc func() const { return m_func; }
  const QString &arg() const { return m_arg; }
  bool hasArgument() const { return !m_arg.isEmpty(); }
  int counter() const { return m_counter; }
  void setCounter(const int n) { m_counter = n; }
  bool hasCounter() const { return m_counter > 0; }
  bool force() const { return m_force; }

  void setData(void *ptr) { m_data = ptr; }
  template<class T> T data() const { return static_cast<T>(m_data); }

  CommandResult exec() const;

private:
  static CommandRegistry *s_registry;
  friend UseCommandRegistry;

  static bool matchCommand(const QString &name, const CommandEntry **entry);

  bool m_isValid;
  void *m_data;
  int m_counter;
  CommandFunc m_func;
  QString m_arg;
  QString m_error;
  bool m_force;
};

#endif
