#ifndef IISVISTATASKPANELSCHEME_H
#define IISVISTATASKPANELSCHEME_H

#include "iistaskpanelscheme.h"

#include "iistaskpanel_global.h"

class IISTASKPANEL_EXPORT iisVistaTaskPanelScheme : public iisTaskPanelScheme
{
public:
    iisVistaTaskPanelScheme(QObject *parent=0);
    ~iisVistaTaskPanelScheme();

  static iisTaskPanelScheme* defaultScheme();

protected:
  static iisVistaTaskPanelScheme *myDefaultVistaScheme;
};

#endif // IISVISTATASKPANELSCHEME_H
