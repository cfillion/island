#include "statusbar.hpp"

#include "page.hpp"
#include "prompt.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QResizeEvent>
#include <QUrl>

using namespace Island;

StatusBar::StatusBar(QWidget *parent)
  : QWidget(parent), m_page(0), m_pageCount(0)
{
  setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
  setFixedHeight(16);

  m_mode = new QLabel;
  m_status = new QLabel;
  m_status->setTextFormat(Qt::PlainText);

  m_buffer = new QLabel;
  m_prompt = new Prompt;

  m_url = new QLabel;
  m_url->setAlignment(Qt::AlignRight);
  m_url->setTextFormat(Qt::PlainText);

  m_pageState = new QLabel;

  m_progress = new QProgressBar;
  m_progress->setTextVisible(false);
  m_progress->setMaximumHeight(8);
  m_progress->hide();

  connect(m_prompt, &Prompt::editingFinished, this, &StatusBar::promptFinished);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setSpacing(5);
  layout->setContentsMargins(5, 0, 2, 0);
  layout->addWidget(m_mode);
  layout->addWidget(m_status);
  layout->addWidget(m_prompt);
  layout->addWidget(m_buffer);
  layout->addWidget(m_url);
  layout->addWidget(m_pageState);
  layout->addWidget(m_progress);
  layout->setStretch(1, 20);
  layout->setStretch(2, 20);
  layout->setStretch(3, 2);
}

void StatusBar::setPage(Page *p)
{
  if(m_page)
    m_page->disconnect(this);

  m_page = p;
  connect(m_page, &Page::displayUrlChanged, this, &StatusBar::updateLabels);
  connect(m_page, &Page::progressChanged, this, &StatusBar::updateLabels);
  connect(m_page, &Page::indexChanged, this, &StatusBar::updateLabels);

  updateLabels();
}

void StatusBar::updateLabels()
{
  // this can be called from setPageCount before the current page is set
  if(!m_page)
    return;

  const QFontMetrics metrics(m_url->font());
  m_url->setText(metrics.elidedText(m_page->displayUrl(),
    Qt::ElideMiddle, width() / 2));

  m_pageState->setText(currentStateText());

  m_progress->setVisible(m_page->isLoading());
  m_progress->setValue(m_page->loadProgress());
}

QString StatusBar::currentStateText() const
{
  QString history;
  if(m_page->canGoBack())
    history.append('+');
  if(m_page->canGoForward())
    history.append('-');

  const int pageId = m_page->index() + 1;
  const QString pageIndex = QString("%0/%2").arg(pageId).arg(m_pageCount);

  QStringList components = QStringList()
    << history
    << pageIndex
  ;

  components.removeAll(QString());

  return QString("[%1]").arg(components.join("] ["));
}

void StatusBar::setPageCount(const int pageCount)
{
  m_pageCount = pageCount;
  updateLabels();
}

void StatusBar::setBuffer(const Buffer &buffer)
{
  const QString text = buffer.toString();
  m_buffer->setText(text.right(16));
}

void StatusBar::setMode(const Mode mode)
{
  if(mode < CommandMode) {
    m_mode->show();
    m_status->show();

    m_prompt->blockSignals(true);
    m_prompt->hide();
    m_prompt->blockSignals(false);
  }
  else {
    m_mode->hide();
    m_status->hide();
    m_prompt->show();
    m_prompt->setFocus();
  }

  switch(mode) {
  case NormalMode:
    m_mode->setText("-- NORMAL --");
    break;
  case InsertMode:
    m_mode->setText("-- INSERT --");
    break;
  case CommandMode:
    m_prompt->setPrompt(":", true);
    break;
  case SearchForwardMode:
    m_prompt->setPrompt("/");
    break;
  case SearchBackwardMode:
    m_prompt->setPrompt("?");
    break;
  }
}

void StatusBar::setStatus(const QString &text)
{
  m_status->setText(text);
}

void StatusBar::resizeEvent(QResizeEvent *e)
{
  const int width = e->size().width();
  const bool showAll = width > 150;

  m_status->setVisible(width > 500);
  m_url->setVisible(width > 400);
  m_buffer->setVisible(showAll);
  m_pageState->setVisible(showAll);

  updateLabels();
}
