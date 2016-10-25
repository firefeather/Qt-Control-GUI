#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QtGui/QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QIcon>
#include <QAction>

class SideBar : public QWidget
{
    Q_OBJECT

public:
    SideBar(QWidget *parent = 0);
    ~SideBar();

    void addAction(QAction *action);
    QAction *addAction(const QString &text, const QIcon &icon = QIcon());

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    QSize minimumSizeHint() const;

    QAction* actionAt(const QPoint &at);
private:
    QList<QAction*> _actions;
    QAction *_pressedAction;
    QAction *_checkedAction;
};

#endif // SIDEBAR_H
