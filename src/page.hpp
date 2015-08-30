class QWebEngineView;
class QWidget;
class TabLabel;

class Page {
public:
  Page(TabLabel *, QWidget *, QWebEngineView *);

  TabLabel *label() const { return m_label; }
  QWidget *viewport() const { return m_viewport; }
  QWebEngineView *engine() const { return m_engine; }

private:
  TabLabel *m_label;
  QWidget *m_viewport; // TODO: s/QWidget/Viewport
  QWebEngineView *m_engine;
};
