TEMPLATE = lib
TARGET = FreeImage
CONFIG -= app_bundle gui core
#CONFIG += staticlib

include($$(PROJROOT)/cegui/external/FreeImage/FreeImage.pri)
include($$(PROJROOT)/cegui/external/zlib/zlib.pri)
include($$(PROJROOT)/cegui/external/libpng/libpng.pri)

# DEFINES += OPJ_STATIC LIBRAW_NODLL NO_LCMS MNG_BUILD_SO FREEIMAGE_EXPORTS
DESTDIR  = $$(PROJROOT)/out
LIBS += -L$$DESTDIR -lz -lpng

unix {

}
else {

}

INCLUDEPATH += \
        FreeImageToolkit \
        LibTIFF4 \
        ZLib \
        LibJXR \
        LibJXR/image \
        LibJXR/image/decode \
        LibJXR/image/sys \
        LibJXR/image/x86 \
        LibJXR/image/encode \
        LibJXR/jxrgluelib \
        LibJXR/common \
        LibJXR/common/include \
        Metadata \
        LibWebP \
        LibWebP/src \
        LibWebP/src/dsp \
        LibWebP/src/mux \
        LibWebP/src/enc \
        LibWebP/src/demux \
        LibWebP/src/webp \
        LibWebP/src/dec \
        LibWebP/src/utils \
        LibOpenJPEG \
        LibRawLite \
        LibRawLite/dcraw \
        LibRawLite/libraw \
        LibRawLite/src \
        LibRawLite/internal \
        DeprecationManager \
        OpenEXR \
        OpenEXR/IlmThread \
        OpenEXR/IlmImf \
        OpenEXR/Half \
        OpenEXR/IexMath \
        OpenEXR/Imath \
        OpenEXR/Iex \
        OpenEXR/Copyrights \
        OpenEXR/Copyrights/ilmbase  \
        OpenEXR/Copyrights/openexr  \
        FreeImage \
        LibJPEG \
        FreeImageLib \
#                $$(PNG_INCLUDE_DIRS)

SOURCES += \
    LibJPEG/jaricom.c \
    LibJPEG/jcapimin.c \
    LibJPEG/jcapistd.c \
    LibJPEG/jcarith.c \
    LibJPEG/jccoefct.c \
    LibJPEG/jccolor.c \
    LibJPEG/jcdctmgr.c \
    LibJPEG/jchuff.c \
    LibJPEG/jcinit.c \
    LibJPEG/jcmainct.c \
    LibJPEG/jcmarker.c \
    LibJPEG/jcmaster.c \
    LibJPEG/jcomapi.c \
    LibJPEG/jcparam.c \
    LibJPEG/jcprepct.c \
    LibJPEG/jcsample.c \
    LibJPEG/jctrans.c \
    LibJPEG/jdapimin.c \
    LibJPEG/jdapistd.c \
    LibJPEG/jdarith.c \
    LibJPEG/jdatadst.c \
    LibJPEG/jdatasrc.c \
    LibJPEG/jdcoefct.c \
    LibJPEG/jdcolor.c \
    LibJPEG/jddctmgr.c \
    LibJPEG/jdhuff.c \
    LibJPEG/jdinput.c \
    LibJPEG/jdmainct.c \
    LibJPEG/jdmarker.c \
    LibJPEG/jdmaster.c \
    LibJPEG/jdmerge.c \
    LibJPEG/jdpostct.c \
    LibJPEG/jdsample.c \
    LibJPEG/jdtrans.c \
    LibJPEG/jerror.c \
    LibJPEG/jfdctflt.c \
    LibJPEG/jfdctfst.c \
    LibJPEG/jfdctint.c \
    LibJPEG/jidctflt.c \
    LibJPEG/jidctfst.c \
    LibJPEG/jidctint.c \
    LibJPEG/jquant1.c \
    LibJPEG/jquant2.c \
    LibJPEG/jutils.c \
    LibJPEG/jmemmgr.c \
    LibJPEG/jmemnobs.c

SOURCES += \
    FreeImage/BitmapAccess.cpp \
    FreeImage/ColorLookup.cpp \
    FreeImage/FreeImage.cpp \
    FreeImage/FreeImageC.c \
    FreeImage/FreeImageIO.cpp \
    FreeImage/GetType.cpp \
    FreeImage/MemoryIO.cpp \
    FreeImage/PixelAccess.cpp \
    FreeImage/MNGHelper.cpp \
    FreeImage/Plugin.cpp \
    FreeImage/PluginPNG.cpp \
    FreeImage/PluginBMP.cpp \
    FreeImage/PSDParser.cpp \
    FreeImage/TIFFLogLuv.cpp \
    FreeImage/Conversion.cpp \
    FreeImage/Conversion16_555.cpp \
    FreeImage/Conversion16_565.cpp \
    FreeImage/Conversion24.cpp \
    FreeImage/Conversion32.cpp \
    FreeImage/Conversion4.cpp \
    FreeImage/Conversion8.cpp \
    FreeImage/ConversionFloat.cpp \
    FreeImage/ConversionRGB16.cpp \
    FreeImage/ConversionRGBA16.cpp \
    FreeImage/ConversionRGBAF.cpp \
    FreeImage/ConversionRGBF.cpp \
    FreeImage/ConversionType.cpp \
    FreeImage/ConversionUINT16.cpp \
    FreeImage/Halftoning.cpp \
    FreeImage/tmoColorConvert.cpp \
    FreeImage/tmoDrago03.cpp \
    FreeImage/tmoFattal02.cpp \
    FreeImage/tmoReinhard05.cpp \
    FreeImage/ToneMapping.cpp \
    FreeImage/LFPQuantizer.cpp \
    FreeImage/NNQuantizer.cpp \
    FreeImage/WuQuantizer.cpp \
    FreeImage/CacheFile.cpp \
    FreeImage/MultiPage.cpp \
    FreeImage/ZLibInterface.cpp \
    FreeImageToolkit/ClassicRotate.cpp \
    FreeImageToolkit/CopyPaste.cpp \
    FreeImageToolkit/Rescale.cpp \
    FreeImageToolkit/Background.cpp \
    FreeImageToolkit/Channels.cpp \
    FreeImageToolkit/Flip.cpp \
    FreeImageToolkit/Display.cpp \
    FreeImageToolkit/MultigridPoissonSolver.cpp \
    FreeImageToolkit/Colors.cpp \
    FreeImageToolkit/Resize.cpp \
    FreeImageToolkit/BSplineRotate.cpp \
    FreeImage/PluginJPEG.cpp \
    DeprecationManager/Deprecated.cpp \
    DeprecationManager/DeprecationMgr.cpp \
    Metadata/Exif.cpp \
    Metadata/FIRational.cpp \
    Metadata/FreeImageTag.cpp \
    Metadata/IPTC.cpp \
    Metadata/TagConversion.cpp \
    Metadata/TagLib.cpp


# DEFINES += PNG_iTXt_SUPPORTED

DISTFILES += \
    nutshell.mk
