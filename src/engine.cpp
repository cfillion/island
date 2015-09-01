#include "engine.hpp"

#include <QChildEvent>

Engine::Engine(const QUrl &url, QWidget *parent)
  : QWebEngineView(parent), m_deferredUrl(url)
{
  // TODO: optional load on focus setting
}

void Engine::showEvent(QShowEvent *)
{
  if(!m_deferredUrl.isEmpty()) {
    load(m_deferredUrl);
    m_deferredUrl = QUrl();
  }
}

void Engine::childEvent(QChildEvent *e)
{
  if(!e->added())
    return;

  // using childEvent is required to handle events on the webview
  // see https://bugreports.qt.io/browse/QTBUG-43602
  e->child()->installEventFilter(this);
}

bool Engine::eventFilter(QObject *, QEvent *e)
{
  if(e->type() == QEvent::MouseButtonPress)
    Q_EMIT triggered();

  return false;
}
