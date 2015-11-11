#ifndef ISLAND_RANGE_HPP
#define ISLAND_RANGE_HPP

class QDebug;
class QString;

class RangeComponent
{
public:
  enum Type {
    Absolute,
    Relative,
  };

  RangeComponent(const int v = 0, const Type t = Absolute)
    : m_value(v), m_type(t) {}
  RangeComponent(const QString &);

  int value() const { return m_value; }
  Type type() const { return m_type; }

  bool isNull() const;
  bool isValid() const;
  QString toString() const;

  void resolve(const int baseValue, const int wrapBound);

  bool operator==(const RangeComponent &) const;
  bool operator!=(const RangeComponent &) const;
  bool operator<(const RangeComponent &) const;

private:
  int m_value;
  Type m_type;
};

class Range
{
public:
  enum Format {
    Default,
    Counter,
  };

  Range(const RangeComponent &min = RangeComponent(),
    const RangeComponent &max = RangeComponent());
  Range(const QString &);

  bool isValid() const;
  bool hasNext() const;
  QString toString(const Format mode = Default) const;

  int current() const;
  int next();
  void resolve(const int minimumBaseValue = 1, const int wrapBound = 1);
  void rewind();

  const RangeComponent &min() const { return m_min; }
  const RangeComponent &max() const { return m_max; }

  bool operator==(const Range &) const;
  bool operator!=(const Range &) const;

private:
  void sort();

  RangeComponent m_min;
  RangeComponent m_max;

  int m_currentOffset;
};

QDebug operator<<(QDebug, const Range &);
QDebug operator<<(QDebug, const RangeComponent &);

#endif
