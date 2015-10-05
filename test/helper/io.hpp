#ifndef ISLAND_TEST_HELPER_IO_HPP
#define ISLAND_TEST_HELPER_IO_HPP

#include <ostream>

class QString;
class KeyPress;

std::ostream &operator<<(std::ostream &, const QString &);
std::ostream &operator<<(std::ostream &, const KeyPress &);

#endif
