#include "tablabel.hpp"

#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QResizeEvent>

TabLabel::TabLabel(QWidget *parent)
  : QWidget(parent)
{
  setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
  setDisabled(true);
  setFixedHeight(16); // TODO: setting

  m_icon = new QLabel;
  m_prefix = new QLabel;
  m_title = new QLabel;
  m_title->setTextFormat(Qt::PlainText);
  m_progress = new QLabel;

  setIndex(0);
  setIcon(QIcon());
  setTitle(QString());
  setLoading(false);

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
  Q_EMIT textChanged(text());
}

void TabLabel::setIcon(const QIcon &icon)
{
  const QPixmap pixmap(icon.pixmap(height(), height()));
  m_icon->setPixmap(pixmap);
  m_icon->setHidden(pixmap.isNull());
}

bool TabLabel::isLoading() const
{
  return m_progress->isVisible();
}

void TabLabel::setLoading(const bool isLoading)
{
  m_progress->setVisible(isLoading);
  setLoadProgress(0);
}

void TabLabel::setLoadProgress(const int progress)
{
  m_loadProgress = progress;
  m_progress->setText(QString("[%1%]").arg(m_loadProgress));
  Q_EMIT textChanged(text());
}

void TabLabel::resizeEvent(QResizeEvent *e)
{
  const int width = e->size().width();

  m_prefix->setVisible(m_icon->isHidden() || width > 32);
  m_title->setVisible(width > 48);
}

QString TabLabel::text() const
{
  if(m_progress->isVisible())
    return QString("%0 %2").arg(m_progress->text(), m_title->text());
  else
    return m_title->text();
}
