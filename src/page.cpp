#include "page.hpp"

Page::Page(TabLabel *label, QWidget *viewport, QWebEngineView *wview)
  : m_label(label), m_viewport(viewport), m_engine(wview)
{
}
