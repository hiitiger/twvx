
TEMPLATE = lib
DEPENDPATH += .
DEFINES += TW_DLL_CXX

include(../config/common.pri)

HEADERS += $$files(thread/*.h)
SOURCES += $$files(thread/*.cpp)

HEADERS += $$files(xml/*.h)
SOURCES += $$files(xml/*.cpp)


HEADERS += $$files(base/*.h)

SOURCES += $$files(base/*.cpp)