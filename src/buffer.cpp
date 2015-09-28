#include "buffer.hpp"

#include <QRegularExpression>

void Buffer::importString(const QString &input)
{
  static const QRegularExpression InputPattern("(\\<[^\\>]+\\>|.)");
  auto matches = InputPattern.globalMatch(input);

  while(matches.hasNext()) {
    auto match = matches.next();
    m_list << match.captured();
  }
}

Buffer &Buffer::operator<<(const QString &str)
{
  m_list << str;
  return *this;
}
