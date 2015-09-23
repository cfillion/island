#ifndef ISLAND_MAPPING_HPP
#define ISLAND_MAPPING_HPP

#include <boost/optional.hpp>
#include <QMap>

#include "commands.hpp"
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

  const boost::optional<Command> &binding() const { return m_binding; }

  void set(const QString &sequence, const Command &command);
  MappingMatch match(const Island::Buffer &buf);

private:
  bool isLeaf() const { return m_children.isEmpty(); }

  Mapping *resolve(const Island::Buffer &buf, const bool create = true);
  Mapping *resolve(const QString &req, const bool create = true);

  void bindTo(const Command &command);

  QMap<QString, Mapping *> m_children;
  boost::optional<Command> m_binding;
};

#endif
