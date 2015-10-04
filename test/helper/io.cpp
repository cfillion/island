#include "io.hpp"

std::ostream &operator<<(std::ostream &os, const QString &str)
{
  os << QString("\"%1\"").arg(str).toStdString();
  return os;
}
