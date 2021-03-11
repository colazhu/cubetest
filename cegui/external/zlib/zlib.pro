TEMPLATE = lib
TARGET = z
CONFIG -= app_bundle gui core
#CONFIG += staticlib
#DEFINES += ZLIB_DLL

include(zlib.pri)
DESTDIR  = $$(NXUI_ROOT)/out

unix {

}
else {

}

HEADERS += \
    crc32.h \
    deflate.h \
    gzguts.h \
    inffast.h \
    inffixed.h \
    inflate.h \
    inftrees.h \
    trees.h \
    zutil.h \
    zconf.h \
    zlib.h

SOURCES += \
    adler32.c \
    compress.c \
    crc32.c \
    deflate.c \
    gzclose.c \
    gzlib.c \
    gzread.c \
    gzwrite.c \
    inflate.c \
    infback.c \
    inftrees.c \
    inffast.c \
    trees.c \
    uncompr.c \
    zutil.c \
    minizip/unzip.c \
    minizip/zip.c \
    minizip/ioapi.c

#    win32/zlib1.rc
