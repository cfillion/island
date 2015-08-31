#ifndef ISLAND_PAGE_HPP
#define ISLAND_PAGE_HPP

#include <QObject>

class Engine;
class QNetworkAccessManager;
class QNetworkReply;
class QWidget;
class TabLabel;
class Viewport;
class Window;

#include <QIcon>
#include <QUrl>

class Page : public QObject {
  Q_OBJECT

public:
  Page(const QUrl &, Window *parent = 0);

  void setViewport(Viewport *v) { m_viewport = v; }
  Viewport *viewport() const { return m_viewport; }
  TabLabel *label() const { return m_label; }
  Engine *engine() const { return m_engine; }
  Window *window() const { return m_window; }

  const QIcon &icon() const { return m_icon; }
  const QString &title() const { return m_title; }
  const QUrl &url() const { return m_url; }

  int index() const;
  void setIndex(const int);
  bool isCurrent() const;
  void setCurrent(const bool);

Q_SIGNALS:
  void titleChanged(Page *);
  void iconChanged(Page *);
  void urlChanged(Page *);
  void triggered(Page *);

private:
  void setTitle(const QString &);
  void fetchIcon(const QUrl &);
  void setUrl(const QUrl &);
  void trigger();

  Window *m_window;
  Viewport *m_viewport;
  TabLabel *m_label;
  Engine *m_engine;

  QNetworkReply *m_iconReply;
  QNetworkAccessManager *m_iconRequestManager;
  QIcon m_icon;
  QString m_title;
  QUrl m_url;
};

#endif
