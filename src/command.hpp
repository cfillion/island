#ifndef ISLAND_COMMAND_HPP
#define ISLAND_COMMAND_HPP

#include <map>
#include <QString>

class Command;
class Window;

struct CommandResult {
  bool ok = true;
  QString message = QString();
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
  Command(const CommandFunc &cmd);
  Command(const QString &cmd);

  bool isValid() const { return m_isValid; }
  CommandFunc func() const { return m_func; }
  template<class T> T ptr() const { return static_cast<T>(m_ptr); }

  CommandResult exec(void *ptr, const int repeat = 1);

private:
  static CommandRegistry *s_registry;
  friend UseCommandRegistry;

  bool m_isValid;
  void *m_ptr;
  CommandFunc m_func;
};

#endif
