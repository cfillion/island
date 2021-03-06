#ifndef ISLAND_BUFFER_HPP
#define ISLAND_BUFFER_HPP

#include <QString>
#include <vector>

class Range;

class Buffer {
public:
  Buffer() {}
  Buffer(const QString &in) { importString(in); }
  Buffer(const char *in) { importString(in); }

  Range range() const;
  QString toString() const;
  void truncate(const int n);
  Buffer truncateCopy(const int n) const;

  bool empty() const { return m_list.empty(); }
  int size() const { return m_list.size(); }
  void clear();
  void resetRange();
  void push(const QString &);

  const QString &operator[](const int i) const { return m_list[i]; }
  Buffer &operator<<(const QString &str);
  bool operator==(const Buffer &o) const { return m_list == o.m_list; }
  bool operator!=(const Buffer &o) const { return !(*this == o); }

  std::vector<QString>::const_iterator begin() const { return m_list.cbegin(); }
  std::vector<QString>::const_iterator end() const { return m_list.cend(); }

private:
  static bool isRangeCharacter(const QChar &);
  void importString(const QString &input);

  QString m_range;
  std::vector<QString> m_list;
};

QDebug operator<<(QDebug, const Buffer &);

#endif
