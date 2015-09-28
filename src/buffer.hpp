#ifndef ISLAND_BUFFER_HPP
#define ISLAND_BUFFER_HPP

#include <QStringList>

class Buffer {
public:
  Buffer(const QList<QString> &list = QStringList()) : m_list(list) {}
  Buffer(const QString &in) { importString(in); }
  Buffer(const char *in) { importString(in); }

  int size() const { return m_list.size(); }
  Buffer mid(const int i) const { return m_list.mid(i); }
  QString toString() const { return m_list.join(QChar()); }
  void clear() { m_list.clear(); }

  const QString &operator[](const int i) const { return m_list[i]; }
  Buffer &operator<<(const QString &str);
  bool operator==(const Buffer &o) const { return m_list == o.m_list; }
  bool operator!=(const Buffer &o) const { return !(*this == o); }

private:
  void importString(const QString &input);

  QStringList m_list;
};

#endif
