#include "vendor/catch.hpp"
#include "helper/io.hpp"

#include <command.hpp>

static void *g_ptr;

static CommandResult test_cmd(const Command &cmd)
{
  g_ptr = const_cast<Command*>(&cmd);
  return CommandResult();
}

static CommandResult alt_cmd(const Command &)
{
  return CommandResult();
}

static const CommandRegistry TestReg{
  {"test_cmd", &test_cmd, true},
  {"tester", &alt_cmd, true},
  {"noarg", &test_cmd, false},
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
    const Command cmd("test_cmd");
    CHECK(cmd.isValid());
    REQUIRE(cmd.func() == &test_cmd);
  }

  SECTION("invalid") {
    const Command cmd("helloworld");
    CHECK_FALSE(cmd.isValid());
  }
}

TEST_CASE("counter from string") {
  const UseCommandRegistry reg(&TestReg);

  const Command cmd("42test_cmd");
  CHECK(cmd.isValid());
  CHECK(cmd.func() == &test_cmd);
  REQUIRE(cmd.counter() == 42);
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

TEST_CASE("set counter", M) {
  const UseCommandRegistry reg(&TestReg);

  Command ptr(&test_cmd);
  Command str("test_cmd");

  CHECK(ptr.counter() == -1);
  CHECK(str.counter() == -1);
  CHECK_FALSE(ptr.hasCounter());

  ptr.setCounter(42);
  REQUIRE(ptr.counter() == 42);
  REQUIRE(ptr.hasCounter());
}

TEST_CASE("counter from constructor", M) {
  Command ptr(&test_cmd, {}, 5);
  REQUIRE(ptr.counter() == 5);
}

TEST_CASE("zero counter is invalid", M) {
  Command cmd(&test_cmd);
  cmd.setCounter(0);
  REQUIRE_FALSE(cmd.hasCounter());
}

TEST_CASE("execute", M) {
  g_ptr = 0;

  const Command cmd(&test_cmd);
  cmd.exec();

  REQUIRE(g_ptr == &cmd);
}

TEST_CASE("partial command matching", M) {
  const UseCommandRegistry reg(&TestReg);

  SECTION("ambiguous") {
    const Command cmd("test");
    REQUIRE(cmd.isValid());
    REQUIRE(cmd.func() == &test_cmd);
  }

  SECTION("unambiguous") {
    const Command cmd("teste");
    REQUIRE(cmd.isValid());
    REQUIRE(cmd.func() == &alt_cmd);
  }
}

TEST_CASE("execute invalid command", M) {
  const UseCommandRegistry reg(&TestReg);

  const Command cmd("hello_world");
  const CommandResult res = cmd.exec();
  REQUIRE(res.ok == false);
  REQUIRE(res.message == "Not a command: hello_world");
}

TEST_CASE("ill-formed commands are invalid", M) {
  const UseCommandRegistry reg(&TestReg);

  const Command cmd("  : ");
  REQUIRE_FALSE(cmd.isValid());
}

TEST_CASE("direct argument", M) {
  const Command cmd(&test_cmd, "hello world");

  REQUIRE(cmd.arg() == "hello world");
}

TEST_CASE("argument from string", M) {
  const UseCommandRegistry reg(&TestReg);

  const Command cmd("test_cmd hello world");
  CHECK(cmd.isValid());
  CHECK(cmd.func() == &test_cmd);

  REQUIRE(cmd.arg() == "hello world");
}

TEST_CASE("unexpected argument", M) {
  const UseCommandRegistry reg(&TestReg);

  const Command cmd("noarg test");
  CHECK_FALSE(cmd.isValid());
  REQUIRE(cmd.exec().message == "Trailing characters");
}

TEST_CASE("ignore space padding", M) {
  const UseCommandRegistry reg(&TestReg);

  const Command cmd("  4  test_cmd     hello  world  ");
  CHECK(cmd.counter() == 4);
  CHECK(cmd.func() == &test_cmd);
  CHECK(cmd.arg() == "hello\x20world");
  REQUIRE(cmd.isValid());
}
