#include "buffer.hpp"

#include <QRegularExpression>

void Buffer::importString(const QString &input)
{
  static const QRegularExpression InputPattern("(\\<[^\\>]+\\>|.)");
  auto matches = InputPattern.globalMatch(input);

  while(matches.hasNext()) {
    auto match = matches.next();
    push(match.captured());
  }
}

Buffer &Buffer::operator<<(const QString &str)
{
  push(str);
  return *this;
}

void Buffer::push(const QString &str)
{
  if(m_list.isEmpty() && str.size() == 1 && str[0].isNumber())
    m_counter += str;
  else
    m_list << str;
}

QString Buffer::toString() const
{
  return m_counter + m_list.join(QString());
}

int Buffer::counter() const
{
  if(m_counter.isEmpty())
    return -1;
  else
    return m_counter.toInt();
}

void Buffer::clear()
{
  m_counter.clear();
  m_list.clear();
}

void Buffer::truncate(const int i)
{
  m_list = m_list.mid(i);
}

Buffer Buffer::truncateCopy(const int i) const
{
  Buffer copy(*this);
  copy.truncate(i);
  return copy;
}
