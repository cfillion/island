#ifndef ISLAND_RANGE_HPP
#define ISLAND_RANGE_HPP

class QDebug;
class QString;

class Range
{
public:
  enum Format {
    Default,
    Counter,
  };

  Range(const QString &);
  Range(const int min = 0, const int max = 0);

  bool isValid() const;
  bool hasNext() const;
  int min() const { return m_min; }
  int max() const { return m_max; }
  QString toString(const Format mode = Default) const;

  int next();
  void reset();
  void reset(const int min, const int max = 0);

  bool operator==(const Range &) const;
  bool operator!=(const Range &) const;

private:
  void init(const int min, const int max);
  int current() const;

  int m_min;
  int m_max;

  int m_currentOffset;
};

QDebug operator<<(QDebug, const Range &);

#endif
