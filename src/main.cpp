#include <QApplication>
#include <QKeyEvent>

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
  EventFilter filter;

  QApplication app(argc, argv);
  app.installEventFilter(&filter);

  Window win;
  win.show();

  return app.exec();
}
