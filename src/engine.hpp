#ifndef ISLAND_ENGINE_HPP
#define ISLAND_ENGINE_HPP

#include <QUrl>
#include <QWebEngineView>

class Engine : public QWebEngineView {
public:
  Engine(const QUrl &url, QWidget *parent = 0);
  const QUrl &initialUrl() const { return m_initialUrl; }

protected:
  void paintEvent(QPaintEvent *) override;

private:
  QUrl m_initialUrl;
};

#endif
