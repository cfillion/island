#include "vendor/catch.hpp"

#include <boost/none.hpp>
#include <mapping.hpp>

static const char *M = "[mapping]";

using namespace Island;

TEST_CASE("bind to command", M) {
  Mapping map;
  REQUIRE(map.command() == 0);

  map.bindTo(Command(0));
  REQUIRE_FALSE(map.command() == 0);
}

TEST_CASE("match buffer", M) {
  Mapping map;
  REQUIRE(map.isLeaf());

  SECTION("unmapped") {
    const auto m = map.match(InputToBuffer("a"));
    REQUIRE(m.index == 0);
    REQUIRE(m.ambiguous == false);
    REQUIRE(m.mapping == 0);
  }

  map.set("aa", 0);
  REQUIRE_FALSE(map.isLeaf());

  SECTION("ambiguous match") {
    const auto m = map.match(InputToBuffer("a"));
    REQUIRE(m.index == 0);
    REQUIRE(m.ambiguous == true);
    REQUIRE(m.mapping == 0);
  }

  SECTION("perfect match") {
    const auto m = map.match(InputToBuffer("aa"));
    REQUIRE(m.index == 1);
    REQUIRE(m.ambiguous == false);
    REQUIRE_FALSE(m.mapping == 0);
    REQUIRE(m.mapping->command());
  }
}
