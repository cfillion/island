#ifndef ISLAND_ENGINE_HPP
#define ISLAND_ENGINE_HPP

#include <QUrl>
#include <QWebEngineView>

class Engine : public QWebEngineView {
  Q_OBJECT

public:
  Engine(const QUrl &url, QWidget *parent = 0);

Q_SIGNALS:
  void triggered();

protected:
  void showEvent(QShowEvent *) override;
  void enterEvent(QEvent *) override;

private:
  QUrl m_deferredUrl;
};

#endif
