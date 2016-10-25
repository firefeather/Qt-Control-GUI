#include <QApplication>
#include "qreflex.h"

int main(int argc, char ** argv)
{
    QApplication app( argc, argv );
    QReflex reflex;
    reflex.show();

    app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
    return app.exec();
}
