
TEMPLATE = lib
DEPENDPATH += .
DEFINES += TW_DLL_CXX

include(../config/common.pri)

HEADERS += $$files(thread/*.h)
HEADERS += $$files(xml/*.h)

SOURCES += $$files(thread/*.cpp)
SOURCES += $$files(xml/*.cpp)