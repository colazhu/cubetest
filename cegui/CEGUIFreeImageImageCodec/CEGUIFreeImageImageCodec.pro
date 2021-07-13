TEMPLATE = lib
TARGET = CEGUIFreeImageImageCodec
CONFIG -= app_bundle gui core
QT -= app_bundle gui core
#CONFIG += c++11
DESTDIR  = $$(PROJROOT)/out
LIBS += -L$$DESTDIR -lFreeImage  -lpng -lz

include($$(PROJROOT)/cegui/CEGUIBase/CEGUIBase.pri)
include($$(PROJROOT)/cegui/external/FreeImage/FreeImage.pri)

# DEFINES += FREEIMAGE_LIB

# LIBS += -lXXX -lXXX -lXXX -lXXX

unix {
    LIBS += -lrt -lpthread -ldl
}
else {

}

INCLUDEPATH += $$(PROJROOT)/cegui/CEGUIFreeImageImageCodec/include
DEPENDPATH += $$(PROJROOT)/cegui/CEGUIFreeImageImageCodec/include


HEADERS += \    
    include/ImageCodec.h \
    include/ImageCodecModule.h
    

SOURCES += \    
    ImageCodec.cpp \
    ImageCodecModule.cpp \

DISTFILES += \
    nutshell.mk
