TEMPLATE = lib
TARGET = glm
CONFIG -= app_bundle gui core

include(glm.pri)
#CONFIG += staticlib

# DEFINES += XXX_EXPORT

DESTDIR  = $$(NXUI_ROOT)/out

unix {

}
else {

}



INCLUDEPATH += \
	./include

# DEPENDPATH += $$(NXUI_ROOT)/external/glm


HEADERS += \
 

SOURCES += \




