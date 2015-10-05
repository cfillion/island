#ifndef ISLAND_MAPPING_HPP
#define ISLAND_MAPPING_HPP

#include <boost/variant.hpp>
#include <QMap>
#include <stack>

#include "buffer.hpp"
#include "command.hpp"
#include "global.hpp"

class Mapping;

typedef std::array<Mapping *, Island::ModeCount> MappingArray;

struct MappingMatch {
  int index = -1;
  bool ambiguous = false;
  Mapping *mapping = 0;
};

QDebug operator<<(QDebug, const MappingMatch &);

typedef boost::variant<Buffer, Command> Binding;

class Mapping {
public:
  enum Type {
    Stem,
    Native,
    User,
  };

  ~Mapping();

  const Buffer *boundBuffer() const;
  const Command *boundCommand() const;

  Type type() const;
  bool isLeaf() const { return m_children.isEmpty(); }
  void set(const Buffer &sequence, const Buffer &binding);
  void set(const Buffer &sequence, const Command &command);
  MappingMatch match(const Buffer &buf);

  template <typename T>
  void bindTo(const T &binding) { m_bindings.push(binding); }

private:
  Mapping *resolve(const Buffer &buf, const bool create = true);
  Mapping *resolve(const QString &req, const bool create = true);

  QMap<QString, Mapping *> m_children;
  std::stack<Binding> m_bindings;
};

#endif
