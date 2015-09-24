#include <QApplication>
#include <QKeyEvent>

#include "commands.hpp"
#include "mapping.hpp"
#include "window.hpp"

class EventFilter : public QObject {
protected:
  virtual bool eventFilter(QObject *o, QEvent *e) override
  {
    const bool isKeyPress = e->type() == QEvent::KeyPress;
    const bool isToWindow = o->objectName() == "WindowClassWindow";

    Window *win = qobject_cast<Window *>(qApp->activeWindow());

    if(win && isKeyPress && isToWindow)
      return win->handleKeyEvent(static_cast<QKeyEvent *>(e));

    return false;
  }
};

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  EventFilter filter;
  app.installEventFilter(&filter);

  Mapping nmap;
  nmap.set("i", &Commands::insert);
  nmap.set(":", &Commands::prompt);

  Mapping imap;
  imap.set("<Esc>", &Commands::normal);

  Mapping pmap;
  pmap.set("<Esc>", &Commands::normal);

  Window win({&nmap, &imap, &pmap});
  win.show();

  return app.exec();
}
