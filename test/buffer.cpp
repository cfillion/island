#include <catch.hpp>
#include "helper/io.hpp"

#include <buffer.hpp>
#include <range.hpp>

static const char *M = "[buffer]";

TEST_CASE("input to buffer", M) {
  SECTION("single letter") {
    REQUIRE(Buffer("a") == Buffer() << "a");
  }

  SECTION("consecutive letters") {
    REQUIRE(Buffer("ab") == Buffer() << "a" << "b");
  }

  SECTION("special char") {
    REQUIRE(Buffer("<Space>") == Buffer() << "<Space>");
  }

  SECTION("special char with letters") {
    REQUIRE(Buffer("a<CR>b") == Buffer() << "a" << "<CR>" << "b");
  }

  SECTION("format sequences") {
    REQUIRE(Buffer("a%ub") == Buffer() << "a" << "%u" << "b");
  }
}

TEST_CASE("truncate buffer", M) {
  Buffer b("hello world");
  b.truncate(6);
  REQUIRE(b == "world");
  REQUIRE(b.truncateCopy(1) == "orld");
}

TEST_CASE("range", M) {
  Buffer b;
  REQUIRE(b.range() == Range());

  b << "+4" << "," << "-" << "2";
  REQUIRE(b.range().toString() == "+4,-2");
  REQUIRE(b.empty());
  REQUIRE(b.toString() == "+4,-2");

  b << "a" << "3";
  REQUIRE(b.range().toString() == "+4,-2");
  REQUIRE(b.size() == 2);
  REQUIRE(b.toString() == "+4,-2a3");

  REQUIRE(b.truncateCopy(0).range() == b.range());
  REQUIRE(b.truncateCopy(b.size()).range() == Range());

  b.clear();
  REQUIRE(b.range() == Range());
}

TEST_CASE("ignore leading zeros from range") {
  const Buffer b("001");
  CHECK(b.range() == Range(1));
  CHECK(b.empty());
  REQUIRE(b.toString() == "1");
}

TEST_CASE("zero range is ignored") {
  const Buffer b("00");
  CHECK(b.empty());
  REQUIRE_FALSE(b.range().isValid());
}
