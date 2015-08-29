#include <QApplication>

#include <QWebEngineView>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  QWebEngineView *view = new QWebEngineView();
  view->load(QUrl("http://cfillion.tk"));
  view->show();

  return app.exec();
}
