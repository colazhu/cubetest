LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_RTTI_FLAG := -frtti
LOCAL_MODULE := libCEGUICoreWindowRendererSet_nx
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES :=  \
    Button.cpp \
    Default.cpp \
    Editbox.cpp \
    FrameWindow.cpp \
    ItemEntry.cpp \
    ItemListbox.cpp \
    Listbox.cpp \
    ListHeader.cpp \
    ListHeaderSegment.cpp \
    Menubar.cpp \
    MenuItem.cpp \
    Module.cpp \
    MultiColumnList.cpp \
    MultiLineEditbox.cpp \
    PopupMenu.cpp \
    ProgressBar.cpp \
    ScrollablePane.cpp \
    Scrollbar.cpp \
    Slider.cpp \
    Static.cpp \
    StaticImage.cpp \
    StaticText.cpp \
    TabButton.cpp \
    TabControl.cpp \
    Titlebar.cpp \
    ToggleButton.cpp \
    Tooltip.cpp \
    Tree.cpp

LOCAL_C_INCLUDES +=  \
       $(NXUI_ROOT)/CEGUI/CEGUIBase/include \
       $(NXUI_ROOT)/CEGUI/CEGUICoreWindowRendererSet/include

LOCAL_SHARED_LIBRARIES += libfreetype libz libCEGuiBase_nx
LOCAL_CFLAGS := -fexceptions
LOCAL_LDFLAGS := -lrt -lpthread -ldl

include $(BUILD_SHARED_LIBRARY)
