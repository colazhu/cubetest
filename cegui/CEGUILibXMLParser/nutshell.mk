LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_RTTI_FLAG := -frtti
LOCAL_MODULE := libCEGUILibXMLParser_nx
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES :=  \
    XMLParser.cpp \
    XMLParserModule.cpp \
    tinystr.cpp \
    tinyxml.cpp \
    tinyxmlerror.cpp \
    tinyxmlparser.cpp

LOCAL_C_INCLUDES +=  \
   $(NXUI_ROOT)/CEGUI/CEGUIBase/include \
   $(NXUI_ROOT)/CEGUI/CEGUILibXMLParser/include

LOCAL_SHARED_LIBRARIES += libCEGuiBase_nx

LOCAL_CFLAGS := -fexceptions
LOCAL_CPPFLAGS := -DCEGUI_TINYXML_HAS_2_6_API
LOCAL_LDFLAGS := -lrt -lpthread

include $(BUILD_SHARED_LIBRARY)
