#ifndef ISLAND_ENGINE_HPP
#define ISLAND_ENGINE_HPP

#include <QUrl>
#include <QWebEngineView>

class Engine : public QWebEngineView {
  Q_OBJECT

public:
  Engine(const QUrl &url, QWidget *parent = 0);
  QUrl url() const;

  bool historyMotion(const int);

Q_SIGNALS:
  void triggered();
  void linkHovered(const QString &);

protected:
  void showEvent(QShowEvent *) override;
  void childEvent(QChildEvent *) override;
  bool eventFilter(QObject *, QEvent *) override;

private:
  QUrl m_deferredUrl;
};

#endif
