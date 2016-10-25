#ifndef IISTASKPANELSCHEME_H
#define IISTASKPANELSCHEME_H

#include <QtGui>

#include "iistaskpanel_global.h"

#include "iisrect.h"

enum iisTaskPanelFoldEffect
{
  NoFolding,
  ShrunkFolding,
  SlideFolding
};

struct IISTASKPANEL_EXPORT iisIconLabelScheme
{
  QColor text, textOver, textOff;
  QPen focusPen;
  QFont font;
  int iconSize;
  bool underlineOver, cursorOver;
};

class IISTASKPANEL_EXPORT iisTaskPanelScheme : public QObject
{
public:
  iisTaskPanelScheme(QObject *parent = 0);
  ~iisTaskPanelScheme();

  static iisTaskPanelScheme* defaultScheme();

  QBrush panelBackground;

  QBrush headerBackground, headerBackgroundOver;
  iisIconLabelScheme headerLabelScheme;
  QPen headerBorder, headerBorderOver;

  int headerSize;
  bool headerAnimation;
  iisRect headerCorners;

  QIcon headerButtonFold, headerButtonFoldOver, headerButtonUnfold, headerButtonUnfoldOver;
  QSize headerButtonSize;

  QBrush groupBackground, groupBackgroundOver;
  QPen groupBorder, groupBorderOver;
  int groupFoldSteps, groupFoldDelay;
  iisTaskPanelFoldEffect groupFoldEffect;
  bool groupFoldThaw;
  iisIconLabelScheme taskLabelScheme;

protected:
  static iisTaskPanelScheme *myDefaultScheme;
};

#endif // IISTASKPANELSCHEME_H
