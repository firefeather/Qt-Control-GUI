#include "iistaskgroup.h"
#include "iistaskpanelscheme.h"
#include "iisiconlabel.h"

iisTaskGroup::iisTaskGroup(QWidget *parent, bool hasHeader)
  : QFrame(parent),
  myHasHeader(hasHeader)
{
  setScheme(iisTaskPanelScheme::defaultScheme());

  QVBoxLayout *vbl = new QVBoxLayout();
  vbl->setMargin(4);
  vbl->setSpacing(0);
  setLayout(vbl);
}

iisTaskGroup::~iisTaskGroup()
{
}

void iisTaskGroup::setScheme(iisTaskPanelScheme *scheme)
{
  if (scheme) {
    myScheme = scheme;
    myLabelScheme = &(scheme->taskLabelScheme);
    update();
  }
}

void iisTaskGroup::addIconLabel(iisIconLabel *label, bool addToLayout)
{
  if (!label) return;

  if (addToLayout) {
    QHBoxLayout *hbl = new QHBoxLayout();
    hbl->setMargin(0);
    hbl->setSpacing(0);
    hbl->addWidget(label);
    hbl->addStretch();

    groupLayout()->addLayout(hbl);
    //layout()->addWidget(label);
  }

  label->setSchemePointer(&myLabelScheme);
}

QPixmap iisTaskGroup::transparentRender()
{
  QPixmap pm(size());
  pm.fill(Qt::transparent);

  render(&pm, QPoint(0,0), rect(), DrawChildren | IgnoreMask);

  return pm;

//  pixmap = QPixmap::grabWidget(this, rect());
//
//  if (!myScheme->groupBackground.isOpaque())
//    pixmap.setMask(pixmap.createHeuristicMask());
}

void iisTaskGroup::paintEvent ( QPaintEvent * event )
{
  QPainter p(this);

  //p.setOpacity(/*m_opacity+*/0.7);
  //p.fillRect(rect(), myScheme->groupBackground);

//  if (!myScheme->groupBackground.isOpaque())
//    p.setBrush(QColor(255,255,255,1));
//  else
//
  p.setBrush(myScheme->groupBackground);

  p.setPen(myScheme->groupBorder);
  p.drawRect(rect().adjusted(0,-(int)myHasHeader,-1,-1));
}

void iisTaskGroup::keyPressEvent ( QKeyEvent * event )
{
  switch (event->key())
  {
    case Qt::Key_Down:
    {
      QKeyEvent ke(QEvent::KeyPress, Qt::Key_Tab, 0);
      QApplication::sendEvent(this, &ke);
      return;
    }

    case Qt::Key_Up:
    {
      QKeyEvent ke(QEvent::KeyPress, Qt::Key_Tab, Qt::ShiftModifier);
      QApplication::sendEvent(this, &ke);
      return;
    }

    default:;
  }

  QWidget::keyPressEvent(event);
}


void iisTaskGroup::keyReleaseEvent ( QKeyEvent * event )
{
  switch (event->key())
  {
    case Qt::Key_Down:
    {
      QKeyEvent ke(QEvent::KeyRelease, Qt::Key_Tab, 0);
      QApplication::sendEvent(this, &ke);
      return;
    }

    case Qt::Key_Up:
    {
      QKeyEvent ke(QEvent::KeyRelease, Qt::Key_Tab, Qt::ShiftModifier);
      QApplication::sendEvent(this, &ke);
      return;
    }

    default:;
  }

  QWidget::keyReleaseEvent(event);
}
