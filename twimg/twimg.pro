
TEMPLATE = lib
DEPENDPATH += .
DEFINES += TW_DLL_IMG

LIBS += twcxx.lib

CONFIG -= qt

PRECOMPILED_HEADER = stable.h

INCLUDEPATH += .
INCLUDEPATH += ../include
INCLUDEPATH += ../include/twcxx

QMAKE_LIBDIR += ../lib/$(ConfigurationName)
QMAKE_LIBDIR += "$(TargetDir)"

DESTDIR = ../bin/$(ConfigurationName)
OBJECTS_DIR = ../objdir/$(ProjectName)/$(ConfigurationName)

