TEMPLATE = app
TARGET = tst_cube

CONFIG -= app_bundle gui core
QT -= app_bundle gui core
CONFIG += c++11

DESTDIR  = $$(PROJROOT)/out

LIBS += -lglfw -lGLESv2 -lEGL -ldl -lpcre
LIBS += -L$$DESTDIR -lz -lfreetype -lcube -lgesture -lCEGUIBase -lCEGUIOpenGLRenderer

include($$(PROJROOT)/cegui/CEGUIBase/CEGUIBase.pri)
include($$(PROJROOT)/cegui/CEGUIOpenGLRenderer/CEGUIOpenGLRenderer.pri)

APP_PATH = \
    $$(PROJROOT) \
    $$(PROJROOT)/base \
    $$(PROJROOT)/gesture \
    $$(PROJROOT)/gesture/data \
    $$(PROJROOT)/cube \
    $$(PROJROOT)/cube/math \
    $$(PROJROOT)/cube/opengl

DEPENDPATH += $$APP_PATH
INCLUDEPATH += $$APP_PATH

HEADERS += \
    ../base/Flags.h \
    ../base/Log.h \
    ../base/Base.h \
    CEGuiRender.h

SOURCES += \
    ../base/Log.cpp \
    main.cpp \
    CEGuiRender.cpp

DISTFILES += \
