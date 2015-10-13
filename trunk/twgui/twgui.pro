
TEMPLATE = lib
DEPENDPATH += .
DEFINES += TW_DLL_GUI
CONFIG += staticlib

include(../config/common.pri)

LIBS += twbase.lib

HEADERS += $$files(twguiapp.h)
SOURCES += $$files(twguiapp.cpp)

HEADERS += $$files(config/*.h)

HEADERS += $$files(core/*.h)
SOURCES += $$files(core/*.cpp)

HEADERS += $$files(layout/*.h)
SOURCES += $$files(layout/*.cpp)

HEADERS += $$files(kernel/*.h)
SOURCES += $$files(kernel/*.cpp)

HEADERS += $$files(frame/*.h)
SOURCES += $$files(frame/*.cpp)

HEADERS += $$files(widget/*.h)
SOURCES += $$files(widget/*.cpp)

HEADERS += $$files(models/*.h)
SOURCES += $$files(models/*.cpp)

HEADERS += $$files(event/*.h)
SOURCES += $$files(event/*.cpp)

HEADERS += $$files(image/*.h)
SOURCES += $$files(image/*.cpp)

HEADERS += $$files(renderer/*.h)
SOURCES += $$files(renderer/*.cpp)

HEADERS += $$files(edit/*.h)
SOURCES += $$files(edit/*.cpp)