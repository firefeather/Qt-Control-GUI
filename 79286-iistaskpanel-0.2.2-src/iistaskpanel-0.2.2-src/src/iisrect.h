#ifndef IISRECT_H
#define IISRECT_H

#include <QtGui>

class iisRect
{
public:
    iisRect(int topLeft, int topRight, int bottomLeft, int bottomRight);
    iisRect(int radius = 0);

    void draw(QPainter *painter, const QRect &rect);

public:
    int tl, tr, bl, br;
    bool is_equal;
};

#endif // IISRECT_H
