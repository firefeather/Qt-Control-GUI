#ifndef QFONTLABEL_H
#define QFONTLABEL_H

#include <QLabel>

class QFontLabel : public QLabel
{
    Q_OBJECT

    QFont   fcurrentFont;

public:
    explicit QFontLabel(QWidget *parent = 0);
    
signals:

    void    fontChanged();
    
public slots:

    void    setCurrentFont(const QFont & Value);
    const   QFont   &   currentFont()   const;
    
};

#endif // QFONTLABEL_H
