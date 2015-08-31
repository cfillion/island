#include "engine.hpp"

Engine::Engine(const QUrl &url, QWidget *parent)
  : QWebEngineView(parent), m_deferredUrl(url)
{
  // TODO: optional load on focus setting
}

void Engine::showEvent(QShowEvent *)
{
  if(!m_deferredUrl.isEmpty()) {
    load(m_deferredUrl);
    m_deferredUrl = QUrl();
  }
}

void Engine::enterEvent(QEvent *)
{
  Q_EMIT triggered();
}
