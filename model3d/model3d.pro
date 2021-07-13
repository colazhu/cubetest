TARGET = mode3d
TEMPLATE = lib # lib app

CONFIG += c++11

DISTFILES +=

HEADERS += \
    GL3Model3DRenderer.h \
    Model3D.h \
    Model3DAnimation.h \
    Model3DDefine.h \
    Model3DInstance.h \
    Model3DManager.h \
    Model3DRenderer.h \
    Model3DRenderer2.h \
    Model3DShaderFrag.h \
    Model3DShaderVert.h \
    Model3D_xmlHandler.h

SOURCES += \
    GL3Model3DRenderer.cpp \
    Model3D.cpp \
    Model3DAnimation.cpp \
    Model3DInstance.cpp \
    Model3DManager.cpp \
    Model3D_xmlHandler.cpp
