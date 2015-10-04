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
    EN_ARG   = 1<<1,
    EN_FORCE = 1<<2,
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
      const CommandOptions::Variant va = CommandOptions::VA_DEFAULT,
      const int counter = -1, void *data = 0);
  Command(const QString &cmd);

  bool isValid() const { return m_isValid; }
  CommandFunc func() const { return m_func; }

  bool hasArgument() const { return !m_arg.isEmpty(); }
  const QString &arg() const { return m_arg; }

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

  static bool matchCommand(const QString &, const CommandEntry **);
  bool checkVariant(const CommandEntry *) const;

  bool m_isValid;
  void *m_data;
  int m_counter;
  CommandFunc m_func;
  QString m_arg;
  QString m_error;
  CommandOptions::Variant m_variant;
};

#endif
