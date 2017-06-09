#include <catch.hpp>
#include "helper/io.hpp"

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
    REQUIRE(r.max().isNull());
  }

  SECTION("sorting") {
    const Range r(4, 2);
    REQUIRE(r.min() == 2);
    REQUIRE(r.max() == 4);
    REQUIRE(r.isValid());
  }

  SECTION("invalid") {
    const Range r;
    REQUIRE(r.min().isNull());
    REQUIRE(r.max().isNull());
    REQUIRE_FALSE(r.isValid());
  }
}

TEST_CASE("string constructor", M) {
  SECTION("absolute") {
    const Range r("2,4");
    CHECK(r.min() == 2);
    CHECK(r.max() == 4);
    REQUIRE(r.isValid());
  }

  SECTION("relative") {
    const Range r("-2,+1");
    CHECK(r.min() == RangeComponent(-2, RangeComponent::Relative));
    CHECK(r.max() == RangeComponent(1, RangeComponent::Relative));
    REQUIRE_FALSE(r.isValid());
  }

  SECTION("single") {
    const Range r("42");
    CHECK(r.min() == 42);
    CHECK(r.max().isNull());
  }

  SECTION("sorting") {
    const Range r("4,2");
    CHECK(r.min() == 2);
    CHECK(r.max() == 4);
    REQUIRE(r.isValid());
  }

  SECTION("null minimum") {
    const Range r(",42");
    CHECK(r.min().isNull());
    CHECK(r.max() == 42);
  }

  SECTION("null maximum") {
    const Range r("42,");
    CHECK(r.min() == 42);
    CHECK(r.max().isNull());
  }

  SECTION("invalid") {
    const Range r("hello");
    CHECK(r.min().value() == 0);
    CHECK(r.max().value() == 0);
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
  r.resolve();

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

  SECTION("relative") {
    const RangeComponent::Type rel = RangeComponent::Relative;
    REQUIRE(Range(RangeComponent(2, rel)).toString() == "+2");
    REQUIRE(Range(RangeComponent(-2, rel)).toString() == "-2");
  }
}

TEST_CASE("relative range is not sorted") {
  SECTION("left") {
    const Range r({42, RangeComponent::Relative}, 2);
    REQUIRE(r.min().value() == 42);
    REQUIRE(r.max().value() == 2);
  }

  SECTION("right") {
    const Range r(2, {42, RangeComponent::Relative});
    REQUIRE(r.min().value() == 2);
    REQUIRE(r.max().value() == 42);
  }
}

TEST_CASE("resolve null range") {
  Range r;
  REQUIRE(r.min() == 0);
  REQUIRE(r.max() == 0);
  REQUIRE(r.isNull());

  r.resolve(42);
  REQUIRE(r.min() == 42);
  REQUIRE(r.max() == 42);
  REQUIRE_FALSE(r.isNull());
}

TEST_CASE("relative minimum") {
  Range r({2, RangeComponent::Relative}, 4);
  CHECK_FALSE(r.isValid());

  r.resolve(1);

  CHECK(r.isValid());
  REQUIRE(r.min().value() == 3);
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
  CHECK(r.min().value() == 4);
  CHECK(r.max().value() == -2);

  r.resolve();

  REQUIRE(r.min() == 2);
  REQUIRE(r.max() == 4);
}

TEST_CASE("relative zero") {
  Range r(4, {0, RangeComponent::Relative});
  r.resolve();

  CHECK(r.isValid());
  REQUIRE(r.min() == 4);
  REQUIRE(r.max() == 4);
}

TEST_CASE("relative wrapping") {
  Range r({-1, RangeComponent::Relative}, {-3, RangeComponent::Relative});
  r.resolve(1, 42);

  CHECK(r.isValid());
  REQUIRE(r.min() == 39);
  REQUIRE(r.max() == 42);
}

TEST_CASE("relative wrapping of single ranges") {
  Range r(RangeComponent(-1, RangeComponent::Relative));
  r.resolve(1, 42);

  CHECK(r.isValid());
  REQUIRE(r.min() == 42);
  REQUIRE(r.max() == 42);
}

TEST_CASE("relative deep wrapping") {
  Range r(RangeComponent(-8, RangeComponent::Relative));
  r.resolve(1, 2);

  CHECK(r.isValid());
  REQUIRE(r.min() == 1);
  REQUIRE(r.max() == 1);
}

TEST_CASE("relative wrapping don't cause infinite loops due to bad input") {
  Range r(RangeComponent(-8, RangeComponent::Relative));
  r.resolve(1, 0);
}

TEST_CASE("counter is the range's minimum value") {
  Range r(2, 4);
  REQUIRE(r.counter() == r.min().value());
}
