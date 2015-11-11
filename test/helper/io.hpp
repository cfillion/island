#ifndef ISLAND_TEST_HELPER_IO_HPP
#define ISLAND_TEST_HELPER_IO_HPP

#include <ostream>

class Buffer;
class KeyPress;
class QString;
class Range;

std::ostream &operator<<(std::ostream &, const Buffer &);
std::ostream &operator<<(std::ostream &, const KeyPress &);
std::ostream &operator<<(std::ostream &, const QString &);
std::ostream &operator<<(std::ostream &, const Range &);

#endif
