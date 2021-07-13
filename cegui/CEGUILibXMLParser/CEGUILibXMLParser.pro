TEMPLATE = lib
TARGET = CEGUILibXMLParser
CONFIG -= app_bundle gui core
QT -= app_bundle gui core
#CONFIG += c++11

include(CEGUILibXMLParser.pri)
include($$(PROJROOT)/cegui/CEGUIBase/CEGUIBase.pri)
# include($$(PROJROOT)/cegui/external/libxml2/libxml2.pri)
DEFINES += CEGUI_TINYXML_HAS_2_6_API

unix {

}
else {

}

DESTDIR  = $$(PROJROOT)/out
LIBS += -L$$DESTDIR # -lxml2 -lCEGUIBase

HEADERS += \    
    include/XMLParser.h \
    include/XMLParserModule.h \
    tinystr.h \
    tinyxml.h
    

SOURCES += \    
    XMLParser.cpp \
    XMLParserModule.cpp \
    tinystr.cpp \
    tinyxml.cpp \
    tinyxmlerror.cpp \
    tinyxmlparser.cpp

DISTFILES += \
    nutshell.mk
