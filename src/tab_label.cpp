#include "tab_label.hpp"

#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QResizeEvent>

TabLabel::TabLabel(QWidget *parent)
  : QWidget(parent)
{
  setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
  setDisabled(true);

  m_icon = new QLabel;
  m_prefix = new QLabel;
  m_title = new QLabel;
  m_title->setTextFormat(Qt::PlainText);
  m_progress = new QLabel;

  setIndex(0);
  setIcon(QIcon());
  setTitle(QString());
  hideProgress();

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setAlignment(Qt::AlignLeft);
  layout->setSpacing(2);
  layout->setContentsMargins(QMargins());
  layout->addWidget(m_icon);
  layout->addWidget(m_prefix);
  layout->addWidget(m_progress);
  layout->addWidget(m_title);
  layout->setStretch(3, 2);
}

void TabLabel::setIndex(const int newIndex)
{
  m_index = newIndex;
  m_prefix->setText(QString("%1:").arg(m_index + 1));
}

void TabLabel::setTitle(const QString &newTitle)
{
  m_title->setText(newTitle.isEmpty() ? "---" : newTitle);
}

void TabLabel::setIcon(const QIcon &icon)
{
  m_icon->setPixmap(icon.pixmap(16, 16));
  m_icon->setHidden(m_icon->pixmap()->isNull());
}

void TabLabel::showProgress()
{
  m_progress->show();
}

void TabLabel::setProgress(const int progress)
{
  m_progress->setText(QString("[%1%]").arg(progress));
}

void TabLabel::hideProgress()
{
  m_progress->hide();
  setProgress(0);
}

void TabLabel::resizeEvent(QResizeEvent *e)
{
  const int width = e->size().width();

  m_prefix->setVisible(m_icon->isHidden() || width > 32);
  m_title->setVisible(width > 48);
}
