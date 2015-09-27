#include "viewport.hpp"

#include "engine.hpp"
#include "page.hpp"

#include <QHBoxLayout>

Viewport::Viewport(QWidget *parent)
  : QWidget(parent)
{
  m_layout = new QHBoxLayout(this);
  m_layout->setContentsMargins(QMargins());
  m_layout->setSpacing(1);
}

void Viewport::addPage(Page *page)
{
  m_layout->addWidget(page->engine());
  page->setViewport(this);
}

int Viewport::count() const
{
  return m_layout->count();
}
