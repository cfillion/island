#include "statusbar.hpp"

#include "page.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QUrl>

StatusBar::StatusBar(QWidget *parent)
  : QWidget(parent), m_page(0), m_pageCount(0)
{
  setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);

  m_url = new QLabel;
  m_url->setAlignment(Qt::AlignRight);
  m_url->setTextFormat(Qt::PlainText);

  m_tabPosition = new QLabel;

  m_progress = new QProgressBar;
  m_progress->setTextVisible(false);
  m_progress->setMaximumHeight(8);
  m_progress->hide();

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setSpacing(5);
  layout->setContentsMargins(QMargins());
  layout->addWidget(m_url);
  layout->addWidget(m_tabPosition);
  layout->addWidget(m_progress);
  layout->setStretch(0, 2);
}

void StatusBar::setPage(Page *p)
{
  if(m_page) {
    m_page->disconnect(this);
    m_page->disconnect(m_progress);
  }

  m_page = p;
  connect(m_page, &Page::urlChanged, this, &StatusBar::updateLabels);

  connect(m_page, &Page::loadStarted, m_progress, &QProgressBar::show);
  connect(m_page, &Page::loadStarted, m_progress, &QProgressBar::reset);
  connect(m_page, &Page::loadProgress, m_progress, &QProgressBar::setValue);
  connect(m_page, &Page::loadFinished, m_progress, &QProgressBar::hide);

  updateLabels();
}

void StatusBar::updateLabels()
{
  if(!m_page)
    return;

  const QUrl url = m_page->url();
  m_url->setText(url.toString(QUrl::RemoveUserInfo | QUrl::DecodeReserved));

  const int pageId = m_page->index() + 1;
  m_tabPosition->setText(QString("[%0/%2]").arg(pageId).arg(m_pageCount));
}

void StatusBar::setPageCount(const int pageCount)
{
  m_pageCount = pageCount;
  updateLabels();
}

void StatusBar::showProgressBar()
{
  m_progress->setValue(0);
  m_progress->show();
}
