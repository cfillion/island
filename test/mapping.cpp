#include "vendor/catch.hpp"

#include <boost/none.hpp>
#include <mapping.hpp>

static const char *M = "[mapping]";

using namespace Island;

TEST_CASE("bound to command", M) {
  Mapping map;
  CHECK(map.bindingType() == Mapping::EmptyBinding);
  CHECK(map.boundBuffer() == 0);
  CHECK(map.boundCommand() == 0);

  map.bindTo(Command(0));

  CHECK(map.bindingType() == Mapping::CommandBinding);
  REQUIRE(map.boundBuffer() == 0);
  REQUIRE_FALSE(map.boundCommand() == 0);
}

TEST_CASE("bound to buffer", M) {
  Mapping map;
  map.bindTo("hello");

  CHECK(map.bindingType() == Mapping::BufferBinding);
  REQUIRE_FALSE(map.boundBuffer() == 0);
  REQUIRE(map.boundCommand() == 0);
}

TEST_CASE("match buffer", M) {
  Mapping map;
  REQUIRE(map.isLeaf());

  SECTION("unmapped") {
    const auto m = map.match("a");
    REQUIRE(m.ambiguous == false);
    REQUIRE(m.mapping == 0);
  }

  SECTION("incomplete match") {
    map.set("aa", Command(0));

    const auto m = map.match("a");
    REQUIRE(m.ambiguous == true);
    REQUIRE(m.mapping != 0);
    REQUIRE(m.mapping != &map);
    REQUIRE(m.mapping->bindingType() == Mapping::EmptyBinding);
    REQUIRE_FALSE(m.mapping->isBound());
  }

  SECTION("ambiguous match") {
    map.set("a", Command(0));
    map.set("aa", Command(0));

    const auto m = map.match("a");
    REQUIRE(m.ambiguous == true);
    REQUIRE(m.mapping != 0);
    REQUIRE(m.mapping != &map);
    REQUIRE(m.mapping->bindingType() == Mapping::CommandBinding);
    REQUIRE(m.mapping->isBound());
  }

  SECTION("perfect match") {
    map.set("a", Command(0));
    REQUIRE_FALSE(map.isLeaf());

    const auto m = map.match("a");
    REQUIRE(m.ambiguous == false);
    REQUIRE(m.mapping != 0);
    REQUIRE(m.mapping != &map);
    REQUIRE(m.mapping->bindingType() == Mapping::CommandBinding);
    REQUIRE(m.mapping->isBound());
  }
}
