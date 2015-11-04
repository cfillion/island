#ifndef ISLAND_MAPPING_HPP
#define ISLAND_MAPPING_HPP

#include <boost/variant.hpp>
#include <QMap>
#include <stack>

#include "buffer.hpp"
#include "command.hpp"
#include "global.hpp"

class Mapping;

typedef std::array<Mapping *, Island::CommandMode+1> MappingArray;

struct MappingMatch {
  const Mapping *mapping;
  bool ambiguous;
};

QDebug operator<<(QDebug, const MappingMatch &);

typedef boost::variant<Buffer, Command> Binding;

class Mapping {
public:
  enum BindingType {
    EmptyBinding,
    CommandBinding,
    BufferBinding,
  };

  Mapping(Mapping *parent = 0);
  ~Mapping();

  const Buffer *boundBuffer() const;
  const Command *boundCommand() const;

  Mapping *parent() const { return m_parent; }
  Mapping *root() const { return m_root; }

  BindingType bindingType() const;
  bool isBound() const { return bindingType() != EmptyBinding; }
  bool empty() const { return m_children.isEmpty(); }
  int size() const { return m_children.size(); }

  void set(const Buffer &buf, const Buffer &binding);
  void set(const Buffer &buf, const Command &command);
  MappingMatch match(const QString &key) const;

  template <typename T>
  void bindTo(const T &binding) { m_bindings.push(binding); }

private:
  Mapping *resolve(const Buffer &buf);
  Mapping *resolve(const QString &key);

  Mapping *m_parent;
  Mapping *m_root;

  QMap<QString, Mapping *> m_children;
  std::stack<Binding> m_bindings;
};

#endif
