#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;
class QWebEngineView;
class QWidget;
class TabLabel;
class Viewport;
class Window;

#include <QIcon>

class Page : public QObject {
  Q_OBJECT

public:
  Page(const QUrl &, Window *parent = 0);

  void setViewport(Viewport *v) { m_viewport = v; }
  Viewport *viewport() const { return m_viewport; }
  TabLabel *label() const { return m_label; }
  QWebEngineView *engine() const { return m_engine; }
  Window *window() const { return m_window; }

  const QIcon &icon() const { return m_icon; }

Q_SIGNALS:
  void titleChanged(Page *);
  void iconChanged(Page *);

private:
  void fetchIcon(const QUrl &);

  Window *m_window;
  Viewport *m_viewport;
  TabLabel *m_label;
  QWebEngineView *m_engine;

  QNetworkReply *m_iconReply;
  QNetworkAccessManager *m_iconRequestManager;
  QIcon m_icon;
};
