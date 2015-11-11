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

  SECTION("sorting") {
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

  SECTION("sorting") {
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
  CHECK(r.hasNext());

  REQUIRE(r.next() == 11);
  CHECK(r.hasNext());

  REQUIRE(r.next() == 12);
  CHECK(r.hasNext());

  REQUIRE(r.next() == 13);
  CHECK_FALSE(r.hasNext());

  REQUIRE(r.next() == 0);

  r.rewind();
  REQUIRE(r.hasNext());
}

TEST_CASE("iterate over single component with 1 as value", M) {
  Range r(1);
  CHECK(r.hasNext());
  REQUIRE(r.next() == 1);
  CHECK_FALSE(r.hasNext());
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

TEST_CASE("relative range is not sorted") {
  SECTION("left") {
    const Range r({42, RangeComponent::Relative}, 2);
    REQUIRE(r.min() == 42);
    REQUIRE(r.max() == 2);
  }

  SECTION("right") {
    const Range r(2, {42, RangeComponent::Relative});
    REQUIRE(r.min() == 2);
    REQUIRE(r.max() == 42);
  }
}

TEST_CASE("resolve null range") {
  Range r;
  REQUIRE(r.min() == 0);
  REQUIRE(r.max() == 0);

  r.resolve(42);
  REQUIRE(r.min() == 42);
  REQUIRE(r.max() == 42);
}

TEST_CASE("relative minimum") {
  Range r({2, RangeComponent::Relative}, 4);
  CHECK_FALSE(r.isValid());

  r.resolve(1);

  CHECK(r.isValid());
  REQUIRE(r.min() == 3);
}

TEST_CASE("relative maximum") {
  Range r(1, {2, RangeComponent::Relative});
  CHECK_FALSE(r.isValid());

  r.resolve();

  CHECK(r.isValid());
  REQUIRE(r.max() == 3);
}

TEST_CASE("sort on resolution") {
  Range r(4, {-2, RangeComponent::Relative});
  CHECK_FALSE(r.isValid());
  CHECK(r.min() == 4);
  CHECK(r.max() == -2);

  r.resolve();

  REQUIRE(r.min() == 2);
  REQUIRE(r.max() == 4);
}

TEST_CASE("relative zero") {
  Range r(4, {0, RangeComponent::Relative});
  CHECK(r.isValid());

  REQUIRE(r.min() == 4);
  REQUIRE(r.max() == 4);
}
