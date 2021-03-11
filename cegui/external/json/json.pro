TEMPLATE = lib
TARGET = json
CONFIG -= app_bundle gui core
CONFIG += c++11
#CONFIG += staticlib

include(json.pri)

#DEFINES += JSON_IS_AMALGAMATION

INCLUDEPATH += \
	./include

HEADERS += \
    allocator.h \
    assertions.h \
    autolink.h \
    config.h \
    features.h \
    forwards.h \
    json.h \
    json_tool.h \
    reader.h \
    value.h \
    version.h \
    version.h.in \
    writer.h


SOURCES += \
    json_reader.cpp \
    json_value.cpp \
    json_valueiterator.inl \
    json_writer.cpp


DEFINES +=

DISTFILES += \
    json.pri



