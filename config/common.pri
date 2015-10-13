
CONFIG -= qt
CONFIG += c++11

PRECOMPILED_HEADER = stable.h

INCLUDEPATH += .
INCLUDEPATH += ../include
INCLUDEPATH += ../include/twbase
INCLUDEPATH += ../include/twgui


QMAKE_LIBDIR += ../lib/$(Configuration)
QMAKE_LIBDIR += ../bin/$(Configuration)

DESTDIR = ../bin/$(Configuration)
OBJECTS_DIR = ../objdir/$(ProjectName)/$(Configuration)

DEFINES += TW_BASE_STATIC
DEFINES += TW_GUI_STATIC