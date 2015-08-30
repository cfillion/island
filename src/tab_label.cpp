#include "tab_label.hpp"

#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMouseEvent>

TabLabel::TabLabel(QWidget *parent)
  : QWidget(parent)
{
  m_icon = new QLabel;
  m_prefix = new QLabel;
  m_title = new QLabel;

  setIndex(0);
  setTitle("---");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->addWidget(m_icon);
  layout->addWidget(m_prefix);
  layout->addWidget(m_title);
  layout->setStretch(2, 2);
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

void TabLabel::setIcon(const QIcon &icon)
{
  m_icon->setPixmap(icon.pixmap(16, 16));
}
