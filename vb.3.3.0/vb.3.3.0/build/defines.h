#ifndef DEFINES_H
#define DEFINES_H

#include <Qt/qglobal.h>

#ifdef LIBVB4_LIB
# define LIBVB4_EXPORT Q_DECL_EXPORT
#else
# define LIBVB4_EXPORT Q_DECL_IMPORT
#endif

#endif // DEFINES_H
