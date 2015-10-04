#include "vendor/catch.hpp"
#include "helper/io.hpp"

#include <global.hpp>

#include <QKeyEvent>

using namespace Island;

static const char *M = "[global]";

TEST_CASE("encode character", M) {
  SECTION("function keys") {
    const QKeyEvent e(QEvent::None, Qt::Key_F12, Qt::NoModifier);
    REQUIRE(EncodeCharacter(&e) == "F12");
  }

  SECTION("special key") {
    const QKeyEvent e(QEvent::None, Qt::Key_Return, Qt::NoModifier);
    REQUIRE(EncodeCharacter(&e) == "CR");
  }

  SECTION("aliases") {
    const QKeyEvent e(QEvent::None, Qt::Key_Enter, Qt::NoModifier);
    REQUIRE(EncodeCharacter(&e) == "CR");
  }

  SECTION("letter") {
    SECTION("lowercase") {
      const QKeyEvent e(QEvent::None, Qt::Key_A, Qt::NoModifier);
      REQUIRE(EncodeCharacter(&e) == "a");
    }

    SECTION("uppercase") {
      const QKeyEvent e(QEvent::None, Qt::Key_A, Qt::ShiftModifier);
      REQUIRE(EncodeCharacter(&e) == "A");
    }
  }

  SECTION("letter with diacritics") {
    SECTION("lowercase") {
      const QKeyEvent e(QEvent::None, Qt::Key_Agrave, Qt::NoModifier);
      REQUIRE(EncodeCharacter(&e) == "à");
    }

    SECTION("uppercase") {
      const QKeyEvent e(QEvent::None, Qt::Key_Agrave, Qt::ShiftModifier);
      REQUIRE(EncodeCharacter(&e) == "À");
    }
  }
}

TEST_CASE("encode sequence", M) {
  SECTION("letter", M) {
    SECTION("no modifier") {
      const QKeyEvent e(QEvent::None, Qt::Key_A, Qt::NoModifier);
      REQUIRE(EncodeSequence(&e) == "a");
    }

    SECTION("control") {
      const QKeyEvent e(QEvent::None, Qt::Key_A, Qt::ControlModifier);
      REQUIRE(EncodeSequence(&e) == "<C-a>");
    }

    SECTION("shift") {
      const QKeyEvent e(QEvent::None, Qt::Key_A, Qt::ShiftModifier);
      REQUIRE(EncodeSequence(&e) == "A");
    }

    SECTION("control and shift") {
      const QKeyEvent e(QEvent::None, Qt::Key_A,
          Qt::ControlModifier | Qt::ShiftModifier);
      REQUIRE(EncodeSequence(&e) == "<C-A>");
    }

    SECTION("all modifiers") {
      const QKeyEvent e(QEvent::None, Qt::Key_P, Qt::ShiftModifier |
          Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier);
      REQUIRE(EncodeSequence(&e) == "<C-A-M-P>");
    }
  }

  SECTION("special key", M) {
    SECTION("no modifier") {
      const QKeyEvent e(QEvent::None, Qt::Key_Space, Qt::NoModifier);
      REQUIRE(EncodeSequence(&e) == "<Space>");
    }

    SECTION("alt") {
      const QKeyEvent e(QEvent::None, Qt::Key_Space, Qt::AltModifier);
      REQUIRE(EncodeSequence(&e) == "<A-Space>");
    }

    SECTION("shift") {
      const QKeyEvent e(QEvent::None, Qt::Key_Space, Qt::ShiftModifier);
      REQUIRE(EncodeSequence(&e) == "<S-Space>");
    }

    SECTION("all modifiers") {
      const QKeyEvent e(QEvent::None, Qt::Key_Return, Qt::ShiftModifier |
          Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier);
      REQUIRE(EncodeSequence(&e) == "<C-A-M-S-CR>");
    }
  }

  SECTION("less/more than special case") {
    SECTION("shift is ignored") {
      const QKeyEvent e(QEvent::None, Qt::Key_Less, Qt::ShiftModifier);
      REQUIRE(EncodeSequence(&e) == "<lt>");
    }
  }
}

TEST_CASE("decode character", M) {
  SECTION("function keys") {
    const KeyModPair p{Qt::Key_F12, Qt::NoModifier};
    REQUIRE(DecodeCharacter("F12") == p);
  }

  SECTION("special key") {
    const KeyModPair p{Qt::Key_Escape, Qt::NoModifier};
    REQUIRE(DecodeCharacter("Esc") == p);
  }

  SECTION("letter") {
    SECTION("lowercase") {
      const KeyModPair p{Qt::Key_A, Qt::NoModifier};
      REQUIRE(DecodeCharacter("a") == p);
    }

    SECTION("uppercase") {
      const KeyModPair p{Qt::Key_A, Qt::ShiftModifier};
      REQUIRE(DecodeCharacter("A") == p);
    }
  }

  SECTION("letter with diacritics") {
    SECTION("lowercase") {
      const KeyModPair p{Qt::Key_Yacute, Qt::NoModifier};
      REQUIRE(DecodeCharacter("ý") == p);
    }

    SECTION("uppercase") {
      const KeyModPair p{Qt::Key_Yacute, Qt::ShiftModifier};
      REQUIRE(DecodeCharacter("Ý") == p);
    }
  }
}

TEST_CASE("decode sequence", M) {
  SECTION("letter", M) {
    SECTION("no modifier") {
      const KeyModPair p{Qt::Key_A, Qt::NoModifier};
      REQUIRE(DecodeSequence("a") == p);
    }

    SECTION("control") {
      const KeyModPair p{Qt::Key_A, Qt::ControlModifier};
      REQUIRE(DecodeSequence("<C-a>") == p);
    }

    SECTION("shift") {
      const KeyModPair p{Qt::Key_A, Qt::ShiftModifier};
      REQUIRE(DecodeSequence("A") == p);
    }

    SECTION("control and shift") {
      const KeyModPair p{Qt::Key_A, Qt::ControlModifier | Qt::ShiftModifier};
      REQUIRE(DecodeSequence("<C-A>") == p);
    }

    SECTION("all modifiers") {
      const KeyModPair p{Qt::Key_P, Qt::ShiftModifier |
          Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier};
      REQUIRE(DecodeSequence("<C-A-M-P>") == p);
    }
  }

  SECTION("special key", M) {
    SECTION("no modifier") {
      const KeyModPair p{Qt::Key_Space, Qt::NoModifier};
      REQUIRE(DecodeSequence("<Space>") == p);
    }

    SECTION("alt") {
      const KeyModPair p{Qt::Key_Space, Qt::AltModifier};
      REQUIRE(DecodeSequence("<A-Space>") == p);
    }

    SECTION("shift") {
      const KeyModPair p{Qt::Key_Space, Qt::ShiftModifier};
      REQUIRE(DecodeSequence("<S-Space>") == p);
    }

    SECTION("all modifiers") {
      const KeyModPair p{Qt::Key_Return, Qt::ShiftModifier |
          Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier};
      REQUIRE(DecodeSequence("<C-A-M-S-CR>") == p);
    }
  }
}
