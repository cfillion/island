#ifndef VIEWPORT_HPP

#include <QWidget>

class Page;
class QHBoxLayout;

class Viewport : public QWidget {
  Q_OBJECT

public:
  Viewport(QWidget *parent = 0);
  void addPage(Page *page);

private:
  QHBoxLayout *m_layout;
};

#endif
