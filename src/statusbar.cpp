#include "statusbar.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QUrl>

StatusBar::StatusBar(QWidget *parent)
  : QWidget(parent)
{
  setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);

  m_url = new QLabel;
  m_url->setAlignment(Qt::AlignRight);
  m_url->setTextFormat(Qt::PlainText);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setSpacing(1);
  layout->setContentsMargins(QMargins());
  layout->addWidget(m_url);
}

void StatusBar::setUrl(const QUrl &url)
{
  m_url->setText(url.toString(QUrl::RemoveUserInfo | QUrl::DecodeReserved));
}
