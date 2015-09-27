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
      return win->handleKeyEvent(static_cast<QKeyEvent *>(e));

    return false;
  }
};

int main(int argc, char *argv[])
{
  UseCommandRegistry reg(&Island::ActionNames);

  QApplication app(argc, argv);

  EventFilter filter;
  app.installEventFilter(&filter);

  Mapping nmap;
  nmap.set("i", &Actions::insert);
  nmap.set(":", &Actions::prompt);

  Mapping imap;
  imap.set("<Esc>", &Actions::normal);

  Mapping pmap;
  pmap.set("<Esc>", &Actions::normal);

  Window win({&nmap, &imap, &pmap});
  win.show();

  return app.exec();
}
