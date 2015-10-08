#include "vendor/catch.hpp"
#include "helper/io.hpp"

#include <keypress.hpp>

#include <QKeyEvent>

static const char *M = "[keypress]";

TEST_CASE("letters", M) {
  SECTION("no modifier") {
    const KeyPress ki(Qt::Key_A);

    REQUIRE(ki.toString() == "a");
    REQUIRE(ki.displayString() == "a");
    REQUIRE(KeyPress("a") == ki);
  }

  SECTION("control") {
    const KeyPress ki(Qt::Key_A, Qt::ControlModifier);

    REQUIRE(ki.toString() == "<C-a>");
    REQUIRE(ki.displayString() == "");
    REQUIRE(KeyPress("<C-a>") == ki);
  }

  SECTION("shift") {
    const KeyPress ki(Qt::Key_A, Qt::ShiftModifier);

    REQUIRE(ki.toString() == "A");
    REQUIRE(KeyPress("A") == ki);
  }

  SECTION("all modifiers") {
    const KeyPress ki(Qt::Key_P, Qt::ShiftModifier |
      Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier);

    REQUIRE(ki.toString() == "<C-A-M-P>");
    REQUIRE(KeyPress("<C-A-M-P>") == ki);
  }
}

TEST_CASE("letters with diacritics", M) {
  SECTION("lowercase") {
    const KeyPress kp(Qt::Key_Yacute);
    REQUIRE(kp.toString() == "ý");
    REQUIRE(KeyPress("ý") == kp);
  }

  SECTION("uppercase") {
    const KeyPress kp(Qt::Key_Yacute, Qt::ShiftModifier);
    REQUIRE(kp.toString() == "Ý");
    REQUIRE(KeyPress("Ý") == kp);
  }
}

TEST_CASE("function keys", M) {
  const KeyPress ki(Qt::Key_F12);

  REQUIRE(ki.toString() == "<F12>");
  REQUIRE(KeyPress("<F12>") == ki);
}

TEST_CASE("special keys", M) {
  SECTION("no modifier") {
    const KeyPress ki(Qt::Key_Space);

    REQUIRE(ki.toString() == "<Space>");
    REQUIRE(ki.displayString() == "\x20");
    REQUIRE(KeyPress("<Space>") == ki);
  }

  SECTION("shift") {
    const KeyPress ki(Qt::Key_Space, Qt::ShiftModifier);

    REQUIRE(ki.toString() == "<S-Space>");
    REQUIRE(KeyPress("<S-Space>") == ki);
  }

  SECTION("all modifiers") {
    const KeyPress ki(Qt::Key_Return, Qt::ShiftModifier |
        Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier);

    REQUIRE(ki.toString() == "<C-A-M-S-CR>");
    REQUIRE(KeyPress("<C-A-M-S-CR>") == ki);
  }

  SECTION("alias") {
    const KeyPress ki(Qt::Key_Enter);
    REQUIRE(ki.toString() == "<CR>");
    REQUIRE(KeyPress("<CR>") != ki);
    REQUIRE(KeyPress("<CR>") == KeyPress(Qt::Key_Return));
  }

  SECTION("non printable") {
    const KeyPress ki(Qt::Key_Return);
    REQUIRE(ki.displayString() == "");
  }
}

TEST_CASE("less/more keys", M) {
  SECTION("shift is ignored") {
    const KeyPress ki(Qt::Key_Less, Qt::ShiftModifier);
    REQUIRE(ki.toString() == "<lt>");
  }
}

TEST_CASE("modifiers keys", M) {
  const KeyPress ki(Qt::Key_Control, Qt::ControlModifier);
  REQUIRE(ki.toString() == "");
  REQUIRE(ki.displayString() == "");
}

TEST_CASE("unprintable keys", M) {
  const KeyPress ki(Qt::Key_Clear);
  REQUIRE(ki.displayString() == "");
}

TEST_CASE("key event conversion", M) {
  QKeyEvent e(QEvent::None, Qt::Key_E, Qt::NoModifier, "é");
  const KeyPress ki(&e);
  REQUIRE(ki.toString() == "é");
}
