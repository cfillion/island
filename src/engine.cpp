#include "engine.hpp"

#include <QChildEvent>
#include <QWebEngineHistory>
#include <QWebEngineSettings>

Engine::Engine(const QUrl &url, QWidget *parent)
  : QWebEngineView(parent), m_deferredUrl(url)
{
  connect(page(), &QWebEnginePage::linkHovered, this, &Engine::linkHovered);

  settings()->setAttribute(QWebEngineSettings::LinksIncludedInFocusChain, false);

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

QUrl Engine::url() const
{
  if(!m_deferredUrl.isEmpty())
    return m_deferredUrl;

  QUrl url(page()->url());

  if(url.isEmpty())
    return page()->requestedUrl();

  return url;
}

bool Engine::historyMotion(const int movement)
{
  if(movement == 0)
    return true;

  const int size = history()->count();
  const int index = history()->currentItemIndex() + movement;
  const int bounded = std::max(0, std::min(index, size-1));

  history()->goToItem(history()->itemAt(bounded));

  return index >= 0 && index < size;
}
