#include "page.hpp"

#include "tab_label.hpp"
#include "window.hpp"

#include <QWebEngineView>

Page::Page(TabLabel *l, QWidget *v, QWebEngineView *e, Window *w)
  : QObject(w), m_label(l), m_viewport(v), m_engine(e), m_window(w)
{
  connect(label(), &TabLabel::triggered,
      this, [=] { Q_EMIT triggered(this); });

  connect(engine(), &QWebEngineView::titleChanged,
      this, [=] { Q_EMIT titleChanged(this); });

  connect(engine(), &QWebEngineView::titleChanged,
      label(), &TabLabel::setTitle);
}
