#include "page.hpp"

#include "engine.hpp"
#include "tablabel.hpp"
#include "window.hpp"

#include <QNetworkReply>
#include <QNetworkRequest>

Page::Page(const QUrl &url, Window *parent)
  : QObject(parent), m_window(parent), m_viewport(0), m_iconReply(0)
{
  m_engine = new Engine(url, parent);

  m_label = new TabLabel(parent);
  m_label->setTitle("*"+url
    .toString(QUrl::RemoveScheme | QUrl::RemoveUserInfo | QUrl::DecodeReserved)
    .remove(QRegExp("^//(www\\.)?"))
  );

  // TODO: use a global app-wide instance
  m_iconRequestManager = new QNetworkAccessManager(this);

  connect(this, &Page::iconChanged, label(), &TabLabel::setIcon);
  connect(engine(), &Engine::titleChanged, label(), &TabLabel::setTitle);
  connect(label(), &TabLabel::textChanged, this, &Page::titleChanged);
  connect(engine(), &Engine::urlChanged, this, &Page::urlChanged);
  connect(engine(), &Engine::iconUrlChanged, this, &Page::fetchIcon);
  connect(engine(), &Engine::triggered, this, [=] { Q_EMIT triggered(this); });

  connect(engine(), &Engine::loadStarted, this, [=] { setLoading(true); });
  connect(engine(), &Engine::loadProgress, this, &Page::setLoadProgress);
  connect(engine(), &Engine::loadFinished, this, [=] { setLoading(false); });

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

QUrl Page::url() const
{
  const QUrl url(engine()->url());

  if(url.isEmpty())
    return engine()->page()->requestedUrl();

  return url;
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
