#include "page.hpp"

#include "tab_label.hpp"
#include "window.hpp"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QWebEngineView>

Page::Page(const QUrl &url, Window *parent)
  : QObject(parent), m_window(parent), m_viewport(0), m_iconReply(0)
{
  m_label = new TabLabel(parent);
  m_engine = new QWebEngineView(parent);
  m_engine->load(url);

  // TODO: use a global app-wide instance
  m_iconRequestManager = new QNetworkAccessManager(this);

  connect(engine(), &QWebEngineView::titleChanged,
      this, [=] { Q_EMIT titleChanged(this); });

  connect(engine(), &QWebEngineView::iconUrlChanged, this, &Page::fetchIcon);

  connect(engine(), &QWebEngineView::titleChanged,
      label(), &TabLabel::setTitle);
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
