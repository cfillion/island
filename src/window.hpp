#ifndef ISLAND_WINDOW_HPP
#define ISLAND_WINDOW_HPP

#include <QWidget>

#include "global.hpp"

class Mapping;
class Page;
class QStackedLayout;
class StatusBar;
class TabBar;

class Window : public QWidget {
  Q_OBJECT

public:
  enum OpenMode {
    NewTab,
    Split,
  };

  Window(Mapping *mapping, QWidget *parent = 0);

  int addPage(const QUrl &url, const OpenMode mode = NewTab);
  void setCurrentPage(Page *);
  void setCurrentTab(const int index);
  void currentTabMotion(const bool polarity, const int size);
  int currentPageIndex() const;

  bool handleKeyEvent(const QKeyEvent *);

Q_SIGNALS:
  void bufferChanged(const Island::Buffer &);
  void modeChanged(const Island::Mode);

private:
  void shiftPageIndexes(const int start = 0);
  QString keyEventToString(const QKeyEvent *) const;
  void setMode(const Island::Mode);
  void execPrompt(const QString &);

  Island::Mode m_mode;
  Mapping *m_mapping;
  QList<Page *> m_pages;
  QStackedLayout *m_stack;
  TabBar *m_tabs;
  Page *m_current;
  StatusBar *m_status;
  Island::Buffer m_buffer;
};

#endif
