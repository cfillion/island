#include "vendor/catch.hpp"

#include <command.hpp>

static void *g_ptr;

CommandResult test_cmd(const Command &cmd)
{
  g_ptr = cmd.ptr<void *>();
  return CommandResult();
}

static const char *M = "[command]";

TEST_CASE("command execution", M) {
  g_ptr = 0;

  SECTION("data pointer") {
    Command cmd(&test_cmd);

    CHECK(cmd.ptr<void*>() == 0);
    cmd.exec((void*)0x42);
    CHECK(cmd.ptr<void*>() == 0);

    REQUIRE(g_ptr == (void*)0x42);
  }
}
