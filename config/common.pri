
CONFIG -= qt

PRECOMPILED_HEADER = stable.h

INCLUDEPATH += .
INCLUDEPATH += ../include
INCLUDEPATH += ../include/twcxx
INCLUDEPATH += ../include/twgui


QMAKE_LIBDIR += ../lib/$(ConfigurationName)
QMAKE_LIBDIR += "$(TargetDir)"

DESTDIR = ../bin/$(ConfigurationName)
OBJECTS_DIR = ../objdir/$(ProjectName)/$(ConfigurationName)
