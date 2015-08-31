#include "engine.hpp"

Engine::Engine(const QUrl &url, QWidget *parent)
  : QWebEngineView(parent), m_initialUrl(url)
{
  // TODO: optional load on focus setting
}

void Engine::paintEvent(QPaintEvent *)
{
  if(!m_initialUrl.isEmpty()) {
    load(m_initialUrl);
    m_initialUrl = QUrl();
  }
}
