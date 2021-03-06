#include <catch.hpp>
#include "helper/io.hpp"

#include <command.hpp>

static void *g_ptr;

static CommandResult address_cmd(const Command &cmd)
{
  g_ptr = const_cast<Command*>(&cmd);
  return CommandResult();
}

static CommandResult test_cmd(const Command &)
{
  return CommandResult();
}

static CommandResult alt_cmd(const Command &)
{
  return CommandResult();
}

using namespace CommandOptions;

static const CommandRegistry TestReg{
  {"test",   &test_cmd,  CO_ARG},
  {"tester", &alt_cmd,   CO_ARG},
  {"noarg",  &test_cmd},
  {"force",  &test_cmd,  CO_FORCE},
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
    const Command cmd("test");
    CHECK(cmd.isValid());
    REQUIRE(cmd.func() == &test_cmd);
  }

  SECTION("invalid") {
    const Command cmd("helloworld");
    CHECK_FALSE(cmd.isValid());
  }
}

TEST_CASE("set data pointer", M) {
  const UseCommandRegistry reg(&TestReg);

  Command ptr(&test_cmd);
  Command str("test");

  CHECK(ptr.data<void*>() == 0);
  CHECK(str.data<void*>() == 0);

  ptr.setData((void*)0x42);
  REQUIRE(ptr.data<void*>() == (void*)0x42);
}

TEST_CASE("data pointer from constructor", M) {
  Command ptr(&test_cmd, {}, VA_DEFAULT, Range(), (void*)0x42);
  REQUIRE(ptr.data<void*>() == (void*)0x42);
}

TEST_CASE("set range", M) {
  const UseCommandRegistry reg(&TestReg);

  Command ptr(&test_cmd);
  Command str("test");

  CHECK_FALSE(ptr.range().isValid());
  CHECK_FALSE(str.range().isValid());

  ptr.setRange({2, 4});
  REQUIRE(ptr.range() == Range(2, 4));
}

TEST_CASE("counter from constructor", M) {
  Command ptr(&test_cmd, {}, VA_DEFAULT, {1});
  REQUIRE(ptr.range() == Range(1));
}

TEST_CASE("range from string") {
  const UseCommandRegistry reg(&TestReg);

  SECTION("absolute") {
    const Command cmd("2,4test");
    CHECK(cmd.isValid());
    CHECK(cmd.func() == &test_cmd);
    REQUIRE(cmd.range().toString() == "2,4");
  }

  SECTION("relative") {
    const Command cmd("+4,-2test");
    CHECK(cmd.isValid());
    CHECK(cmd.func() == &test_cmd);
    REQUIRE(cmd.range().toString() == "+4,-2");
  }

  SECTION("null minimum") {
    const Command cmd(",2test");
    CHECK(cmd.isValid());
    CHECK(cmd.func() == &test_cmd);
    REQUIRE(cmd.range().toString() == ",2");
  }

  SECTION("null maximum") {
    const Command cmd("2,test");
    CHECK(cmd.isValid());
    CHECK(cmd.func() == &test_cmd);
    REQUIRE(cmd.range().toString() == "2");
  }
}

TEST_CASE("execute", M) {
  g_ptr = 0;

  const Command cmd(&address_cmd);
  cmd.exec();

  REQUIRE(g_ptr == &cmd);
}

TEST_CASE("partial command matching", M) {
  const UseCommandRegistry reg(&TestReg);

  SECTION("ambiguous") {
    const Command cmd("tes");
    REQUIRE(!cmd.isValid());
  }

  SECTION("exact match") {
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
  REQUIRE(cmd.errorString() == "Not a command: hello_world");

  const CommandResult res = cmd.exec();
  REQUIRE_FALSE(res.ok);
  REQUIRE(res.message == cmd.errorString());
}

TEST_CASE("ill-formed commands are invalid", M) {
  const UseCommandRegistry reg(&TestReg);

  const Command cmd("  : ");
  REQUIRE_FALSE(cmd.isValid());
}

TEST_CASE("argument from constructor", M) {
  const Command cmd(&test_cmd, "hello world");

  REQUIRE(cmd.argument() == "hello world");
}

TEST_CASE("argument from string", M) {
  const UseCommandRegistry reg(&TestReg);

  const Command cmd("test hello world");
  CHECK(cmd.isValid());
  CHECK(cmd.func() == &test_cmd);

  REQUIRE(cmd.hasArgument());
  REQUIRE(cmd.argument() == "hello world");
}

TEST_CASE("unexpected argument", M) {
  const UseCommandRegistry reg(&TestReg);

  const Command cmd("noarg test");
  CHECK_FALSE(cmd.isValid());
  REQUIRE_FALSE(cmd.hasArgument());
  REQUIRE(cmd.exec().message == "Trailing characters");
}

TEST_CASE("variant from reference", M) {
  const Command set(&test_cmd, {}, VA_FORCE);
  REQUIRE(set.variant() == VA_FORCE);

  const Command def(&test_cmd);
  REQUIRE(def.variant() == VA_DEFAULT);
}

TEST_CASE("variant from string", M) {
  const UseCommandRegistry reg(&TestReg);

  SECTION("default") {
    const Command cmd("test");
    REQUIRE(cmd.variant() == VA_DEFAULT);
  }

  SECTION("force") {
    const Command cmd("force!");
    CHECK(cmd.isValid());
    REQUIRE(cmd.variant() == VA_FORCE);
  }
}

TEST_CASE("invalid variant", M) {
  const UseCommandRegistry reg(&TestReg);

  const Command cmd("test!");
  CHECK_FALSE(cmd.isValid());
  REQUIRE(cmd.exec().message == "Not a command: test!");
}

TEST_CASE("entry from reference") {
  Command cmd(&test_cmd);
  REQUIRE(cmd.entry() == 0);
}

TEST_CASE("entry from string") {
  const UseCommandRegistry reg(&TestReg);

  SECTION("valid") {
    const Command cmd("test");
    REQUIRE(cmd.entry()->func == &test_cmd);
  }

  SECTION("invalid") {
    const Command cmd("helloworld");
    REQUIRE(cmd.entry() == 0);
  }
}

TEST_CASE("query flags") {
  const UseCommandRegistry reg(&TestReg);

  SECTION("valid") {
    REQUIRE(Command("test").hasFlag(CO_ARG));
    REQUIRE_FALSE(Command("noarg").hasFlag(CO_ARG));
  }

  SECTION("invalid") {
    const Command cmd("helloworld");
    REQUIRE_FALSE(cmd.hasFlag(CO_ARG));
  }

  SECTION("unknown") {
    const Command cmd(&test_cmd);
    REQUIRE_FALSE(cmd.hasFlag(CO_ARG));
  }
}

TEST_CASE("command search", M) {
  const UseCommandRegistry reg(&TestReg);

  SECTION("empty prefix") {
    const CommandList list = Command::findCommands("");

    CHECK(list.size() == 4);
    REQUIRE(list[0]->name == "force");
    REQUIRE(list[1]->name == "noarg");
    REQUIRE(list[2]->name == "test");
    REQUIRE(list[3]->name == "tester");
  }

  SECTION("matching prefix") {
    const CommandList list = Command::findCommands("test");

    CHECK(list.size() == 2);
    REQUIRE(list[0]->name == "test");
    REQUIRE(list[1]->name == "tester");
  }
}

TEST_CASE("command parser", M) {
  SECTION("empty name is valid") {
    const CommandParser parser("4,2");
    CHECK(parser.range() == "4,2");
    CHECK(parser.name() == "");
    REQUIRE(parser.isValid());
  }

  SECTION("empty argument is captured") {
    const CommandParser parser("4cmd ");
    CHECK(parser.argument() == "");
    CHECK(parser.argumentStart() == 5);
    REQUIRE(parser.isValid());
  }

  SECTION("ignores space padding") {
    const CommandParser parser("  4  test     !olleh  ");
    CHECK(parser.range() == "4");
    CHECK(parser.name() == "test");
    CHECK(parser.argument() == "!olleh");
    REQUIRE(parser.isValid());
  }

  SECTION("space padding before command name without range") {
    const CommandParser parser("  test ");
    CHECK(parser.range() == "");
    CHECK(parser.name() == "test");
    REQUIRE(parser.isValid());
  }
}
