#include "page.hpp"

#include "engine.hpp"
#include "tab_label.hpp"
#include "window.hpp"

#include <QNetworkReply>
#include <QNetworkRequest>

Page::Page(const QUrl &url, Window *parent)
  : QObject(parent), m_window(parent), m_viewport(0), m_iconReply(0), m_url(url)
{
  m_engine = new Engine(url, parent);

  m_label = new TabLabel(parent);
  m_label->setTitle("*"+url
    .toString(QUrl::RemoveScheme | QUrl::RemoveUserInfo | QUrl::DecodeReserved)
    .remove(QRegExp("^//(www\\.)?"))
  );

  // TODO: use a global app-wide instance
  m_iconRequestManager = new QNetworkAccessManager(this);

  connect(engine(), &Engine::titleChanged, label(), &TabLabel::setTitle);
  connect(engine(), &Engine::iconUrlChanged, this, &Page::fetchIcon);
  connect(engine(), &Engine::urlChanged, this, &Page::setUrl);
  connect(engine(), &Engine::triggered, this, [=] { Q_EMIT triggered(this); });

  connect(engine(), &Engine::loadStarted, this, &Page::loadStarted);
  connect(engine(), &Engine::loadProgress, this, &Page::loadProgress);
  connect(engine(), &Engine::loadFinished, this, &Page::loadFinished);

  connect(label(), &TabLabel::textChanged, this, &Page::titleChanged);

  connect(this, &Page::iconChanged, label(), &TabLabel::setIcon);
  connect(this, &Page::loadStarted, label(), &TabLabel::showProgress);
  connect(this, &Page::loadProgress, label(), &TabLabel::setProgress);
  connect(this, &Page::loadFinished, label(), &TabLabel::hideProgress);

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

void Page::setUrl(const QUrl &url)
{
  m_url = url;
  Q_EMIT urlChanged(this);
}
