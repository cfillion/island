#include <QApplication>
#include <QKeyEvent>

#include "actions.hpp"
#include "mapping.hpp"
#include "names.hpp"
#include "window.hpp"

class EventFilter : public QObject {
protected:
  virtual bool eventFilter(QObject *o, QEvent *e) override
  {
    const bool isKeyPress = e->type() == QEvent::KeyPress;
    const bool isToWindow =
      o->objectName() == QStringLiteral("WindowClassWindow");

    Window *win = qobject_cast<Window *>(qApp->activeWindow());

    if(win && isKeyPress && isToWindow)
      return win->handleInput(static_cast<QKeyEvent *>(e));

    return false;
  }
};

int main(int argc, char *argv[])
{
  using namespace CommandOptions;

  UseCommandRegistry reg(&Island::ActionNames);

  QApplication app(argc, argv);

  EventFilter filter;
  app.installEventFilter(&filter);

  Mapping nmap;
  nmap.set("i", &Actions::insert_mode);
  nmap.set(":", &Actions::command_mode);
  nmap.set("d", &Actions::tab_close);
  nmap.set("H", &Actions::history_back);
  nmap.set("L", &Actions::history_forward);
  nmap.set("ga", &Actions::tab_open);
  nmap.set("r", &Actions::tab_reload);
  nmap.set("R", {&Actions::tab_reload, {}, VA_FORCE});
  nmap.set("o", ":open<Space>");
  nmap.set("O", ":tabopen<Space>");
  nmap.set("/", &Actions::search_forward);
  nmap.set("?", &Actions::search_backward);
  nmap.set("n", &Actions::search_next);
  nmap.set("N", &Actions::search_previous);
  nmap.set("<Esc>", &Actions::search_clear);

  Mapping imap;
  imap.set("<Esc>", &Actions::normal_mode);

  Mapping pmap;
  pmap.set("<Esc>", &Actions::normal_mode);
  pmap.set("<CR>", &Actions::prompt_execute);
  pmap.set("<Home>", &Actions::prompt_home);
  pmap.set("<End>", &Actions::prompt_end);
  pmap.set("<Left>", &Actions::prompt_left);
  pmap.set("<Right>", &Actions::prompt_right);
  pmap.set("<C-u>", &Actions::prompt_clear);
  pmap.set("<Tab>", &Actions::prompt_complete);
  pmap.set("<S-Tab>", &Actions::prompt_complete_reverse);

  Window win({&nmap, &imap, &pmap});
  win.show();

  return app.exec();
}
