#include "completer.hpp"

#include "command.hpp"

#include <QEvent>
#include <QHBoxlayout>
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

  connect(this, &Completer::currentTextChanged, this, &Completer::wordChanged);

  hide();
}

QSize Completer::sizeHint() const
{
  QSize size(QListWidget::sizeHint());
  size.setHeight(sizeHintForRow(0));
  return size;
}

void Completer::trigger(const int index, const QString &word)
{
  if(isVisible() || index != 0)
    return;

  const auto list = Command::findCommands(word);

  for(const CommandEntry &entry : list) {
    addItem(entry.name);

    if(entry.hasFlag(EN_FORCE))
      addItem(entry.name + "!");
  }

  const int size = count();

  if(size == 1) {
    Q_EMIT wordChanged(item(0)->text());
    clear();
  }
  else if(size > 1) {
    show();
    Q_EMIT triggered();
  }
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

bool Completer::event(QEvent *e)
{
  if(e->type() == QEvent::Show || e->type() == QEvent::WindowIconChange)
    raise();

  return QListWidget::event(e);
}
