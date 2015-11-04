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

class Page : public QObject {
  Q_OBJECT

public:
  Page(const QString &, Window *parent = 0);
  void destroyComponents();

  void setViewport(Viewport *v) { m_viewport = v; }
  Viewport *viewport() const { return m_viewport; }
  TabLabel *label() const { return m_label; }
  Engine *engine() const { return m_engine; }
  Window *window() const { return m_window; }

  const QIcon &icon() const { return m_icon; }
  QString displayTitle() const;
  QString displayUrl() const;

  int index() const;
  void setIndex(const int);
  bool isCurrent() const;
  void setCurrent(const bool);
  bool isLoading() const;
  int loadProgress() const;

  bool historyMotion(const int);
  bool load(const QString &input);
  void reload(const bool useCache = true);
  void findText(const QString &, const bool forward = true);
  void stop();

Q_SIGNALS:
  void displayTitleChanged(const QString &);
  void iconChanged(const QIcon &);
  void displayUrlChanged();
  void progressChanged();
  void triggered(Page *);

private:
  QUrl parseUrl(const QString &input);
  void setUrl(const QUrl &);
  void fetchIcon(const QUrl &);
  void setLoading(const bool);
  void setLoadProgress(const int);
  void setHoveredLink(const QString &);

  Window *m_window;
  Viewport *m_viewport;
  TabLabel *m_label;
  Engine *m_engine;

  QNetworkReply *m_iconReply;
  QNetworkAccessManager *m_iconRequestManager;

  QIcon m_icon;
  QString m_hoveredLink;
};

#endif
