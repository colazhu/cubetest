TEMPLATE = lib
TARGET = CEGUICoreWindowRendererSet
QT-=app_bundle gui core
CONFIG -= app_bundle gui core
CONFIG += c++11

include($$(PROJROOT)/cegui/CEGUIBase/CEGUIBase.pri)
# DEFINES += XXX_EXPORT

# LIBS += -lXXX -lXXX -lXXX -lXXX

unix {

}
else {

}

DESTDIR  = $$(PROJROOT)/out

INCLUDEPATH += $$(PROJROOT)/cegui/CEGUICoreWindowRendererSet/include
DEPENDPATH += $$(PROJROOT)/cegui/CEGUICoreWindowRendererSet/include


HEADERS += \    
    include/Core/Button.h \
    include/Core/Default.h \
    include/Core/Editbox.h \
    include/Core/FrameWindow.h \
    include/Core/ItemEntry.h \
    include/Core/ItemListbox.h \
    include/Core/Listbox.h \
    include/Core/ListHeader.h \
    include/Core/ListHeaderSegment.h \
    include/Core/Menubar.h \
    include/Core/MenuItem.h \
    include/Core/Module.h \
    include/Core/MultiColumnList.h \
    include/Core/MultiLineEditbox.h \
    include/Core/PopupMenu.h \
    include/Core/ProgressBar.h \
    include/Core/ScrollablePane.h \
    include/Core/Scrollbar.h \
    include/Core/Slider.h \
    include/Core/Static.h \
    include/Core/StaticImage.h \
    include/Core/StaticText.h \
    include/Core/TabButton.h \
    include/Core/TabControl.h \
    include/Core/Titlebar.h \
    include/Core/ToggleButton.h \
    include/Core/Tooltip.h \
    include/Core/Tree.h
    

SOURCES += \    
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

DISTFILES += \
    nutshell.mk
