#include "tabbar.hpp"

#include "tablabel.hpp"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QWheelEvent>

TabBar::TabBar(QWidget *parent)
  : QWidget(parent), m_wheelDelta(0), m_wheelPolarity(false)
{
  m_layout = new QHBoxLayout(this);
  m_layout->setSpacing(1);
  m_layout->setContentsMargins(QMargins());
}

void TabBar::insertLabel(const int index, TabLabel *label)
{
  m_layout->insertWidget(index, label);
}

void TabBar::mousePressEvent(QMouseEvent *e)
{
  QObject *obj = childAt(e->pos());

  while(obj && obj->parent() != this)
    obj = obj->parent(); 

  TabLabel *label = qobject_cast<TabLabel *>(obj);

  if(!label)
    return;

  switch(e->button()) {
  case Qt::LeftButton:
    Q_EMIT triggered(label->index());
    break;
  case Qt::RightButton:
    Q_EMIT closeRequested(label->index());
    break;
  default:
    break;
  }
}

void TabBar::wheelEvent(QWheelEvent *e)
{
  const int delta = e->angleDelta().y();

  const bool polarity = delta > 0;

  if(m_wheelPolarity != polarity) {
    m_wheelDelta = 0;
    m_wheelPolarity = polarity;
  }

  const int precision = 1; // TODO: setting
  m_wheelDelta += abs(delta) / (precision * 120.0);

  if(m_wheelDelta < 1.0)
    return;

  Q_EMIT wheelMotion(polarity, 1);
  m_wheelDelta = 0;
}
