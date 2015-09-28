#ifndef ISLAND_MAPPING_HPP
#define ISLAND_MAPPING_HPP

#include <boost/optional.hpp>
#include <QMap>

#include "command.hpp"
#include "global.hpp"

class Mapping;

typedef std::array<Mapping *, Island::ModeCount> MappingArray;

struct MappingMatch {
  int index = -1;
  bool ambiguous = false;
  Mapping *mapping = 0;
};

QDebug operator<<(QDebug debug, const MappingMatch &m);

class Mapping {
public:
  ~Mapping();

  const Command *command() const;

  bool isLeaf() const { return m_children.isEmpty(); }
  void bindTo(const Command &command);
  void set(const QString &sequence, const Command &command);
  MappingMatch match(const Island::Buffer &buf);

private:
  Mapping *resolve(const Island::Buffer &buf, const bool create = true);
  Mapping *resolve(const QString &req, const bool create = true);

  QMap<QString, Mapping *> m_children;
  boost::optional<Command> m_command;
};

#endif
