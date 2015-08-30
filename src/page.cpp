#include "page.hpp"

#include "tab_label.hpp"
#include "window.hpp"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QWebEngineView>

Page::Page(TabLabel *l, QWidget *v, QWebEngineView *e, Window *w)
  : QObject(w), m_label(l), m_viewport(v), m_engine(e), m_window(w),
    m_iconReply(0)
{
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
