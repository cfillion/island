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

  void setData(void *ptr) { m_data = ptr; }
  template<class T> T data() const { return static_cast<T>(m_data); }

  CommandResult exec() const;

private:
  static CommandRegistry *s_registry;
  friend UseCommandRegistry;

  bool m_isValid;
  void *m_data;
  CommandFunc m_func;
};

#endif
