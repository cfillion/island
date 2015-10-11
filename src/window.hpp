#ifndef ISLAND_WINDOW_HPP
#define ISLAND_WINDOW_HPP

#include <array>
#include <QTimer>
#include <QWidget>

#include "buffer.hpp"
#include "global.hpp"
#include "keypress.hpp"
#include "mapping.hpp"

class Mapping;
class Page;
class Prompt;
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

  explicit Window(const MappingArray &mappings, QWidget *parent = 0);

  Prompt *prompt() const;

  int addPage(const QString &url, const OpenMode mode = NewTab);
  void setCurrentPage(Page *);
  void setCurrentTab(const int index);
  Page *currentPage() const { return m_current; }
  int currentPageIndex() const;
  int pageCount() const { return m_pages.size(); }
  Page *page(const int i) const { return m_pages.value(i); }
  void closeTab(const int index);
  void closePage(Page *page);

  bool handleInput(const KeyPress &);
  void simulateInput(const Buffer &);

  void setMode(const Island::Mode);

Q_SIGNALS:
  void bufferChanged(const Buffer &);
  void modeChanged(const Island::Mode);

private:
  void currentTabMotion(const bool polarity, const int size);
  void shiftPageIndexes(const int start = 0);
  void execPrompt(const QString &);
  void execDelayedMapping();
  void execMapping(const Mapping *);
  void execCommand(Command &);
  void clearBuffer();

  Page *m_current;
  QList<Page *> m_pages;
  QStackedLayout *m_stack;
  TabBar *m_tabs;
  StatusBar *m_status;

  Island::Mode m_mode;
  MappingArray m_mappings;
  Buffer m_buffer;
  Mapping *m_delayedMapping;
  QTimer m_mappingTimer;
};

#endif
