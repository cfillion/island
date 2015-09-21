#ifndef ISLAND_MAPPING_HPP
#define ISLAND_MAPPING_HPP

#include <functional>
#include <QMap>

#include "global.hpp"

class Window;
class Mapping;

typedef std::function<void(Window *, const int)> NativeCode;
typedef std::array<Mapping *, Island::ModeCount> MappingArray;

struct MappingMatch {
  int index = -1;
  bool ambiguous = false;
  Mapping *mapping = 0;
};

QDebug operator<<(QDebug debug, const MappingMatch &m);

class Mapping {
public:
  Mapping() : m_isCommand(false) {}
  ~Mapping();

  void set(const QString &input, const QString &command);

  MappingMatch match(const Island::Buffer &buf);

private:
  Mapping *resolve(const Island::Buffer &buf, const bool create = false);
  Mapping *resolve(const QString &req, const bool create = false);

  void setCommand(const bool isCmd) { m_isCommand = isCmd; }
  bool hasChildren() { return !m_children.isEmpty(); }

  QMap<QString, Mapping *> m_children;
  bool m_isCommand;
};

#endif
