
TEMPLATE = vcapp
DEPENDPATH += .
LIBS += twbase.lib
LIBS += twgui.lib

include(../config/common.pri)

SOURCES += main.cpp
HEADERS += $$files(test/*.h)
SOURCES += $$files(test/*.cpp)
