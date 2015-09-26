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

void Mapping::set(const QString &sequence, const Command &command)
{
  const Buffer sequenceBuf = InputToBuffer(sequence);
  // const Buffer commandBuf = InputToBuffer(command);

  Mapping *seqMapping = resolve(sequenceBuf, true);
  seqMapping->bindTo(command);
  Q_UNUSED(seqMapping);
}

void Mapping::bindTo(const Command &cmd)
{
  m_binding = cmd;
}

Mapping *Mapping::resolve(const Island::Buffer &buf, const bool create)
{
  const int bufSize = buf.size();
  const QString seq = buf[0];

  Mapping *node = resolve(seq, create);

  if(bufSize > 1 && node)
    return node->resolve(buf.mid(1), create);

  return node;
}

Mapping *Mapping::resolve(const QString &seq, const bool create)
{
  Mapping *node = m_children.value(seq);

  if(!node && create)
    node = m_children[seq] = new Mapping;
  
  return node;
}

MappingMatch Mapping::match(const Island::Buffer &buf)
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

  if(node->m_binding)
    match.mapping = node;

  match.ambiguous = !node->isLeaf();

  return match;
}
