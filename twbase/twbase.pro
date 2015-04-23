
TEMPLATE = lib
DEPENDPATH += .
DEFINES += TW_DLL_BASE

include(../config/common.pri)


HEADERS += $$files(config/*.h)
SOURCES += $$files(config/*.cpp)

HEADERS += $$files(thread/*.h)
SOURCES += $$files(thread/*.cpp)

HEADERS += $$files(xml/*.h)
SOURCES += $$files(xml/*.cpp)

HEADERS += $$files(msg/*.h)
SOURCES += $$files(msg/*.cpp)

HEADERS += $$files(base/*.h)
SOURCES += $$files(base/*.cpp)

HEADERS += $$files(graphics/*.h)
SOURCES += $$files(graphics/*.cpp)

HEADERS += $$files(core/*.h)
SOURCES += $$files(core/*.cpp)

HEADERS += $$files(cxx/*.h)
SOURCES += $$files(cxx/*.cpp)

HEADERS += $$files(signal/*.h)
SOURCES += $$files(signal/*.cpp)

HEADERS += $$files(task/*.h)
SOURCES += $$files(task/*.cpp)

HEADERS += $$files(log/*.h)
SOURCES += $$files(log/*.cpp)

HEADERS += $$files(native/*.h)
SOURCES += $$files(native/*.cpp)

HEADERS += $$files(tool/*.h)
SOURCES += $$files(tool/*.cpp)

HEADERS += $$files(tinyipc/*.h)
SOURCES += $$files(tinyipc/*.cpp)