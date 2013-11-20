
TEMPLATE = lib
DEPENDPATH += .
DEFINES += TW_DLL_GUI

include(../config/common.pri)



HEADERS += $$files(core/*.h)
SOURCES += $$files(core/*.cpp)

HEADERS += $$files(widget/*.h)
SOURCES += $$files(widget/*.cpp)

HEADERS += $$files(native/*.h)
SOURCES += $$files(native/*.cpp)

HEADERS += $$files(eventloop/*.h)
SOURCES += $$files(eventloop/*.cpp)