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

QDebug operator<<(QDebug debug, const RangeComponent &c)
{
  QDebugStateSaver saver(debug);
  debug.nospace().noquote() << '(' << c.value() << '\x20';

  switch(c.type()) {
  case RangeComponent::Absolute:
    debug << "abs";
    break;
  case RangeComponent::Relative:
    debug << "rel";
    break;
  };

  debug << ')';

  return debug;
}

RangeComponent::RangeComponent(const QString &input)
{
  m_value = input.toInt();
  m_type = input.isEmpty() || input[0].isNumber() ? Absolute : Relative;
}

bool RangeComponent::isNull() const
{
  return m_value == 0;
}

bool RangeComponent::isValid() const
{
  return m_type == Absolute && !isNull();
}

void RangeComponent::resolve(const int baseValue, const int wrapBound)
{
  switch(m_type) {
  case Absolute:
    if(isNull())
      m_value = baseValue;
    break;
  case Relative:
    m_value += baseValue;

    while(m_value < 1)
      m_value += std::max(1, wrapBound);

    m_type = Absolute;
    break;
  }
}

QString RangeComponent::toString() const
{
  if(m_value == 0)
    return QString();

  QString str = QString::number(m_value);

  if(m_type == Relative && m_value >= 0)
    str.prepend("+");

  return str;
}

bool RangeComponent::operator==(const RangeComponent &o) const
{
  return m_value == o.value() && m_type == o.type();
}

bool RangeComponent::operator!=(const RangeComponent &o) const
{
  return !(*this == o);
}

bool RangeComponent::operator<(const RangeComponent &o) const
{
  return m_value < o.value();
}

Range::Range(const RangeComponent &min, const RangeComponent &max)
  : m_min(min), m_max(max)
{
  sort();
  rewind();
}

Range::Range(const QString &input)
{
  static const QRegularExpression regex("\\A([\\+-]?\\d+)(?:,([\\+-]?\\d+))?\\z");
  const auto match = regex.match(input);

  m_min = RangeComponent(match.captured(1));
  m_max = RangeComponent(match.captured(2));

  sort();
  rewind();
}

void Range::sort()
{
  if(!isValid())
    return;

  const auto min = m_min;
  const auto max = m_max;

  m_min = std::min(min, max);
  m_max = std::max(min, max);
}

bool Range::isValid() const
{
  return m_min.isValid() && m_max.isValid();
}

void Range::resolve(const int minimumBaseValue, const int wrapBound)
{
  m_min.resolve(minimumBaseValue, wrapBound);
  m_max.resolve(m_min.value(), wrapBound);

  sort();
}

void Range::rewind()
{
  m_currentOffset = -1;
}

int Range::current() const
{
  if(!isValid())
    return 0;

  return m_min.value() + m_currentOffset;
}

bool Range::hasNext() const
{
  const int cv = current();
  return cv == 0 || cv < m_max.value();
}

int Range::next()
{
  if(!hasNext())
    return 0;

  m_currentOffset++;
  return current();
}

QString Range::toString(const Format mode) const
{
  if(m_max.isNull() || m_min == m_max)
    return m_min.toString();
  else if(mode == Default)
    return QString("%1,%2").arg(m_min.toString(), m_max.toString());

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
