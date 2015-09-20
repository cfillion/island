#include "vendor/catch.hpp"

#include <global.hpp>

static const char *M = "[global]";

using namespace Island;

TEST_CASE("input to buffer", M) {
  SECTION("single letter") {
    REQUIRE(InputToBuffer("a") == Buffer() << "a");
  }

  SECTION("consecutive letters") {
    REQUIRE(InputToBuffer("ab") == Buffer() << "a" << "b");
  }

  SECTION("special char") {
    REQUIRE(InputToBuffer("<Space>") == Buffer() << "<Space>");
  }

  SECTION("special char with letters") {
    REQUIRE(InputToBuffer("a<CR>b") == Buffer() << "a" << "<CR>" << "b");
  }
}
