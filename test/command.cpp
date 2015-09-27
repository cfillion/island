#include "vendor/catch.hpp"

#include <command.hpp>

static void *g_ptr;

static CommandResult ptr_test(const Command &cmd)
{
  g_ptr = cmd.ptr<void *>();
  return CommandResult();
}

static const CommandRegistry TestReg{
  {"ptr_test", &ptr_test},
};

static const char *M = "[command]";

TEST_CASE("callback from reference") {
  Command cmd(&ptr_test);
  CHECK(cmd.isValid());
  REQUIRE(cmd.func() == &ptr_test);
}

TEST_CASE("callback from string") {
  UseCommandRegistry reg(&TestReg);

  SECTION("valid") {
    Command cmd("ptr_test");
    CHECK(cmd.isValid());
    REQUIRE(cmd.func() == &ptr_test);
  }

  SECTION("invalid") {
    Command cmd("helloworld");
    CHECK_FALSE(cmd.isValid());
  }
}

TEST_CASE("command execution", M) {
  g_ptr = 0;

  SECTION("data pointer") {
    Command cmd(&ptr_test);

    CHECK(cmd.ptr<void*>() == 0);
    cmd.exec((void*)0x42);
    CHECK(cmd.ptr<void*>() == 0);

    REQUIRE(g_ptr == (void*)0x42);
  }
}
