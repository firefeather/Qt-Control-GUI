#ifndef IISTASKHEADER_H
#define IISTASKHEADER_H

#include <QtGui>

#include "iistaskpanel_global.h"

class iisTaskPanelScheme;
struct iisIconLabelScheme;
class iisIconLabel;

class IISTASKPANEL_EXPORT iisTaskHeader : public QFrame
{
  Q_OBJECT

public:
  iisTaskHeader(const QIcon &icon, const QString &title, bool expandable, QWidget *parent = 0);
  virtual ~iisTaskHeader();

  void setScheme(iisTaskPanelScheme *scheme);

signals:
  void activated();

public slots:
  void fold();

protected slots:
  void animate();

protected:
  virtual void paintEvent ( QPaintEvent * event );
  virtual void enterEvent ( QEvent * event );
  virtual void leaveEvent ( QEvent * event );
  virtual void mouseReleaseEvent ( QMouseEvent * event );
  virtual void keyPressEvent ( QKeyEvent * event );
  virtual void keyReleaseEvent ( QKeyEvent * event );

  bool eventFilter(QObject *obj, QEvent *event);

  void changeIcons();

  iisTaskPanelScheme *myScheme;
  iisIconLabelScheme *myLabelScheme;

  bool myExpandable;
  bool m_over, m_buttonOver, m_fold;
  double m_opacity;

  iisIconLabel *myTitle;
  QLabel *myButton;
};

#endif // IISTASKHEADER_H
