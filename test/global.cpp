#include "vendor/catch.hpp"

#include <global.hpp>

static const char *M = "[global]";

using namespace Island;

TEST_CASE("input to buffer", M) {
  SECTION("single letter") {
    REQUIRE(InputToBuffer("a") == QStringList() << "a");
  }

  SECTION("consecutive letters") {
    REQUIRE(InputToBuffer("ab") == QStringList() << "a" << "b");
  }

  SECTION("special char") {
    REQUIRE(InputToBuffer("<Space>") == QStringList() << "<Space>");
  }

  SECTION("special char with letters") {
    REQUIRE(InputToBuffer("a<CR>b") == QStringList() << "a" << "<CR>" << "b");
  }
}
