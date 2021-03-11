TEMPLATE = lib
TARGET = CEGUIFreeImageImageCodec
CONFIG -= app_bundle gui core
#CONFIG += c++11
DESTDIR  = $$(NXUI_ROOT)/out
LIBS += -L$$DESTDIR -lFreeImage  -lpng -lz

include($$(NXUI_ROOT)/CEGUI/CEGUIBase/CEGUIBase.pri)
include($$(NXUI_ROOT)/external/FreeImage/FreeImage.pri)

# DEFINES += FREEIMAGE_LIB

# LIBS += -lXXX -lXXX -lXXX -lXXX

unix {
    LIBS += -lrt -lpthread -ldl
}
else {

}



INCLUDEPATH += $$(NXUI_ROOT)/CEGUI/CEGUIFreeImageImageCodec/include
DEPENDPATH += $$(NXUI_ROOT)/CEGUI/CEGUIFreeImageImageCodec/include


HEADERS += \    
    include/ImageCodec.h \
    include/ImageCodecModule.h
    

SOURCES += \    
    ImageCodec.cpp \
    ImageCodecModule.cpp \

DISTFILES += \
    nutshell.mk
