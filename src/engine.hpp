#ifndef ISLAND_ENGINE_HPP
#define ISLAND_ENGINE_HPP

#include <QUrl>
#include <QWebEngineView>

class EnginePage;
class Window;

class Engine : public QWebEngineView {
  Q_OBJECT

public:
  Engine(Window *parent = 0);
  QUrl url() const;
  QString title() const;

  EnginePage *page() const;

  void reload(const bool useCache = true);
  void stop();
  bool canGoBack() const;
  bool canGoForward() const;
  bool historyMotion(const int);

Q_SIGNALS:
  void triggered();
  void linkHovered(const QString &);

protected:
  QWebEngineView *createWindow(QWebEnginePage::WebWindowType type) override;

  void showEvent(QShowEvent *) override;
  void childEvent(QChildEvent *) override;
  bool eventFilter(QObject *, QEvent *) override;

private:
  Window *m_window;
};

class EnginePage : public QWebEnginePage {
  Q_OBJECT

public:
  EnginePage(QObject *parent = 0);

  bool isBlockingRequests() const { return m_blockRequests; }
  void setBlockRequests(const bool newVal) { m_blockRequests = newVal; }

  Engine *view() const;

protected:
  bool acceptNavigationRequest(const QUrl &, NavigationType, bool) override;

private:
  bool m_blockRequests;
};

#endif
