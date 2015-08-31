#include "page.hpp"

#include "engine.hpp"
#include "tab_label.hpp"
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

  connect(engine(), &Engine::iconUrlChanged, this, &Page::fetchIcon);
  connect(engine(), &Engine::titleChanged, this, &Page::setTitle);
  connect(engine(), &Engine::triggered, this, &Page::trigger);
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

    label()->setIcon(m_icon);
    Q_EMIT iconChanged(this);
  });
}

void Page::setTitle(const QString &newTitle)
{
  m_title = newTitle;
  label()->setTitle(m_title);

  Q_EMIT titleChanged(this);
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

void Page::trigger()
{
  if(!isCurrent())
    Q_EMIT triggered(this);
}
