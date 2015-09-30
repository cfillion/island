#include "vendor/catch.hpp"
#include "helper/io.hpp"

#include <buffer.hpp>

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
}

TEST_CASE("truncate buffer", M) {
  Buffer b("hello world");
  b.truncate(6);
  REQUIRE(b == "world");
  REQUIRE(b.truncateCopy(1) == "orld");
}

TEST_CASE("counter", M) {
  Buffer b;
  REQUIRE(b.counter() == -1);

  b << "4" << "2";
  REQUIRE(b.counter() == 42);
  REQUIRE(b.size() == 0);
  REQUIRE(b.toString() == "42");

  b << "a" << "3";
  REQUIRE(b.counter() == 42);
  REQUIRE(b.size() == 2);
  REQUIRE(b.toString() == "42a3");

  REQUIRE(b.truncateCopy(0).counter() == 42);
  REQUIRE(b.truncateCopy(b.size()).counter() == -1);

  b.clear();
  REQUIRE(b.counter() == -1);
}

TEST_CASE("ignore leading zeros from counter") {
  const Buffer b("001");
  CHECK(b.counter() == 1);
  CHECK(b.size() == 0);
  CHECK(b.toString() == "1");
}
