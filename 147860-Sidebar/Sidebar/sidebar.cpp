#include "sidebar.h"

int action_height = 90;

SideBar::SideBar(QWidget *parent)
    : QWidget(parent), _pressedAction(NULL), _checkedAction(NULL)
{
    setFixedWidth(90);
}

SideBar::~SideBar()
{

}

void SideBar::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    QFont fontText(p.font());
    fontText.setFamily("Helvetica Neue");
    p.setFont(fontText);

    //Adapter l'adresse de la texture ici
    QImage texture("path/to/texture.png");
    p.fillRect(event->rect(), QBrush(texture));
    p.setPen(Qt::black);
    p.drawLine(event->rect().topRight(), event->rect().bottomRight());

    int actions_height = _actions.size()*action_height;

    int action_y = event->rect().height()/2-actions_height/2;
    foreach(QAction *action, _actions)
    {
        QRect actionRect(0, action_y, event->rect().width(), action_height);

        if(action->isChecked())
        {
            p.setOpacity(0.5);
            p.fillRect(actionRect, QColor(19, 19, 19));
            p.setPen(QColor(9, 9, 9));
            p.drawLine(actionRect.topLeft(), actionRect.topRight());
            p.setOpacity(1);

        }

        if(action == _actions.last())
        {
            p.setPen(QColor(15, 15, 15));
            p.drawLine(QPoint(0, actionRect.bottomLeft().y()-1), QPoint(actionRect.width(), actionRect.bottomRight().y()-1));
            p.setPen(QColor(55, 55, 55));
            p.drawLine(actionRect.bottomLeft(), actionRect.bottomRight());

        }

        if(!action->isChecked())
        {
            p.setPen(QColor(15, 15, 15));
            p.drawLine(actionRect.topLeft(), actionRect.topRight());
            p.setPen(QColor(55, 55, 55));
            p.drawLine(QPoint(0, actionRect.topLeft().y()+1), QPoint(actionRect.width(), actionRect.topRight().y()+1));
        }
        int icon_size = 48;

        QRect actionIconRect(0, action_y, event->rect().width(), action_height-20);
        QIcon  actionIcon(action->icon());
        actionIcon.paint(&p, actionIconRect);


        p.setPen(QColor(217, 217, 217));
        if(action->isChecked())
            p.setPen(QColor(255, 255, 255));
        QRect actionTextRect(0, action_y+actionRect.height()-20, event->rect().width(), 15);
        p.drawText(actionTextRect, Qt::AlignCenter, action->text());

        action_y += actionRect.height();
    }

}

QSize SideBar::minimumSizeHint() const
{
    return QSize(90, _actions.size()*action_height);
}

void SideBar::addAction(QAction *action)
{
    _actions.push_back(action);
    action->setCheckable(true);
    update();
    return;
}

QAction *SideBar::addAction(const QString &text, const QIcon &icon)
{
    QAction *action = new QAction(icon, text, this);
    action->setCheckable(true);
    _actions.push_back(action);
    update();
    return action;
}

void SideBar::mousePressEvent(QMouseEvent *event)
{
    _pressedAction = actionAt(event->pos());
    if(_pressedAction == NULL || _pressedAction == _checkedAction)
        return;
    update();
}

void SideBar::mouseReleaseEvent(QMouseEvent *event)
{
    QAction* tempAction = actionAt(event->pos());
    if(_pressedAction != tempAction || tempAction == NULL)
    {
        _pressedAction = NULL;
        return;
    }
    if(_checkedAction != NULL)
        _checkedAction->setChecked(false);
    _checkedAction = _pressedAction;
    if(_checkedAction != NULL)
        _checkedAction->setChecked(true);
    update();
    _pressedAction = NULL;
    return;
}

QAction* SideBar::actionAt(const QPoint &at)
{
    int actions_height = _actions.size()*action_height;

    int action_y = rect().height()/2-actions_height/2;
    foreach(QAction *action, _actions)
    {
        QRect actionRect(0, action_y, rect().width(), action_height);
        if(actionRect.contains(at))
            return action;
        action_y += actionRect.height();
    }
    return NULL;
}
