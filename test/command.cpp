#include "vendor/catch.hpp"

#include <command.hpp>

static void *g_ptr;

static CommandResult test_cmd(const Command &cmd)
{
  g_ptr = const_cast<Command*>(&cmd);
  return CommandResult();
}

static const CommandRegistry TestReg{
  {"test_cmd", &test_cmd},
};

static const char *M = "[command]";

TEST_CASE("callback from reference") {
  Command cmd(&test_cmd);
  CHECK(cmd.isValid());
  REQUIRE(cmd.func() == &test_cmd);
}

TEST_CASE("callback from string") {
  const UseCommandRegistry reg(&TestReg);

  SECTION("valid") {
    Command cmd("test_cmd");
    CHECK(cmd.isValid());
    REQUIRE(cmd.func() == &test_cmd);
  }

  SECTION("invalid") {
    Command cmd("helloworld");
    CHECK_FALSE(cmd.isValid());
  }
}

TEST_CASE("set data pointer", M) {
  const UseCommandRegistry reg(&TestReg);

  Command ptr(&test_cmd);
  Command str("test_cmd");

  CHECK(ptr.data<void*>() == 0);
  CHECK(str.data<void*>() == 0);

  ptr.setData((void*)0x42);
  REQUIRE(ptr.data<void*>() == (void*)0x42);
}

TEST_CASE("execute", M) {
  g_ptr = 0;

  const Command cmd(&test_cmd);
  cmd.exec();

  REQUIRE(g_ptr == &cmd);
}
