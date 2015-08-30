#include <QObject>

class QWebEngineView;
class QWidget;
class TabLabel;
class Window;

class Page : public QObject {
  Q_OBJECT

public:
  Page(TabLabel *, QWidget *, QWebEngineView *, Window *);

  TabLabel *label() const { return m_label; }
  QWidget *viewport() const { return m_viewport; }
  QWebEngineView *engine() const { return m_engine; }
  Window *window() const { return m_window; }

Q_SIGNALS:
  void triggered(Page *);
  void titleChanged(Page *);

private:
  TabLabel *m_label;
  QWidget *m_viewport; // TODO: s/QWidget/Viewport
  QWebEngineView *m_engine;
  Window *m_window;
};
