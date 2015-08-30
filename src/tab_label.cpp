#include "tab_label.hpp"

#include <QHBoxLayout>
#include <QLabel>

TabLabel::TabLabel(QWidget *parent)
  : QWidget(parent)
{
  m_prefix = new QLabel;
  m_title = new QLabel;

  setIndex(0);
  setTitle("---");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->addWidget(m_prefix);
  layout->addWidget(m_title);
  layout->setStretch(1, 2);
}

void TabLabel::setIndex(const int newIndex)
{
  m_index = newIndex;
  m_prefix->setText(QString("%1:").arg(m_index + 1));
}

void TabLabel::setTitle(const QString &newTitle)
{
  m_title->setText(newTitle);
}

void TabLabel::mousePressEvent(QMouseEvent *)
{
  Q_EMIT triggered(m_index);
}
