#ifndef IISTASKPANEL_H
#define IISTASKPANEL_H

#include <QtGui>
#include <QtCore>

#include "iistaskpanel_global.h"

class iisTaskPanelScheme;

class IISTASKPANEL_EXPORT iisTaskPanel : public QWidget
{
public:
	iisTaskPanel(QWidget *parent = 0);
	~iisTaskPanel();

	void addWidget(QWidget *w);
	void addStretch(int s = 0);

	void setScheme(iisTaskPanelScheme *scheme);

protected:
	virtual void paintEvent ( QPaintEvent * event );

	iisTaskPanelScheme *myScheme;
};

#endif // IISTASKPANEL_H
