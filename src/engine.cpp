#include "engine.hpp"

#include <QChildEvent>
#include <QWebEngineHistory>
#include <QWebEngineProfile>
#include <QWebEngineSettings>

#include "page.hpp"
#include "window.hpp"

Engine::Engine(Window *window)
  : QWebEngineView(window), m_window(window)
{
  EnginePage *page = new EnginePage(window);
  setPage(page);

  connect(page, &QWebEnginePage::linkHovered, this, &Engine::linkHovered);

  settings()->setAttribute(QWebEngineSettings::LinksIncludedInFocusChain, false);

  // TODO: optional load on focus setting
  page->setBlockRequests(true);
}

EnginePage *Engine::page() const
{
  return dynamic_cast<EnginePage *>(QWebEngineView::page());
}

void Engine::showEvent(QShowEvent *)
{
  if(page()->isBlockingRequests())
    reload();
}

void Engine::childEvent(QChildEvent *e)
{
  if(!e->added())
    return;

  // using childEvent is required to handle most events on the webview
  // see https://bugreports.qt.io/browse/QTBUG-43602
  e->child()->installEventFilter(this);
}

bool Engine::eventFilter(QObject *, QEvent *e)
{
  if(e->type() == QEvent::MouseButtonPress)
    Q_EMIT triggered();

  return false;
}

QWebEngineView *Engine::createWindow(QWebEnginePage::WebWindowType)
{
  const int index = m_window->addPage(QString());
  return m_window->page(index)->engine();
}

QUrl Engine::url() const
{
  QUrl url(page()->url());

  if(url.isEmpty())
    return page()->requestedUrl();

  return url;
}

QString Engine::title() const
{
  if(page()->isBlockingRequests()) {
    return "*"+url()
      .toString(QUrl::RemoveScheme | QUrl::RemoveUserInfo | QUrl::DecodeReserved)
      .remove(QRegExp("^//(www\\.)?"))
    ;
  }

  return QWebEngineView::title();
}

void Engine::reload(const bool useCache)
{
  if(page()->isBlockingRequests())
    page()->setBlockRequests(false);

  triggerPageAction(
    useCache ? QWebEnginePage::Reload
    : QWebEnginePage::ReloadAndBypassCache
  );
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

bool Engine::canGoBack() const
{
  return history()->canGoBack();
}

bool Engine::canGoForward() const
{
  return history()->canGoForward();
}

void Engine::stop()
{
  triggerPageAction(QWebEnginePage::Stop);
}

EnginePage::EnginePage(QObject *parent)
  : QWebEnginePage(new QWebEngineProfile(parent), parent),
    m_blockRequests(false)
{
}

Engine *EnginePage::view() const
{
  return dynamic_cast<Engine *>(QWebEnginePage::view());
}

bool EnginePage::acceptNavigationRequest(const QUrl &, NavigationType, bool)
{
  Q_EMIT titleChanged(QString());

  return !m_blockRequests;
}
