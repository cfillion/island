#include "io.hpp"

#include <ostream>

std::ostream &operator<<(std::ostream &os, const QString &str)
{
  os << QString("\"%1\"").arg(str).toStdString();
  return os;
}
