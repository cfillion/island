#include "buffer.hpp"

#include <QDebug>
#include <QRegularExpression>

#include "range.hpp"

QDebug operator<<(QDebug debug, const Buffer &buf)
{
  QDebugStateSaver saver(debug);
  debug.nospace()
    << "Buffer("
    << buf.range();

  for(const QString &seq : buf)
    debug << ", " << seq;

  debug << ')';

  return debug;
}

void Buffer::importString(const QString &input)
{
  static const QRegularExpression pattern("(\\<[^\\>]+\\>|%[a-zA-Z%]|.)");
  auto matches = pattern.globalMatch(input);

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
  if(str.isEmpty())
    return;

  if(empty() && (str[0].isNumber() || str[0] == ',')) {
    if(m_range.size() > 0 || str[0] != '0')
      m_range += str;
  }
  else
    m_list.push_back(str);
}

QString Buffer::toString() const
{
  QString string = m_range;

  for(const QString &seq : m_list)
    string += seq;

  return string;
}

Range Buffer::range() const
{
  return Range(m_range);
}

void Buffer::clear()
{
  resetRange();
  m_list.clear();
}

void Buffer::resetRange()
{
  m_range.clear();
}

void Buffer::truncate(const int n)
{
  if(size() == n)
    return clear();

  m_list.erase(m_list.begin(), m_list.begin() + n);
}

Buffer Buffer::truncateCopy(const int n) const
{
  Buffer copy(*this);
  copy.truncate(n);
  return copy;
}
