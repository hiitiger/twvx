
TEMPLATE = lib
DEPENDPATH += .
DEFINES += TW_DLL_IMG

include(../config/common.pri)

CONFIG -= qt

PRECOMPILED_HEADER = stable.h

INCLUDEPATH += .
INCLUDEPATH += ../include
