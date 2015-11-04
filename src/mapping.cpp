#include "mapping.hpp"

using namespace Island;

#include <QDebug>

QDebug operator<<(QDebug debug, const MappingMatch &m)
{
  QDebugStateSaver saver(debug);
  debug.nospace()
    << "MappingMatch("
    << "ambiguous: " << m.ambiguous
    << ", mapping: " << m.mapping
    << ')';

  return debug;
}

Mapping::Mapping(Mapping *parent)
  : m_parent(parent), m_root(parent)
{
  if(m_parent && m_parent->root())
    m_root = m_parent->root();
}

Mapping::~Mapping()
{
  qDeleteAll(m_children);
}

void Mapping::set(const Buffer &buf, const Buffer &binding)
{
  if(buf.empty())
    return;

  Mapping *map = resolve(buf);
  map->bindTo(binding);
}

void Mapping::set(const Buffer &buf, const Command &command)
{
  if(buf.empty())
    return;

  Mapping *map = resolve(buf);
  map->bindTo(command);
}

Mapping *Mapping::resolve(const Buffer &buf)
{
  Mapping *node = resolve(buf[0]);

  if(buf.size() > 1 && node)
    return node->resolve(buf.truncateCopy(1));

  return node;
}

Mapping *Mapping::resolve(const QString &key)
{
  if(Mapping *node = m_children.value(key))
    return node;

  return m_children[key] = new Mapping(this);
}

MappingMatch Mapping::match(const QString &key) const
{
  const Mapping *node = m_children.value(key);
  return {node, node && !node->empty()};
}

const Buffer *Mapping::boundBuffer() const
{
  if(bindingType() != BufferBinding)
    return 0;

  const Buffer &buf = boost::get<Buffer>(m_bindings.top());
  return &buf;
}

const Command *Mapping::boundCommand() const
{
  if(bindingType() != CommandBinding)
    return 0;

  const Command &cmd = boost::get<Command>(m_bindings.top());
  return &cmd;
}

Mapping::BindingType Mapping::bindingType() const
{
  if(m_bindings.empty())
    return EmptyBinding;

  return m_bindings.top().which() == 0 ? BufferBinding : CommandBinding;
}
