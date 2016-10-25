#include "iisrect.h"

iisRect::iisRect(int radius) :
    tl(radius),
    tr(radius),
    bl(radius),
    br(radius)
{
  is_equal = true;
}

iisRect::iisRect(int topLeft, int topRight, int bottomLeft, int bottomRight) :
    tl(topLeft),
    tr(topRight),
    bl(bottomLeft),
    br(bottomRight)
{
  is_equal = (tl == tr && bl == br && tl == bl);
}

void iisRect::draw(QPainter *painter, const QRect &rect)
{
  if (is_equal)
  {
    if (tl)
      painter->drawRoundedRect(rect, tl, tl);
    else
      painter->drawRect(rect);

    return;
  }

  // rect is non-uniform
  QPainterPath path;

  int h = rect.height();
  int w = rect.width();

  // top-left
  if (tl)
  {
    path.moveTo(tl,0);
    path.arcTo(QRect(0,0,tl*2,tl*2),90,90);
  }
  else
    path.moveTo(0,0);

  // bottom-left
  if (bl)
  {
    path.lineTo(0,h-bl);
    path.arcTo(QRect(0,h-bl*2,bl*2,bl*2),180,90);
  }
  else
    path.lineTo(0,h);

  // bottom-right
  if (br)
  {
    path.lineTo(w-br,h);
    path.arcTo(QRect(w-br*2,h-br*2,br*2,br*2),270,90);
  }
  else
    path.lineTo(w,h);

  // top-right
  if (tr)
  {
    path.lineTo(w,tr);
    path.arcTo(QRect(w-tr*2,0,tr*2,tr*2),0,90);
  }
  else
    path.lineTo(w,0);

  path.closeSubpath();

  painter->drawPath(path);
}
