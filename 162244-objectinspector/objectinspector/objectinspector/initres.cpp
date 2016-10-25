#include <QtGlobal>

static  bool    init()
{
    Q_INIT_RESOURCE_EXTERN(objinspector)
    Q_INIT_RESOURCE(objinspector);
    return  true;
}

static  bool    inited = init();

