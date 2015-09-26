#ifndef ISLAND_COMMAND_HPP
#define ISLAND_COMMAND_HPP

#include <QString>

#define ISLAND_COMMAND(x) CommandResult x(const Command &);

class Command;
class Window;

struct CommandResult {
  bool ok = true;
  QString message = QString();
};

typedef CommandResult (*CommandCallback)(const Command &);

class Command {
public:
  Command(const CommandCallback &cmd);
  Command(const QString &cmd);

  CommandCallback callback() const { return m_func; }
  template<class T> T ptr() const { return static_cast<T>(m_ptr); }

  CommandResult exec(void *ptr, const int repeat = 1);

private:
  void *m_ptr;
  CommandCallback m_func;
};

#endif
