#include <catch.hpp>

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
  REQUIRE(map.empty());
  REQUIRE(map.size() == 0);
  REQUIRE(map.root()  == 0);
  REQUIRE(map.parent()  == 0);

  SECTION("unmapped") {
    const auto m = map.match("a");
    REQUIRE(m.ambiguous == false);
    REQUIRE(m.mapping == 0);
  }

  SECTION("perfect match") {
    map.set("a", Command(0));
    REQUIRE_FALSE(map.empty());
    REQUIRE(map.size() == 1);

    const auto m = map.match("a");
    REQUIRE(m.ambiguous == false);

    REQUIRE(m.mapping != 0);
    REQUIRE(m.mapping->root() == &map);
    REQUIRE(m.mapping->parent() == &map);

    REQUIRE(m.mapping->bindingType() == Mapping::CommandBinding);
    REQUIRE(m.mapping->isBound());
  }

  SECTION("incomplete match") {
    map.set("aa", Command(0));

    const auto m = map.match("a");
    REQUIRE(m.ambiguous == true);
    REQUIRE(m.mapping != 0);
    REQUIRE(m.mapping->bindingType() == Mapping::EmptyBinding);
    REQUIRE_FALSE(m.mapping->isBound());
  }

  SECTION("ambiguous match") {
    map.set("a", Command(0));
    map.set("aa", Command(0));

    const auto m = map.match("a");
    REQUIRE(m.ambiguous == true);
    REQUIRE(m.mapping != 0);
    REQUIRE(m.mapping->bindingType() == Mapping::CommandBinding);
    REQUIRE(m.mapping->isBound());
  }

  SECTION("sub match") {
    map.set("a", Command(0));
    map.set("aa", Command(0));

    const auto m_first = map.match("a");

    const auto m = m_first.mapping->match("a");
    REQUIRE(m.ambiguous == false);

    REQUIRE(m.mapping != 0);
    REQUIRE(m.mapping->root() == &map);
    REQUIRE(m.mapping->parent() == m_first.mapping);

    REQUIRE(m.mapping->bindingType() == Mapping::CommandBinding);
    REQUIRE(m.mapping->isBound());
  }
}
