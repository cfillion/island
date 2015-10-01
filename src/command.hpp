#ifndef ISLAND_COMMAND_HPP
#define ISLAND_COMMAND_HPP

#include <map>
#include <QString>
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
typedef std::map<QString, CommandFunc> CommandRegistry;

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
    const std::vector<QString> &argv = {}, const int counter = -1);
  Command(const QString &cmd);

  bool isValid() const { return m_isValid; }
  CommandFunc func() const { return m_func; }

  int argc() const { return m_argv.size(); }
  const QString &arg(const int i) const { return m_argv[i]; }

  void setCounter(const int n) { m_counter = n; }
  int counter() const { return m_counter; }
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
  std::vector<QString> m_argv;
  QString m_error;
};

#endif
