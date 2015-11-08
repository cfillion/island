#include "completer.hpp"

#include "command.hpp"

#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>

using namespace CommandOptions;

Completer::Completer(QWidget *parent)
  : QListWidget(parent)
{
  setAttribute(Qt::WA_MacShowFocusRect, false);
  setFocusPolicy(Qt::NoFocus);
  setFrameShape(QFrame::NoFrame);
  setFlow(QListView::LeftToRight);

  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  connect(this, &Completer::currentTextChanged, this, &Completer::replaceWord);

  hide();
}

QSize Completer::sizeHint() const
{
  QSize size(QListWidget::sizeHint());
  size.setHeight(sizeHintForRow(0));
  return size;
}

void Completer::trigger(const QString &leftInput)
{
  const CommandParser parser(leftInput);

  if(isVisible() || !parser.isValid())
    return;

  if(parser.nameEnd() == leftInput.size())
    completeName(parser);
  else
    completeArgument(parser);

  const int size = count();

  if(size == 1) {
    replaceWord(item(0)->text());
    clear();
  }
  else if(size > 1) {
    show();
    Q_EMIT triggered();
  }
}

void Completer::completeName(const CommandParser &parser)
{
  m_startIndex = parser.nameStart();
  m_endIndex = parser.nameEnd();

  const auto list = Command::findCommands(parser.name());

  for(const CommandEntry *entry : list) {
    addItem(entry->name);

    if(entry->hasFlag(EN_FORCE))
      addItem(entry->name + "!");
  }
}

void Completer::completeArgument(const CommandParser &parser)
{
  m_startIndex = parser.argumentStart();
  m_endIndex = parser.argumentEnd();
}

void Completer::dismiss()
{
  if(!isVisible())
    return;

  // don't emit wordChanged() when clearing the list
  blockSignals(true);
  clear();
  blockSignals(false);

  hide();
}

void Completer::move(const int movement)
{
  if(!isVisible())
    return;

  const int size = count();
  const int row = (currentRow() + movement) % size;

  setCurrentRow(row > -1 ? row : size - 1);
}

void Completer::replaceWord(const QString &word)
{
  Q_EMIT wordChanged(m_startIndex, m_endIndex, word);
  m_endIndex = m_startIndex + word.size();
}

bool Completer::event(QEvent *e)
{
  if(e->type() == QEvent::Show || e->type() == QEvent::WindowIconChange)
    raise();

  return QListWidget::event(e);
}
