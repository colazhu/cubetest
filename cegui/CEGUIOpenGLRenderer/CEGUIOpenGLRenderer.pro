TEMPLATE = lib
TARGET = CEGUIOpenGLRenderer
CONFIG -= app_bundle gui core
CONFIG += c++11

include($$(PROJROOT)/cegui/CEGUIBase/CEGUIBase.pri)
include($$(PROJROOT)/cegui/external/glm/glm.pri)
# DEFINES += XXX_EXPORT

# LIBS += -lXXX -lXXX -lXXX -lXXX

unix {

}
else {

}

DESTDIR  = $$(PROJROOT)/out

INCLUDEPATH += $$(PROJROOT)/cegui/CEGUIOpenGLRenderer/include
DEPENDPATH += $$(PROJROOT)/cegui/CEGUIOpenGLRenderer/include


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
