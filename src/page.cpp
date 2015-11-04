#include "page.hpp"

#include "engine.hpp"
#include "tablabel.hpp"
#include "viewport.hpp"
#include "window.hpp"

#include <QNetworkReply>
#include <QNetworkRequest>

Page::Page(const QString &url, Window *parent)
  : QObject(parent), m_window(parent), m_viewport(0), m_iconReply(0)
{
  m_engine = new Engine(parseUrl(url), parent);

  m_label = new TabLabel(parent);
  m_label->setTitle(m_engine->title());

  // TODO: use a global app-wide instance
  m_iconRequestManager = new QNetworkAccessManager(this);

  connect(this, &Page::iconChanged, label(), &TabLabel::setIcon);

  connect(engine(), &Engine::titleChanged, label(), &TabLabel::setTitle);
  connect(engine(), &Engine::triggered, this, [=] { Q_EMIT triggered(this); });
  connect(engine(), &Engine::linkHovered, this, &Page::setHoveredLink);
  connect(engine(), &Engine::iconUrlChanged, this, &Page::fetchIcon);
  connect(engine(), &Engine::loadStarted, this, [=] { setLoading(true); });
  connect(engine(), &Engine::loadProgress, this, &Page::setLoadProgress);
  connect(engine(), &Engine::loadFinished, this, [=] { setLoading(false); });

  // proxy connections
  connect(label(), &TabLabel::textChanged, this, &Page::displayTitleChanged);
  connect(engine(), &Engine::urlChanged, this, &Page::displayUrlChanged);
}

void Page::destroyComponents()
{
  m_label->deleteLater();
  m_engine->deleteLater();

  if(m_viewport->count() == 1)
    m_viewport->deleteLater();
}

void Page::fetchIcon(const QUrl &url)
{
  if(m_iconReply) {
    if(m_iconReply->url() == url)
      return;

    m_iconReply->abort();
    m_iconReply->deleteLater();
  }

  m_iconReply = m_iconRequestManager->get(QNetworkRequest(url));
  m_iconReply->setParent(this);

  connect(m_iconReply, &QNetworkReply::finished, this, [=] {
    const QByteArray data = m_iconReply->readAll();
    m_icon = QIcon(QPixmap::fromImage(QImage::fromData(data)));
    Q_EMIT iconChanged(m_icon);
  });
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
  label()->setIndex(newIndex);
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

bool Page::load(const QString &input)
{
  const QUrl url = parseUrl(input);

  if(!url.isValid())
    return false;

  m_engine->setUrl(url);
  return true;
}

void Page::reload(const bool useCache)
{
  if(m_engine->loadDeferredUrl())
    return;

  m_engine->triggerPageAction(
    useCache ? QWebEnginePage::Reload
    : QWebEnginePage::ReloadAndBypassCache
  );
}

void Page::findText(const QString &input, const bool forward)
{
  QWebEnginePage::FindFlags flags;

  if(!forward)
    flags |= QWebEnginePage::FindBackward;

  m_engine->findText(input, flags);
}

void Page::stop()
{
  m_engine->triggerPageAction(QWebEnginePage::Stop);
}

QUrl Page::parseUrl(const QString &input)
{
  return QUrl::fromUserInput(input);
}
