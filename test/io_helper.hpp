#include <QString>

std::ostream &operator<<(std::ostream &os, const QString &str)
{
  os << str.toStdString();
  return os;
}
