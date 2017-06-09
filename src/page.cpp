#include "page.hpp"

#include "engine.hpp"
#include "tablabel.hpp"
#include "viewport.hpp"
#include "window.hpp"

Page::Page(const QString &url, Window *parent)
  : QObject(parent), m_window(parent), m_viewport(0), m_iconReply(0)
{
  m_engine = new Engine(parent);

  m_label = new TabLabel(parent);

  connect(this, &Page::indexChanged, label(), &TabLabel::setIndex);

  connect(engine(), &Engine::titleChanged, this, &Page::setTitle);
  connect(engine(), &Engine::triggered, this, [=] { Q_EMIT triggered(this); });
  connect(engine(), &Engine::linkHovered, this, &Page::setHoveredLink);
  connect(engine(), &Engine::loadStarted, this, [=] { setLoading(true); });
  connect(engine(), &Engine::loadProgress, this, &Page::setLoadProgress);
  connect(engine(), &Engine::loadFinished, this, [=] { setLoading(false); });
  connect(engine(), &Engine::iconChanged, label(), &TabLabel::setIcon);

  // proxy connections
  connect(label(), &TabLabel::textChanged, this, &Page::displayTitleChanged);
  connect(engine(), &Engine::urlChanged, this, &Page::displayUrlChanged);

  m_engine->setUrl(parseUrl(url));
}

void Page::destroyComponents()
{
  m_label->deleteLater();
  m_engine->deleteLater();

  if(m_viewport->count() == 1)
    m_viewport->deleteLater();
}

QString Page::displayTitle() const
{
  return label()->text();
}

int Page::index() const
{
  return label()->index();
}

void Page::setIndex(const int newIndex)
{
  Q_EMIT indexChanged(newIndex);
}

bool Page::isCurrent() const
{
  return label()->isEnabled();
}

void Page::setCurrent(const bool isCurrent)
{
  label()->setEnabled(isCurrent);
}

QString Page::displayUrl() const
{
  if(!m_hoveredLink.isEmpty())
    return m_hoveredLink;

  const QUrl url(engine()->url());
  return url.toString(QUrl::RemoveUserInfo | QUrl::DecodeReserved);
}

bool Page::isLoading() const
{
  return label()->isLoading();
}

int Page::loadProgress() const
{
  return label()->loadProgress();
}

void Page::setTitle()
{
  label()->setTitle(engine()->title());
}

void Page::setLoading(const bool isLoading)
{
  label()->setLoading(isLoading);
  Q_EMIT progressChanged();
}

void Page::setLoadProgress(const int progress)
{
  label()->setLoadProgress(progress);
  Q_EMIT progressChanged();
}

void Page::setHoveredLink(const QString &url)
{
  m_hoveredLink = url;
  Q_EMIT displayUrlChanged();
}

bool Page::historyMotion(const int motion)
{
  return m_engine->historyMotion(motion);
}

bool Page::canGoBack() const
{
  return m_engine->canGoBack();
}

bool Page::canGoForward() const
{
  return m_engine->canGoForward();
}

void Page::findText(const QString &input, const bool forward)
{
  QWebEnginePage::FindFlags flags;

  if(!forward)
    flags |= QWebEnginePage::FindBackward;

  m_engine->findText(input, flags);
}

void Page::reload(const bool useCache)
{
  engine()->reload(useCache);
}

void Page::stop()
{
  engine()->stop();
}

bool Page::load(const QString &input)
{
  const QUrl url = parseUrl(input);

  if(!url.isValid())
    return false;

  m_engine->setUrl(url);
  return true;
}

QUrl Page::parseUrl(const QString &input)
{
  return QUrl::fromUserInput(input);
}
