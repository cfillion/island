#ifndef ISLAND_BUFFER_HPP
#define ISLAND_BUFFER_HPP

#include <QStringList>

class Buffer {
public:
  Buffer(const QList<QString> &list = QStringList()) : m_list(list) {}
  Buffer(const QString &in) { importString(in); }
  Buffer(const char *in) { importString(in); }

  int counter() const;
  QString toString() const;
  void truncate(const int i);
  Buffer truncateCopy(const int i) const;

  int size() const { return m_list.size(); }
  void clear();
  void push(const QString &);

  const QString &operator[](const int i) const { return m_list[i]; }
  Buffer &operator<<(const QString &str);
  bool operator==(const Buffer &o) const { return m_list == o.m_list; }
  bool operator!=(const Buffer &o) const { return !(*this == o); }

private:
  void importString(const QString &input);

  QString m_counter;
  QStringList m_list;
};

#endif
