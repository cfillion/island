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

void Mapping::set(const Buffer &sequence, const Command &command)
{
  Mapping *map = resolve(sequence, true);
  map->bindTo(command);
}

void Mapping::bindTo(const Command &cmd)
{
  m_command = cmd;
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

  if(node->command())
    match.mapping = node;

  match.ambiguous = !node->isLeaf();

  return match;
}

const Command *Mapping::command() const
{
  if(m_command == boost::none)
    return 0;

  const Command &cmd = *m_command;
  return &cmd;
}
