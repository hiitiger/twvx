
TEMPLATE = lib
DEPENDPATH += .
DEFINES += TW_DLL_GUI

LIBS += twcxx.lib

include(../config/common.pri)

HEADERS += twguiconfig.h
HEADERS += twguiinternaldefine.h
HEADERS += $$files(core/*.h)
SOURCES += $$files(core/*.cpp)

HEADERS += $$files(frame/*.h)
SOURCES += $$files(frame/*.cpp)

HEADERS += $$files(widget/*.h)
SOURCES += $$files(widget/*.cpp)

HEADERS += $$files(native/*.h)
SOURCES += $$files(native/*.cpp)

HEADERS += $$files(event/*.h)
SOURCES += $$files(event/*.cpp)