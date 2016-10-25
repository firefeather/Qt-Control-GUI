/********************************************************************** 
**
** This file is part of QIron Toolkit.
**
** Copyright (C) 2009-2010 Dzimi Mve Alexandre <dzimiwine@gmail.com>
** 
** Contact: dzimiwine@gmail.com
** 
** QIron is a free toolkit developed in Qt by Dzimi Mve A.; you can redistribute 
** sources and libraries of this library and/or modify them under the terms of 
** the GNU Library General Public License version 3.0 as published by the 
** Free Software Foundation and appearing in the file LICENSE.txt included in 
** the packaging of this file.
** Please review the following information to ensure the GNU General 
** Public License version 3.0 requirements will be met: 
** http://www.gnu.org/copyleft/gpl.html.
**
** This SDK is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.
**
**********************************************************************/
#ifndef QIRBREADCRUMBBAR_UI_H
#define QIRBREADCRUMBBAR_UI_H

#include <QComboBox>
#include "qirabstractbreadcrumbmodel.h"
#include "qirbreadcrumbbar.h"

class QAction;
class QIrBreadCrumbBarStyle;
class QIrBreadCrumbComboBox;
class QIrBreadCrumbComboBoxContainer;
class QIrBreadCrumbLabel;
class QLabel;

class QIrBreadCrumbItem
{
public:
    enum Type {EmptyArea,Indicator,Label};

    QIrBreadCrumbItem(QIrBreadCrumbComboBoxContainer *, QIrBreadCrumbItem::Type);

    inline QRect rect() const { return m_rect; }
    inline void setRect( const QRect & rect ) { m_rect = rect; }

    inline void setVisible( bool visible ) { m_visible = visible; }
    inline bool isVisible() { return m_visible; }

    inline QIrBreadCrumbComboBoxContainer * container() const { return m_container; }
    inline Type type() const { return m_type; }

	virtual void clicked( const QPoint & pos ) = 0;

private:
    bool m_visible;
    QIrBreadCrumbItem::Type m_type;
    QIrBreadCrumbComboBoxContainer * m_container;
    QRect m_rect;
};

class QIrBreadCrumbEmptyArea : public QIrBreadCrumbItem
{
public:
    QIrBreadCrumbEmptyArea( QIrBreadCrumbComboBoxContainer * );
    
	void clicked( const QPoint & pos );
};

class QIrBreadCrumbIndicator : public QIrBreadCrumbItem
{
public:
    QIrBreadCrumbIndicator(QIrBreadCrumbLabel *, QIrBreadCrumbComboBoxContainer *);
	~QIrBreadCrumbIndicator();

    inline bool isTruncated() const { return m_trunc; }
    inline void setTruncated( bool trunc ) { m_trunc = trunc; }

    inline QIrBreadCrumbLabel * label() const { return m_label; }
   
    void clicked( const QPoint & pos );

private:
    bool m_trunc;
    QIrBreadCrumbLabel * m_label;
};

class QIrBreadCrumbLabel : public QIrBreadCrumbItem
{
public:
    QIrBreadCrumbLabel(QIrBreadCrumbIndicator *, QIrBreadCrumbComboBoxContainer *, const QIrBreadCrumbModelNode & );
    
    inline QIrBreadCrumbModelNode node() const { return m_node; }
       
    inline QIrBreadCrumbIndicator * indicator() { return m_indicator; }
    inline void setIndicator( QIrBreadCrumbIndicator * indicator )  { m_indicator = indicator; }

	void clicked( const QPoint & pos );

private:
    QIrBreadCrumbModelNode m_node;
    QIrBreadCrumbIndicator * m_indicator;
};

class QIrBreadCrumbComboBoxContainer : public QWidget
{
    Q_OBJECT

public:
    QIrBreadCrumbComboBoxContainer( QIrBreadCrumbComboBox * comboBox );
    ~QIrBreadCrumbComboBoxContainer();

    inline QIrBreadCrumbComboBox * comboBox() const { return m_comboBox; }

	void splitPath( const QString & );

	inline QIrBreadCrumbModelNodeList nodeList() const { return m_nodeList; }
	void refresh();

protected:
    void mousePressEvent( QMouseEvent * );
    void mouseMoveEvent( QMouseEvent * );
    void mouseReleaseEvent(QMouseEvent * );
    void paintEvent( QPaintEvent * );
    void resizeEvent( QResizeEvent * );
    void leaveEvent( QEvent * );

private:
    friend class QIrBreadCrumbComboBox;

	void clear();
    void clearAll();
    void updateGeometries();
    int itemAt( const QPoint & pos );

    QList< QIrBreadCrumbItem * > m_items;
	QIrBreadCrumbModelNodeList m_nodeList;
    QIrBreadCrumbIndicator * m_rootIndicator;
    QIrBreadCrumbEmptyArea * m_emptyArea;

    int m_hoverItem;
    int m_downItem;
    QIrBreadCrumbComboBox * m_comboBox;
    bool m_clicked;
};

class QIrBreadCrumbIconWidget : public QWidget
{
	Q_OBJECT

public:
	QIrBreadCrumbIconWidget( QIrBreadCrumbComboBox * comboBox );
	
	inline QPixmap pixmap() const { return m_pixmap; }
	inline void setPixmap( const QPixmap & p ) { m_pixmap = p; update(); } 

protected:
	void paintEvent( QPaintEvent * );
	void mousePressEvent( QMouseEvent * );

private slots:
    void slotCopyAddress();
    void slotEditAddress();

private:
	QIrBreadCrumbComboBox * m_comboBox;
	QPixmap m_pixmap;
	bool m_clicked;
};

class QIrBreadCrumbComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit QIrBreadCrumbComboBox( QIrBreadCrumbBar * parent );
    ~QIrBreadCrumbComboBox();

	inline bool isFlat() const { return m_flat; }
	void setFlat( bool f );
	inline QIrBreadCrumbBar * bar() const { return m_bar; }
    inline QString location() const { return m_location; }
    void setLocation( const QString & );

    void showPopup();

    void showBreadCrumbs( bool popupError = true );
    void edit();

	inline QIrBreadCrumbComboBoxContainer * container() const { return m_container; }
    void updateGeometries();
private slots:
    void slotSetLocation( QAction * );
    void slotHandleEditTextChanged();
    void slotActivated();

protected:
	bool event( QEvent * );
    void resizeEvent( QResizeEvent * );
    void focusOutEvent( QFocusEvent * );

private:

	bool m_flat;
    QString m_location;
    QIrBreadCrumbIconWidget * m_iconLabel;
    QIrBreadCrumbComboBoxContainer * m_container;
    QIrBreadCrumbBar * m_bar;
	bool m_clicked;
};

class QIrBreadCrumbBarUi : public QIrWidgetUi
{
    QIR_DECLARE_WIDGET(QIrBreadCrumbBar);

public:
    QIrBreadCrumbBarUi();
    ~QIrBreadCrumbBarUi();

	bool m_flat;
	bool m_editable;
    QIrBreadCrumbComboBox * m_comboBox;
	QIrAbstractBreadCrumbModel * m_model;
	QIrBreadCrumbBarStyle * m_style;
	static QIrBreadCrumbBarStyle * m_globalStyle;
		
protected:
	void init();
};

#endif // QIRBREADCRUMBBAR_UI_H
