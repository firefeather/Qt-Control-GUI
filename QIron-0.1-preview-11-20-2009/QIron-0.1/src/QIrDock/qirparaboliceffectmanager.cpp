#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include "qirdock.h"
#include "qirdocklet.h"
#include "qirdockstyle.h"
#include "qirdockview.h"
#include "qgraphicseffect.h"
#include "qirdockletbundle.h"
#include "private/qirparaboliceffectmanager_p.h"
#include "../Common/private/qirobject_p.h"
#include "../Common/qirboolblocker.h"
#include <QPointer>

#define DOCKLET_JUMP_HEIGHT 20

QIR_BEGIN_NAMESPACE

////////////////////////////
//QIrParabolicEffectManager
///////////////////////////
QIrParabolicEffectManager::QIrParabolicEffectManager( QObject * parent ) : QIrDockEffectManager(parent,* new QIrParabolicEffectManagerPrivate)
{
	QIR_P(QIrParabolicEffectManager);

	p->setup();
}
QIrParabolicEffectManager::~QIrParabolicEffectManager()
{
}
void QIrParabolicEffectManager::polish( QIrDockletBundle * bundle )
{
	QIR_P(QIrParabolicEffectManager);

	if ( !p->geomAnimations.contains(bundle) ) {
		QPropertyAnimation * animGeom = new QPropertyAnimation(bundle,"geometry");
		QPropertyAnimation * animOffset = new QPropertyAnimation(bundle,"dockletOffset",this);

		animGeom->setDuration(250);
		animGeom->setEasingCurve(QEasingCurve::OutCubic);
		p->geomAnimations[bundle] = animGeom;
		p->bundleAnimationGroup->addAnimation(animGeom);
		
		animOffset->setDuration(750);
		animOffset->setEasingCurve(QEasingCurve::InOutQuad);
		p->offsetAnimations[bundle] = animOffset;

		connect(bundle,SIGNAL(geometryChanged()),p->view->dock(),SLOT(slotUpdateGeometry()));
		connect(bundle,SIGNAL(clicked()),this,SLOT(slotHandleDockletClicked()));
	}
	invalidate();
}
void QIrParabolicEffectManager::unpolish( QIrDockletBundle * bundle )
{
	QIR_P(QIrParabolicEffectManager);
	QPropertyAnimation * geomAnim = p->geomAnimations.take(bundle);
	QPropertyAnimation * offsetAnim = p->offsetAnimations.take(bundle);
	
	disconnect(bundle,SIGNAL(geometryChanged()),p->view->dock(),SLOT(slotUpdateGeometry()));
	disconnect(bundle,SIGNAL(clicked()),this,SLOT(slotHandleDockletClicked()));

	p->bundleAnimationGroup->removeAnimation(geomAnim);
	delete geomAnim;
	delete offsetAnim;
	if ( p->hover == bundle ) {
		//p->useAnimation = false;
	//	p->hover = 0;
	}
	p->updateMinDockletRects();
}
QRectF QIrParabolicEffectManager::contentRect() const
{
	QRectF rect;

	foreach (QIrDockletBundle * docklet, view()->bundles()) {
		rect |= docklet->geometry();
	}
	return rect;
}
QRectF QIrParabolicEffectManager::normalContentRect() const
{
	QIR_P(const QIrParabolicEffectManager);
	QRectF rect;

	const_cast< QIrParabolicEffectManagerPrivate * >(p)->updateMinDockletRects();

	foreach (QRectF r, p->minDockletRects)
		rect |= r;
	return rect;
}
QRectF QIrParabolicEffectManager::extendContentRect() const
{
	QIR_P(const QIrParabolicEffectManager);
	QIrDock * dock = p->view->dock();
	QIrDockStyle * style = dock->dockStyle();
	int spacing = p->view->dockletSpacing();
	QSize sz;
	int ret;

	if ( !style )
		return QRectF();

	sz = sizeForIconSize(style,spacing,dock->effectiveMinimumIconSize(),dock->maximumIconSize(),dock->maximumIconSize(),false,dock->orientation());
	ret = (dock->orientation() == Qt::Horizontal ? sz.height() : sz.width()) + DOCKLET_JUMP_HEIGHT;
	
	return QRectF(QPoint(),QSize(ret,ret));
}
void QIrParabolicEffectManager::invalidate()
{
	QIR_P(QIrParabolicEffectManager);

	p->updateMinDockletRects();
	p->updateDockletRects();
	doLayout();
}
QSize QIrParabolicEffectManager::sizeHint() const
{
	QIR_P(const QIrParabolicEffectManager);
	QIrDockView * view = p->view;
	QList< QIrDockletBundle * > list = view->bundles();
	QIrDock * dock = view->dock();
	QRectF bounding;
	QRectF geom;
	int sepCount = 0, count = list.count();
	QIrDockStyle * style = dock->dockStyle();
	int spacing = p->view->dockletSpacing();

	if ( !style )
		return QSize();

	bounding = contentRect();
	for (int i = 0; i < count; i++) {
		if ( list[i]->docklet()->isSeparator() )
			sepCount++;
	}
	QSize s = bounding.size().toSize();
	
	if ( !dock->isExtended() ) {
		QSize mindockletSize = sizeForIconSize(style,spacing,dock->effectiveMinimumIconSize(),dock->minimumIconSize(),dock->maximumIconSize(),false,dock->orientation());
		QSize minSepSize = sizeForIconSize(style,spacing,dock->effectiveMinimumIconSize(),dock->minimumIconSize(),
			dock->maximumIconSize(),true,dock->orientation());

		if ( dock->orientation() == Qt::Horizontal ) {
			s.rwidth() = (count - sepCount) * (mindockletSize.width() + 1) + sepCount * (minSepSize.width() + 1) - 1;
		} else {
			s.rheight() = (count - sepCount) * (mindockletSize.height() + 1) + sepCount * (minSepSize.height() + 1) - 1;
		}
	}	
	return s;
}
void QIrParabolicEffectManager::enterEvent( QEvent * )
{
}
void QIrParabolicEffectManager::leaveEvent( QEvent * )
{
	QIR_P(QIrParabolicEffectManager);

	p->evaluatePos(QPoint(-10000,-10000));
}
void QIrParabolicEffectManager::mouseMoveEvent( QMouseEvent * evt )
{
	QIR_P(QIrParabolicEffectManager);
	QIrDockView * view = this->view();
	QIrDock * dock = view->dock();
	static QPoint lastCursorPos;
	int delta = 0;
	static int threshold = 10;
	static bool lastTransitState;

	if ( lastTransitState != p->isTransiting ) {
		lastCursorPos = evt->globalPos();
		lastTransitState = p->isTransiting;
	}
	if ( dock->orientation() == Qt::Horizontal )
		delta = lastCursorPos.x() - evt->globalPos().x();
	else
		delta = lastCursorPos.y() - evt->globalPos().y();
	if (p->bundleAnimationGroup->state() == QAbstractAnimation::Running && p->isExtending && qAbs(delta) > threshold ) {
		p->isAnimationInterrupted = true;
		p->bundleAnimationGroup->stop();
	}
	p->evaluatePos(evt->pos());
}
void QIrParabolicEffectManager::mousePressEvent( QMouseEvent * )
{
}
void QIrParabolicEffectManager::mouseReleaseEvent( QMouseEvent * )
{
}
void QIrParabolicEffectManager::clearHover()
{	
	QIR_P(QIrParabolicEffectManager);

	if ( p->hover && p->hover->docklet() ) {
		p->isHoverMode = false;
		changeHoverDocklet(0,p->hover->docklet());
		p->hover = 0;
	}
}
void QIrParabolicEffectManager::reset()
{
	QIR_P(QIrParabolicEffectManager);

	QIR_NO_RECURSIVE(p->restoring,(void)0);
	
	clearHover();
	p->updateMinDockletRects();
	p->restoreSizes();
	invalidate();	
}
void QIrParabolicEffectManager::slotHandleAnimationFinished()
{
	QIR_P(QIrParabolicEffectManager);

	p->isTransiting = false;
	if ( !p->isHoverMode ) {
		emit normalizeRequested();
	}
	p->view->dock()->slotUpdateGeometry();
}
void QIrParabolicEffectManager::slotHandleAnimationStateChanged()
{
	QIR_P(QIrParabolicEffectManager);

	if ( p->bundleAnimationGroup->state() != QAbstractAnimation::Stopped )
		return;
	p->isTransiting = false;
	p->isAnimationInterrupted = false;
}
int QIrParabolicEffectManager::calculateReach() const
{
	QIR_P(const QIrParabolicEffectManager);
	QIrDock * dock = p->view->dock();
	QIrDockStyle * style = dock->dockStyle();
	qreal distance = 0;
	QSizeF w(0,0);
	int spacing = p->view->dockletSpacing();
	
	QList< QIrDockletBundle * > bundles = p->view->bundles();

	
	QSizeF minR = sizeForIconSize(style,spacing,dock->effectiveMinimumIconSize(),dock->effectiveMinimumIconSize(),dock->maximumIconSize(),true,dock->orientation());
        int minS = (int)(dock->orientation() == Qt::Horizontal ? minR.width() : minR.height());

        w += sizeForIconSize(style,spacing,dock->effectiveMinimumIconSize(),calculateIconSize((int)distance),dock->maximumIconSize(),true,dock->orientation()) / 2.0;
	for (int i = 1; i <= p->coverage; i++) {
	
		distance += minS;
                w += sizeForIconSize(style,spacing,dock->effectiveMinimumIconSize(),calculateIconSize((int)distance),dock->maximumIconSize(),true,dock->orientation());
	}
        return (int)(dock->orientation() == Qt::Horizontal ? w.width() : w.height());

}
int QIrParabolicEffectManager::calculateIconSize( int delta ) const
{
	QIR_P(const QIrParabolicEffectManager);


	QIrDock * dock = view()->dock();
        int dc = (int)p->calculateCoverage();
	int min = dock->effectiveMinimumIconSize();
	int max = dock->maximumIconSize();
	int iconSize;
	if (( delta < -dc ) || ( delta > dc ))
        iconSize = min;
    else
		iconSize = (max - (delta * delta * (max - min)) / (dc * dc));
	return iconSize;
}
void QIrParabolicEffectManager::doLayout()
{
	QIR_P(QIrParabolicEffectManager);

	if ( !p->isTransiting ) {
		QList< QIrDockletBundle * > list = view()->bundles();
		int count = list.count();

		for (int i = 0; i < count; i++) {
			list[i]->setGeometry(p->dockletRects[i]);
		}
	}
	emit updateGeometryRequested();
	p->view->adjustLayers();
}
int QIrParabolicEffectManager::closestLegalIconSize( const QSize & s ) const
{
	QIR_P(const QIrParabolicEffectManager);
	int spacing = p->view->dockletSpacing();
	int c = p->view->bundles().count();
	QIrDock * dock = p->view->dock();
	QIrDockStyle * style = dock->dockStyle();

	if ( !style || c == 0 )
		return 8;
	if ( dock->orientation() == Qt::Horizontal )
		return style->iconSizeForSize(QSize(s.width() / c - spacing,s.height()),false,Qt::Horizontal);
	else
		return style->iconSizeForSize(QSize(s.width(), s.height() / c - spacing),false,Qt::Vertical);
}
int QIrParabolicEffectManager::dockletCoverage() const
{
	QIR_P(const QIrParabolicEffectManager);

	return (p->coverage < 1 ? 1 : p->coverage > 10 ? 10 : p->coverage);
}
void QIrParabolicEffectManager::setDockletCoverage( int dc )
{
	QIR_P(QIrParabolicEffectManager);

	p->coverage = dc; 
}
void QIrParabolicEffectManager::changeHoverDocklet(QIrDocklet * current, QIrDocklet * previous)
{
	QIR_P(QIrParabolicEffectManager);
	QIrDock * dock = p->view->dock();
	bool noEffect = (dock->effectiveMinimumIconSize() == dock->maximumIconSize()); 

	if ( !noEffect ) {
		if ( p->view->dock()->showTipMode() != QIrDock::NonSpontaneousShowTip )
			p->view->dock()->setShowTipMode(QIrDock::NonSpontaneousShowTip);
	} else {
		if ( p->view->dock()->showTipMode() != QIrDock::SpontaneousShowTip )
			p->view->dock()->setShowTipMode(QIrDock::SpontaneousShowTip);
	}


	hoverDockletChanged(current,previous);
	if ( !current || !previous ) {
		if ( current ) {
			emit extendRequested();
		} 
		if ( !p->useAnimation ) {
			if ( !current ) {
				emit normalizeRequested();
			}
			p->mustStartAnimation = false;
		} else 
			p->mustStartAnimation = true;
	}
}
void QIrParabolicEffectManager::slotHandleDockletClicked()
{
	QIR_P(QIrParabolicEffectManager);
	QIrDock * dock = p->view->dock();
	QIrDockletBundle * bundle = qobject_cast< QIrDockletBundle * >(sender());

	if ( !bundle )
		return;
	QPropertyAnimation * animation = p->offsetAnimations[bundle];

	animation->setStartValue(QPointF(0,0));
	switch ( dock->dockArea() ) {
		case QIrAbstractDock::DA_BottomDockArea :
			animation->setKeyValueAt(0.5,QPointF(0,-20));
			break;
		case QIrAbstractDock::DA_TopDockArea :
			animation->setKeyValueAt(0.5,QPointF(0,20));
			break;
		case QIrAbstractDock::DA_LeftDockArea :
			animation->setKeyValueAt(0.5,QPointF(20,0));
			break;
		default :
			animation->setKeyValueAt(0.5,QPointF(-20,0));
			break;
	}
	animation->setEndValue(QPointF(0,0));
	animation->start();
}
QIR_END_NAMESPACE
