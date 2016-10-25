/***************************************************************************
**                                                                        **
**  GridView, a simple GridView made with Qt4                             **
**  Copyright (C) 2013 Sacha Schutz                                       **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author : Sacha Schutz                                        **
**           Website: http://www.labsquare.org                            **
**           Email  : sacha@labsquare.org                                 **
**           Date   : 12.03.12                                            **
****************************************************************************/

#ifndef GRIDVIEW_H
#define GRIDVIEW_H

#include <QtGui>

class GridWidget;
class GridView;


/*! \class GridWidget
  \brief show a GridView without scrolling. If you want to have Scroll option, use
  \see GridView instead.

  GridWidget is defined by cell size, columncount and
  row count.See \ref setCellSize, \ref setGridSize.
  You can also switch on a cell with a specific color using \ref switchOn and select
  a cell using \ref selectOn
  Use \ref clear and \ref clearSelection to reset.
  GridWidget support mouse selection. And send the signal \ref cellClicked associated with grid
  coord.
  */

class GridWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GridWidget(int rowCount = 100, int columnCount =100,QWidget *parent = 0);
    void setGridSize(int rowCount, int columnCount);
    void switchOn(int x,int y, const QColor& color);
    void switchOff(int x, int y);
    void selectOn(int x,int y);
    void selectOff(int x,int y);
    void clear();
    void clearSelection();
    void setCellSize(int size);
    QPixmap * snap() ;

protected:
    void drawGrid(QPaintDevice * device);
    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent *);

protected:
    void createGrid();

signals:
    void cellClicked(QPoint pos);


private:
    int mCellSize;
    int mRowCount;
    int mColumnCount;
    QPixmap mGridPix;
    QHash<int, QColor >mColors;
    QList<int> mCellSelected;

};
/*! \class GridView
  \brief show a GridView without scrolling. If you want to have Scroll option, use
  \see GridView instead.

  GridWidget is defined by cell size, columncount and
  row count.See \ref setCellSize, \ref setGridSize.
  You can also switch on a cell with a specific color using \ref switchOn and select
  a cell using \ref selectOn
  Use \ref clear and \ref clearSelection to reset.
  GridWidget support mouse selection. And send the signal \ref cellClicked associated with grid
  coord.
  */
class GridView : public QScrollArea
{
    Q_OBJECT
public:
    explicit GridView(int row = 100, int column=100,QWidget *parent = 0);
    ~GridView();

    void setGridSize(int rowCount, int columnCount){
        mGridWidget->setGridSize(rowCount,columnCount);
    }

    void switchOn(int x,int y, const QColor& color){
        mGridWidget->switchOn(x,y,color);
    }

    void switchOff(int x, int y){
        mGridWidget->switchOff(x,y);
    }

    void selectOn(int x,int y){
        mGridWidget->selectOn(x,y);
    }

    void selectOff(int x,int y){
        mGridWidget->selectOff(x,y);
    }

    void clear(){
        mGridWidget->clear();
    }

    void clearSelection(){
        mGridWidget->clearSelection();
    }

    void setCellSize(int size){
        mGridWidget->setCellSize(size);
    }
    QPixmap * snap() {
        return mGridWidget->snap();
    }
signals:
    void cellClicked(QPoint pos);

private:
    GridWidget * mGridWidget;
    
};

//==== GridWidget
#endif // GRIDVIEW_H
