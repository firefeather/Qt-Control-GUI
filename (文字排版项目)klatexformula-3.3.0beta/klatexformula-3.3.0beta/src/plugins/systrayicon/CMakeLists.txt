# ############################################################ #
# CMake project file for klatexformula/src/plugins/systrayicon #
# ############################################################ #
# $Id: CMakeLists.txt 733 2011-09-16 15:41:13Z phfaist $
# ############################################################ #

set(systrayicon_SRCS
  systrayicon.cpp
)
set(systrayicon_HEADERS
  systrayicon.h
)
set(systrayicon_UIS
  systrayiconconfigwidget.ui
  systraymainiconifybuttons.ui
)

qt4_wrap_cpp(systrayicon_MOC_CPPS ${systrayicon_HEADERS})
qt4_wrap_ui(systrayicon_UIS_H ${systrayicon_UIS})
add_library(systrayicon MODULE ${systrayicon_SRCS} ${systrayicon_MOC_CPPS} ${systrayicon_UIS_H})

KLFPluginUndefOrLink(systrayicon  klftools klfbackend klfapp)
