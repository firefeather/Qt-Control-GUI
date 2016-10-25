#include "iistaskpanel.h"
#include "iistaskpanelscheme.h"
#include "iistaskbox.h"
#include "iistaskgroup.h"

iisTaskPanel::iisTaskPanel(QWidget *parent) :
  QWidget(parent)
{
  myScheme = iisTaskPanelScheme::defaultScheme();

  setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

  QVBoxLayout *vbl = new QVBoxLayout();
  vbl->setMargin(8);
  vbl->setSpacing(8);
  setLayout(vbl);
}

iisTaskPanel::~iisTaskPanel()
{
}

void iisTaskPanel::setScheme(iisTaskPanelScheme *scheme)
{
  if (scheme) {
    myScheme = scheme;

    // set scheme for children
    QObjectList list(children());
    foreach(QObject *obj, list) {
      if (dynamic_cast<iisTaskBox*>(obj)) {
        ((iisTaskBox*)obj)->setScheme(scheme);
        continue;
      }

      if (dynamic_cast<iisTaskGroup*>(obj)) {
        ((iisTaskGroup*)obj)->setScheme(scheme);
        continue;
      }
    }

    update();
  }
}

void iisTaskPanel::paintEvent ( QPaintEvent * event )
{
  QPainter p(this);

  //p.setOpacity(0.5);
  p.fillRect(rect(), myScheme->panelBackground);
}

void iisTaskPanel::addWidget(QWidget *w)
{
  if (w)
    layout()->addWidget(w);
}

void iisTaskPanel::addStretch(int s)
{
  ((QVBoxLayout*)layout())->addStretch(s);
}

