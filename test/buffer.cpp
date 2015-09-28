#include "vendor/catch.hpp"

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
  REQUIRE(b.mid(6) == Buffer("world"));
  REQUIRE(b == "hello world");
}
