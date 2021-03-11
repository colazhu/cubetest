LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_RTTI_FLAG := -frtti
LOCAL_MODULE := libCEGUIOpenGLRenderer_nx
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES :=  \
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

LOCAL_C_INCLUDES +=  \
	externals/freetype/include \
	externals/zlib/contrib \
	externals/zlib \
       $(NXUI_ROOT)/externals \
       $(NXUI_ROOT)/CEGUI/CEGUIBase/include \
       $(NXUI_ROOT)/CEGUI/CEGUIOpenGLRenderer/include \
        $(egl_inc)

egl_inc := $(call include-path-for,opengl)
egl_cflags := $(call cflags-for,opengl)
egl_libs := $(call shared-libs-for,opengl)


LOCAL_SHARED_LIBRARIES += libfreetype libz libCEGuiBase_nx $(egl_libs)

LOCAL_CFLAGS := -fexceptions
LOCAL_CPPFLAGS := $(egl_cflags)
LOCAL_LDFLAGS := -lrt -lpthread -ldl

include $(BUILD_SHARED_LIBRARY)
