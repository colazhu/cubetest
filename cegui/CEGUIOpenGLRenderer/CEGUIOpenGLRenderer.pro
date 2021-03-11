TEMPLATE = lib
TARGET = CEGUIOpenGLRenderer
CONFIG -= app_bundle gui core
CONFIG += c++11

include($$(NXUI_ROOT)/CEGUI/CEGUIBase/CEGUIBase.pri)
include($$(NXUI_ROOT)/external/glm/glm.pri)
# DEFINES += XXX_EXPORT

# LIBS += -lXXX -lXXX -lXXX -lXXX

unix {

}
else {

}

DESTDIR  = $$(NXUI_ROOT)/out

INCLUDEPATH += $$(NXUI_ROOT)/CEGUI/CEGUIOpenGLRenderer/include
DEPENDPATH += $$(NXUI_ROOT)/CEGUI/CEGUIOpenGLRenderer/include


HEADERS += \
    include/GeometryBufferBase.h \
    include/GL3FBOTextureTarget.h \
    include/GL3GeometryBuffer.h \
    include/GL3Renderer.h \
    include/GlmPimpl.h \
    include/RendererBase.h \
    include/RenderTarget.h \
    include/Shader.h \
    include/ShaderManager.h \
    include/StandardShaderFrag.h \
    include/StandardShaderVert.h \
    include/StateChangeWrapper.h \
    include/Texture.h \
    include/TextureTarget.h \
    include/ViewportTarget.h
    

SOURCES += \    
    GeometryBufferBase.cpp \
    GL3FBOTextureTarget.cpp \
    GL3GeometryBuffer.cpp \
    GL3Renderer.cpp \
    RendererBase.cpp \
    RenderTarget.inl \
    Shader.cpp \
    ShaderManager.cpp \
    StateChangeWrapper.cpp \
    Texture.cpp \
    TextureTarget.cpp \
    ViewportTarget.cpp

DISTFILES += \
    nutshell.mk
