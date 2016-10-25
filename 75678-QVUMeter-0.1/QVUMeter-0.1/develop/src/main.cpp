#include <QApplication>
#include "qvumeter.h"

int main(int argc, char ** argv)
{
    QApplication app( argc, argv );
    QVUMeter vumeter;
    vumeter.show();

    app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
    return app.exec();
}
