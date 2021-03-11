LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_RTTI_FLAG := -frtti
LOCAL_MODULE := libCEGUIFreeImageImageCodec_nx
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES :=  \
    ImageCodec.cpp \
    ImageCodecModule.cpp \

LOCAL_C_INCLUDES +=  \
    externals/zlib/contrib \
    externals/zlib \
    externals/libpng/include \
   $(NXUI_ROOT)/externals/FreeImage \
   $(NXUI_ROOT)/externals/FreeImage/LibTIFF \
   $(NXUI_ROOT)/externals/FreeImage/LibRawLite \
   $(NXUI_ROOT)/externals/FreeImage/DeprecationManager \
   $(NXUI_ROOT)/externals/FreeImage/OpenEXR \
   $(NXUI_ROOT)/externals/FreeImage/OpenEXR/Half \
   $(NXUI_ROOT)/externals/FreeImage/OpenEXR/Iex \
   $(NXUI_ROOT)/externals/FreeImage/OpenEXR/IlmImf \
   $(NXUI_ROOT)/externals/FreeImage/OpenEXR/Imath \
   $(NXUI_ROOT)/externals/FreeImage/OpenEXR/IlmThread \
   $(NXUI_ROOT)/CEGUI/CEGUIBase/include \
   $(NXUI_ROOT)/CEGUI/CEGUIFreeImageImageCodec/include


LOCAL_SHARED_LIBRARIES += libFreeImage_nx libpng libz libCEGuiBase_nx

LOCAL_CFLAGS := -fexceptions

LOCAL_LDFLAGS := -lrt -lpthread -ldl

include $(BUILD_SHARED_LIBRARY)
