TEMPLATE    = lib
TARGET      = png
CONFIG -= app_bundle gui core
QT -= app_bundle gui core
# CONFIG += staticlib

# DEFINES += PNG_ARM_NEON_OPT=0
include($$(PROJROOT)/cegui/external/zlib/zlib.pri)
include($$(PROJROOT)/cegui/external/libpng/libpng.pri)

DESTDIR  = $$(PROJROOT)/out
LIBS += -L$$DESTDIR -lz


SOURCES += \
  png.c \
  pngerror.c \
  pngget.c \
  pngmem.c \
  pngpread.c \
  pngread.c \
  pngrio.c \
  pngrtran.c \
  pngrutil.c \
  pngset.c \
  pngtrans.c \
  pngwio.c \
  pngwrite.c \
  pngwtran.c \
  pngwutil.c

HEADERS += \
    include/png.h \
    include/pngconf.h \
#    include/zlib.h \
#    include/zconf.h

DEFINES += PNG_iTXt_SUPPORTED # PNG_BENIGN_ERRORS_SUPPORTED # PNG_READ_SUPPORTED
