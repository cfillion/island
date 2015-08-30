#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;
class QWebEngineView;
class QWidget;
class TabLabel;
class Window;

#include <QIcon>

class Page : public QObject {
  Q_OBJECT

public:
  Page(const QUrl &, Window *parent = 0);

  TabLabel *label() const { return m_label; }
  // QWidget *viewport() const { return m_viewport; }
  QWebEngineView *engine() const { return m_engine; }
  Window *window() const { return m_window; }

  const QIcon &icon() const { return m_icon; }

Q_SIGNALS:
  void titleChanged(Page *);
  void iconChanged(Page *);

private:
  void fetchIcon(const QUrl &);

  TabLabel *m_label;
  // QWidget *m_viewport; // TODO: s/QWidget/Viewport
  QWebEngineView *m_engine;
  Window *m_window;

  QNetworkReply *m_iconReply;
  QNetworkAccessManager *m_iconRequestManager;
  QIcon m_icon;
};
