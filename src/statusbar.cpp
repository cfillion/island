#include "statusbar.hpp"

#include "page.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QResizeEvent>
#include <QUrl>

StatusBar::StatusBar(QWidget *parent)
  : QWidget(parent), m_page(0), m_pageCount(0)
{
  setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
  setFixedHeight(16);

  m_status = new QLabel;
  m_buffer = new QLabel;

  m_status->setText("-- INSERT --");

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
  layout->setContentsMargins(5, 0, 2, 0);
  layout->addWidget(m_status);
  layout->addWidget(m_buffer);
  layout->addWidget(m_url);
  layout->addWidget(m_tabPosition);
  layout->addWidget(m_progress);
  layout->setStretch(0, 20);
  layout->setStretch(1, 2);
}

void StatusBar::setPage(Page *p)
{
  if(m_page)
    m_page->disconnect(this);

  m_page = p;
  connect(m_page, &Page::displayUrlChanged, this, &StatusBar::updateLabels);
  connect(m_page, &Page::progressChanged, this, &StatusBar::updateLabels);

  updateLabels();
}

void StatusBar::updateLabels()
{
  // this can be called from setPageCount before the current page is set
  if(!m_page)
    return;

  m_url->setText(m_page->displayUrl());

  const int pageId = m_page->index() + 1;
  m_tabPosition->setText(QString("[%0/%2]").arg(pageId).arg(m_pageCount));

  m_progress->setVisible(m_page->isLoading());
  m_progress->setValue(m_page->loadProgress());
}

void StatusBar::setPageCount(const int pageCount)
{
  m_pageCount = pageCount;
  updateLabels();
}

void StatusBar::setBuffer(const QString &text)
{
  m_buffer->setText(text.right(10));
}

void StatusBar::resizeEvent(QResizeEvent *e)
{
  const int width = e->size().width();
  const bool showAll = width > 150;

  m_url->setVisible(width > 400);
  m_buffer->setVisible(showAll);
  m_tabPosition->setVisible(showAll);
}
