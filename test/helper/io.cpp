#include "io.hpp"

#include <QDebug>

#include <buffer.hpp>
#include <keypress.hpp>

template <typename T>
static void debugPrint(std::ostream &os, const T &val)
{
  QString str;

  QDebug db(&str);
  db << val;

  os << str.toStdString();
}

std::ostream &operator<<(std::ostream &os, const Buffer &buf)
{
  debugPrint(os, buf);
  return os;
}

std::ostream &operator<<(std::ostream &os, const QString &str)
{
  os << QString("\"%1\"").arg(str).toStdString();
  return os;
}

std::ostream &operator<<(std::ostream &os, const KeyPress &kp)
{
  debugPrint(os, kp);
  return os;
}
