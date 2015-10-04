#ifndef ISLAND_TEST_HELPER_IO_HPP
#define ISLAND_TEST_HELPER_IO_HPP

#include <ostream>
#include <QString>
#include <utility>

std::ostream &operator<<(std::ostream &, const QString &);

template <typename A, typename B>
std::ostream &operator<<(std::ostream &os, const std::pair<A,B> &p)
{
  os << std::get<A>(p) << ", " << std::get<B>(p);
  return os;
}

#endif
