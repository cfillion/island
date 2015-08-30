#include <QApplication>

#include "window.hpp"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  app.setApplicationDisplayName("Island");

  Window win;
  win.show();

  return app.exec();
}
