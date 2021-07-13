TEMPLATE = lib
TARGET = freetype
QT-=app_bundle gui core
CONFIG -= app_bundle gui core
#CONFIG += c++11
#CONFIG += staticlib

# DEFINES += XXX_EXPORT
include(freetype.pri)
DESTDIR  = $$(PROJROOT)/out
LIBS += -L$$DESTDIR -lz

unix {

}
else {

}

INCLUDEPATH += \
	./include

# DEPENDPATH += $$(PROJROOT)/cegui/external/freetype/include


HEADERS += \
    src/autofit/afangles.h \
    src/autofit/afcjk.h \
    src/autofit/afdummy.h \
    src/autofit/aferrors.h \
    src/autofit/afglobal.h \
    src/autofit/afhints.h \
    src/autofit/afindic.h \
    src/autofit/aflatin.h \
    src/autofit/aflatin2.h \
    src/autofit/afloader.h \
    src/autofit/afmodule.h \
    src/autofit/afpic.h \
    src/autofit/aftypes.h \
    src/autofit/afwarp.h
 

SOURCES += \
    src/base/ftsystem.c \
    src/base/ftinit.c \
    src/base/ftdebug.c \
    src/base/ftbase.c \
    src/base/ftbbox.c \
    src/base/ftglyph.c \
    src/base/ftbdf.c \
    src/base/ftbitmap.c \
    src/base/ftcid.c \
    src/base/ftfstype.c \
    src/base/ftgasp.c \
    src/base/ftgxval.c \
    src/base/ftlcdfil.c \
    src/base/ftmm.c \
    src/base/ftotval.c \
    src/base/ftpatent.c \
    src/base/ftpfr.c \
    src/base/ftstroke.c \
    src/base/ftsynth.c \
    src/base/fttype1.c \
    src/base/ftwinfnt.c \
    src/base/ftxf86.c \
    src/base/ftobjs.c \
    src/base/basepic.c \
    src/base/ftadvanc.c \
    src/base/ftapi.c \
    src/base/ftcalc.c \
    src/base/ftdbgmem.c \
    src/base/ftgloadr.c \
    src/base/ftmac.c \
    src/base/ftoutln.c \
    src/base/ftpic.c \
    src/base/ftrfork.c \
    src/base/ftsnames.c \
    src/base/ftstream.c \
    src/base/fttrigon.c \
    src/base/ftutil.c \
    src/bdf/bdf.c \
    src/cff/cff.c \
    src/cid/type1cid.c \
    src/pcf/pcf.c \
    src/pfr/pfr.c \
    src/sfnt/sfnt.c \
    src/truetype/truetype.c \
    src/type1/type1.c \
    src/type42/type42.c \
    src/winfonts/winfnt.c \
    src/raster/raster.c \
    src/smooth/smooth.c \
    src/autofit/autofit.c \
    src/cache/ftcache.c \
    src/gzip/ftgzip.c \
    src/lzw/ftlzw.c \
    src/gxvalid/gxvalid.c \
    src/otvalid/otvalid.c \
    src/psaux/psaux.c \
    src/pshinter/pshinter.c \
    src/psnames/psnames.c \
    src/autofit/afangles.c \
    src/autofit/afcjk.c \
    src/autofit/afdummy.c \
    src/autofit/afglobal.c \
    src/autofit/afhints.c \
    src/autofit/afindic.c \
    src/autofit/aflatin.c \
    src/autofit/aflatin2.c \
    src/autofit/afloader.c \
    src/autofit/afmodule.c \
    src/autofit/afpic.c \
    src/autofit/afwarp.c


DEFINES += FT2_BUILD_LIBRARY FT_CONFIG_OPTION_SYSTEM_ZLIB



