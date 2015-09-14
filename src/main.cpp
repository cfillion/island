#include <QApplication>
#include <QKeyEvent>

#include "mapping.hpp"
#include "window.hpp"

class EventFilter : public QObject {
protected:
  virtual bool eventFilter(QObject *, QEvent *e) override
  {
    Window *win = qobject_cast<Window *>(qApp->activeWindow());

    if(e->type() == QEvent::KeyPress && win)
      return win->handleKeyEvent(static_cast<QKeyEvent *>(e));

    return false;
  }
};

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  EventFilter filter;
  app.installEventFilter(&filter);

  Mapping mapping;
  mapping.nmap("i", ":insert");

  Window win(&mapping);
  win.show();

  return app.exec();
}
