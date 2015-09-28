#include "vendor/catch.hpp"
#include "helper/io.hpp"

#include <global.hpp>

#include <QKeyEvent>

using namespace Island;

static const char *M = "[global]";

TEST_CASE("get character from key event", M) {
  SECTION("function keys") {
    const QKeyEvent e(QEvent::None, Qt::Key_F12, Qt::NoModifier);
    REQUIRE(GetKeyEventCharacter(&e) == "F12");
  }

  SECTION("special key") {
    const QKeyEvent e(QEvent::None, Qt::Key_Escape, Qt::NoModifier);
    REQUIRE(GetKeyEventCharacter(&e) == "Esc");
  }

  SECTION("letter") {
    SECTION("lowercase") {
      const QKeyEvent e(QEvent::None, Qt::Key_A, Qt::NoModifier);
      REQUIRE(GetKeyEventCharacter(&e) == "a");
    }

    SECTION("uppercase") {
      const QKeyEvent e(QEvent::None, Qt::Key_A, Qt::ShiftModifier);
      REQUIRE(GetKeyEventCharacter(&e) == "A");
    }
  }

  SECTION("letter with diacritics") {
    SECTION("lowercase") {
      const QKeyEvent e(QEvent::None, Qt::Key_Agrave, Qt::NoModifier);
      REQUIRE(GetKeyEventCharacter(&e) == "à");
    }

    SECTION("uppercase") {
      const QKeyEvent e(QEvent::None, Qt::Key_Agrave, Qt::ShiftModifier);
      REQUIRE(GetKeyEventCharacter(&e) == "À");
    }
  }
}

TEST_CASE("get sequence from key event", M) {
  SECTION("letter", M) {
    SECTION("no modifier") {
      const QKeyEvent e(QEvent::None, Qt::Key_A, Qt::NoModifier);
      REQUIRE(KeyEventToSequence(&e) == "a");
    }

    SECTION("control") {
      const QKeyEvent e(QEvent::None, Qt::Key_A, Qt::ControlModifier);
      REQUIRE(KeyEventToSequence(&e) == "<C-a>");
    }

    SECTION("shift") {
      const QKeyEvent e(QEvent::None, Qt::Key_A, Qt::ShiftModifier);
      REQUIRE(KeyEventToSequence(&e) == "A");
    }

    SECTION("control and shift") {
      const QKeyEvent e(QEvent::None, Qt::Key_A,
          Qt::ControlModifier | Qt::ShiftModifier);
      REQUIRE(KeyEventToSequence(&e) == "<C-A>");
    }

    SECTION("all modifiers") {
      const QKeyEvent e(QEvent::None, Qt::Key_P, Qt::ShiftModifier |
          Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier);
      REQUIRE(KeyEventToSequence(&e) == "<C-A-M-P>");
    }
  }

  SECTION("special key", M) {
    SECTION("no modifier") {
      const QKeyEvent e(QEvent::None, Qt::Key_Space, Qt::NoModifier);
      REQUIRE(KeyEventToSequence(&e) == "<Space>");
    }

    SECTION("alt") {
      const QKeyEvent e(QEvent::None, Qt::Key_Space, Qt::AltModifier);
      REQUIRE(KeyEventToSequence(&e) == "<A-Space>");
    }

    SECTION("shift") {
      const QKeyEvent e(QEvent::None, Qt::Key_Space, Qt::ShiftModifier);
      REQUIRE(KeyEventToSequence(&e) == "<S-Space>");
    }

    SECTION("all modifiers") {
      const QKeyEvent e(QEvent::None, Qt::Key_Enter, Qt::ShiftModifier |
          Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier);
      REQUIRE(KeyEventToSequence(&e) == "<C-A-M-S-CR>");
    }
  }

  SECTION("less/more than special case") {
    SECTION("shift is ignored") {
      const QKeyEvent e(QEvent::None, Qt::Key_Less, Qt::ShiftModifier);
      REQUIRE(KeyEventToSequence(&e) == "<lt>");
    }
  }
}
