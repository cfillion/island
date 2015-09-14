#include "mapping.hpp"

Mapping::Mapping()
{
}

void Mapping::nmap(const QString &input, const QString &command)
{
  m_map[input] = command;
}
