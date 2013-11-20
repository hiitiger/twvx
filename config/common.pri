
CONFIG -= qt

PRECOMPILED_HEADER = stable.h

INCLUDEPATH += .
INCLUDEPATH += ../include

QMAKE_LIBDIR += ../lib/$(ConfigurationName)
QMAKE_LIBDIR += "$(TargetDir)"

DESTDIR = ../bin/$(ConfigurationName)
OBJECTS_DIR = ../objdir/$(ProjectName)/$(ConfigurationName)
