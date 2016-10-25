#include "iisvistataskpanelscheme.h"

iisVistaTaskPanelScheme* iisVistaTaskPanelScheme::myDefaultVistaScheme = 0;

iisVistaTaskPanelScheme::iisVistaTaskPanelScheme(QObject *parent)
  : iisTaskPanelScheme(parent)
{
  QLinearGradient panelBackgroundGrd(0,0, 0,300);
  panelBackgroundGrd.setColorAt(0, 0x416FA6);
  panelBackgroundGrd.setColorAt(1, 0x6BB86E);
  panelBackground = panelBackgroundGrd;

  headerBackground = QBrush(Qt::transparent);
  headerBorder = QColor(Qt::transparent);

  QLinearGradient headerBackgroundGrd(0,0, 0,25);
  headerBackgroundGrd.setColorAt(0, QColor(0xF9,0xFD,0xFF,100));
  headerBackgroundGrd.setColorAt(0.5, QColor(0xEA,0xF7,0xFF,20));
  headerBackgroundGrd.setColorAt(0.51, QColor(0xEA,0xF7,0xFF,20));
  headerBackgroundGrd.setColorAt(1, QColor(0xF9,0xFD,0xFF,100));
  headerBackgroundOver = headerBackgroundGrd;

  //headerBackgroundOver = QBrush(Qt::transparent);

  //headerBorderOver = QColor(0xDA, 0xF2, 0xFC, 100);
  headerBorderOver = headerBorder;

  headerSize = 24;
  headerAnimation = true;

  headerLabelScheme.text = QColor(0xffffff);
  headerLabelScheme.textOver = QColor(0xffffff);
  headerLabelScheme.iconSize = 22;
  headerLabelScheme.font.setBold(false);
  headerLabelScheme.font.setPixelSize(12);

  headerButtonFold = QPixmap(":/Resources/headerButtonFold_Vista1.png");
  headerButtonFoldOver = QPixmap(":/Resources/headerButtonFoldOver_Vista1.png");
  headerButtonUnfold = QPixmap(":/Resources/headerButtonUnfold_Vista1.png");
  headerButtonUnfoldOver = QPixmap(":/Resources/headerButtonUnfoldOver_Vista1.png");
  headerButtonSize = QSize(17,17);

  groupBackground = QBrush(Qt::transparent);
  groupBorder = QColor(Qt::transparent);
  groupBackgroundOver = QBrush(Qt::transparent);
  groupBorderOver = QColor(Qt::transparent);

  groupFoldSteps = 20; groupFoldDelay = 15;
//  groupFoldThaw = false;
  groupFoldEffect = ShrunkFolding;

  taskLabelScheme.text = QColor(0xffffff);
  taskLabelScheme.textOver = QColor(0xDAF2FC);
}

iisVistaTaskPanelScheme::~iisVistaTaskPanelScheme()
{

}

iisTaskPanelScheme* iisVistaTaskPanelScheme::defaultScheme()
{
  if (!myDefaultVistaScheme)
    myDefaultVistaScheme = new iisVistaTaskPanelScheme();

  return myDefaultVistaScheme;
}
