#ifndef ISLAND_MAPPING_HPP
#define ISLAND_MAPPING_HPP

#include <map>
#include <QString>

class Window;

class Mapping {
public:
  Mapping();
  void nmap(const QString &input, const QString &command);

private:
  std::map<QString, QString> m_map;
};

#endif
