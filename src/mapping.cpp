#include "mapping.hpp"

using namespace Island;

#include <QDebug>

QDebug operator<<(QDebug debug, const MappingMatch &m)
{
  QDebugStateSaver saver(debug);
  debug.nospace()
    << "MappingMatch("
    << "index: " << m.index
    << ", ambiguous: " << m.ambiguous
    << ", mapping: " << m.mapping
    << ')';

  return debug;
}

Mapping::~Mapping()
{
  qDeleteAll(m_children);
}

void Mapping::set(const Buffer &sequence, const Buffer &binding)
{
  Mapping *map = resolve(sequence, true);
  map->bindTo(binding);
}

void Mapping::set(const Buffer &sequence, const Command &command)
{
  Mapping *map = resolve(sequence, true);
  map->bindTo(command);
}

Mapping *Mapping::resolve(const Buffer &buf, const bool create)
{
  const int bufSize = buf.size();
  const QString seq = buf[0];

  Mapping *node = resolve(seq, create);

  if(bufSize > 1 && node)
    return node->resolve(buf.truncateCopy(1), create);

  return node;
}

Mapping *Mapping::resolve(const QString &seq, const bool create)
{
  Mapping *node = m_children.value(seq);

  if(!node && create)
    node = m_children[seq] = new Mapping;
 
  return node;
}

MappingMatch Mapping::match(const Buffer &buf)
{
  MappingMatch match;

  Mapping *node = this;

  const int bufSize = buf.size();
  for(int i = 0; i < bufSize; i++) {
    const QString key = buf[i];
    node = node->resolve(key, false);

    match.index = i;

    if(!node)
      return match;
  }

  if(node->type() != Stem)
    match.mapping = node;

  match.ambiguous = !node->isLeaf();

  return match;
}

const Buffer *Mapping::boundBuffer() const
{
  if(type() != User)
    return 0;

  const Buffer &buf = boost::get<Buffer>(m_bindings.top());
  return &buf;
}

const Command *Mapping::boundCommand() const
{
  if(type() != Native)
    return 0;

  const Command &cmd = boost::get<Command>(m_bindings.top());
  return &cmd;
}

Mapping::Type Mapping::type() const
{
  if(m_bindings.empty())
    return Stem;

  return m_bindings.top().which() == 0 ? User : Native;
}
