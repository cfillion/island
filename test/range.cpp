#include "vendor/catch.hpp"

#include <range.hpp>

#include <QString>

static const char *M = "[range]";

TEST_CASE("integer constructor", M) {
  SECTION("valid") {
    const Range r(2, 4);
    REQUIRE(r.min() == 2);
    REQUIRE(r.max() == 4);
    REQUIRE(r.isValid());
  }

  SECTION("single") {
    const Range r(42);
    REQUIRE(r.min() == 42);
    REQUIRE(r.max() == 42);
    REQUIRE(r.isValid());
  }

  SECTION("inverted") {
    const Range r(4, 2);
    REQUIRE(r.min() == 2);
    REQUIRE(r.max() == 4);
    REQUIRE(r.isValid());
  }

  SECTION("invalid") {
    const Range r;
    REQUIRE(r.min() == 0);
    REQUIRE(r.max() == 0);
    REQUIRE_FALSE(r.isValid());
  }
}

TEST_CASE("string constructor", M) {
  SECTION("valid") {
    const Range r("2,4");
    REQUIRE(r.min() == 2);
    REQUIRE(r.max() == 4);
    REQUIRE(r.isValid());
  }

  SECTION("single") {
    const Range r("42");
    REQUIRE(r.min() == 42);
    REQUIRE(r.max() == 42);
    REQUIRE(r.isValid());
  }

  SECTION("inverted") {
    const Range r("4,2");
    REQUIRE(r.min() == 2);
    REQUIRE(r.max() == 4);
    REQUIRE(r.isValid());
  }

  SECTION("invalid") {
    const Range r("hello");
    REQUIRE(r.min() == 0);
    REQUIRE(r.max() == 0);
    REQUIRE_FALSE(r.isValid());
  }
}

TEST_CASE("iterating", M) {
  Range r(11, 13);
  REQUIRE(r.hasNext());

  REQUIRE(r.next() == 11);
  REQUIRE(r.hasNext());

  REQUIRE(r.next() == 12);
  REQUIRE(r.hasNext());

  REQUIRE(r.next() == 13);
  REQUIRE_FALSE(r.hasNext());

  r.reset();
  REQUIRE(r.hasNext());
}

TEST_CASE("string formatting") {
  SECTION("default") {
    REQUIRE(Range(2, 4).toString() == "2,4");
    REQUIRE(Range(2).toString() == "2");
    REQUIRE(Range().toString() == "");
  }

  SECTION("counter") {
    REQUIRE(Range(2, 4).toString(Range::Counter) == "");
    REQUIRE(Range(42).toString(Range::Counter) == "42");
  }
}
