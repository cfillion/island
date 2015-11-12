#ifndef ISLAND_ENGINE_HPP
#define ISLAND_ENGINE_HPP

#include <QUrl>
#include <QWebEngineView>

class Window;

class Engine : public QWebEngineView {
  Q_OBJECT

public:
  Engine(const QUrl &url, Window *parent = 0);
  QUrl url() const;
  QString title() const;

  bool loadDeferredUrl();

  bool canGoBack() const;
  bool canGoForward() const;
  bool historyMotion(const int);
  void setUrl(const QUrl &url);

Q_SIGNALS:
  void triggered();
  void linkHovered(const QString &);

protected:
  QWebEngineView *createWindow(QWebEnginePage::WebWindowType type) override;

  void showEvent(QShowEvent *) override;
  void childEvent(QChildEvent *) override;
  bool eventFilter(QObject *, QEvent *) override;

private:
  void setDeferredUrl(const QUrl &);

  QString m_titleOverride;
  QUrl m_deferredUrl;
  Window *m_window;
};

#endif
