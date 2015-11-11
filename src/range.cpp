#include "range.hpp"

#include <algorithm>
#include <QDebug>
#include <QRegularExpression>

QDebug operator<<(QDebug debug, const Range &range)
{
  QDebugStateSaver saver(debug);
  debug.nospace().noquote() << "Range(" << range.toString() << ')';

  return debug;
}

Range::Range(const int min, const int max)
{
  reset(min, max);
}

Range::Range(const QString &input)
{
  static const QRegularExpression regex("\\A(\\d+)(?:,(\\d+))?\\z");
  const auto match = regex.match(input);

  const int min = match.captured(1).toInt();
  const int max = match.captured(2).toInt();

  reset(min, max);
}

void Range::reset()
{
  m_currentOffset = -1;
}

void Range::reset(const int min, const int max)
{
  m_min = std::min(min, max);
  m_max = std::max(min, max);

  if(m_min == 0)
    m_min = m_max;

  reset();
}

int Range::current() const
{
  return m_currentOffset + m_min;
}

bool Range::isValid() const
{
  return m_min > 0 && m_max > 0;
}

bool Range::hasNext() const
{
  return isValid() && current() < m_max;
}

int Range::next()
{
  m_currentOffset++;
  return current();
}

QString Range::toString(const Format mode) const
{
  if(m_min == m_max) {
    if(isValid())
      return QString::number(m_max);
  }
  else if(mode == Default)
    return QString("%1,%2").arg(m_min).arg(m_max);

  return QString();
}

bool Range::operator==(const Range &o) const
{
  return m_min == o.min() && m_max == o.max();
}

bool Range::operator!=(const Range &o) const
{
  return !(*this == o);
}
