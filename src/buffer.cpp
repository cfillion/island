#include "buffer.hpp"

#include <QRegularExpression>

void Buffer::importString(const QString &input)
{
  static const QRegularExpression pattern("(\\<[^\\>]+\\>|.)");
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

  if(empty() && str[0].isNumber()) {
    if(m_counter.size() > 0 || str[0] != '0')
      m_counter += str;
  }
  else
    m_list.push_back(str);
}

QString Buffer::toString() const
{
  QString string = m_counter;

  for(const QString &seq : m_list)
    string += seq;

  return string;
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
  resetCounter();
  m_list.clear();
}

void Buffer::resetCounter()
{
  m_counter.clear();
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
