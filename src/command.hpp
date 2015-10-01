#ifndef ISLAND_COMMAND_HPP
#define ISLAND_COMMAND_HPP

#include <QString>
#include <set>
#include <tuple>

class Command;
class Window;

struct CommandResult {
  CommandResult(const bool _ok = true, const QString &msg = QString())
    : ok(_ok), message(msg) {}

  bool ok;
  QString message;
};

typedef CommandResult (*CommandFunc)(const Command &);
typedef std::tuple<QString, CommandFunc, bool> CommandEntry;
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
  Command(const CommandFunc &func,
    const QString &arg = QString(), const int counter = -1);
  Command(const QString &cmd);

  bool isValid() const { return m_isValid; }
  CommandFunc func() const { return m_func; }
  const QString &arg() const { return m_arg; }
  int counter() const { return m_counter; }
  void setCounter(const int n) { m_counter = n; }
  bool hasCounter() const { return m_counter > 0; }

  void setData(void *ptr) { m_data = ptr; }
  template<class T> T data() const { return static_cast<T>(m_data); }

  CommandResult exec() const;

private:
  static CommandRegistry *s_registry;
  friend UseCommandRegistry;

  bool m_isValid;
  void *m_data;
  int m_counter;
  CommandFunc m_func;
  QString m_arg;
  QString m_error;
};

#endif
