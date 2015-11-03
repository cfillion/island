#include "engine.hpp"

#include <QChildEvent>
#include <QWebEngineHistory>
#include <QWebEngineSettings>

Engine::Engine(const QUrl &url, QWidget *parent)
  : QWebEngineView(parent)
{
  connect(page(), &QWebEnginePage::linkHovered, this, &Engine::linkHovered);

  settings()->setAttribute(QWebEngineSettings::LinksIncludedInFocusChain, false);

  // TODO: optional load on focus setting
  setDeferredUrl(url);
}

void Engine::showEvent(QShowEvent *)
{
  loadDeferredUrl();
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

QString Engine::title() const
{
  if(m_titleOverride.isEmpty())
    return QWebEngineView::title();
  else
    return m_titleOverride;
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

bool Engine::loadDeferredUrl()
{
  if(m_deferredUrl.isEmpty())
    return false;

  load(m_deferredUrl);
  m_deferredUrl = QUrl();

  return true;
}

void Engine::setDeferredUrl(const QUrl &url)
{
  m_deferredUrl = url;

  m_titleOverride = "*"+url
    .toString(QUrl::RemoveScheme | QUrl::RemoveUserInfo | QUrl::DecodeReserved)
    .remove(QRegExp("^//(www\\.)?"))
  ;

  Q_EMIT titleChanged(m_titleOverride);
}

void Engine::setUrl(const QUrl &url)
{
  if(m_deferredUrl.isEmpty())
    load(url);
  else
    setDeferredUrl(url);
}
