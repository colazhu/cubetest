TEMPLATE = lib
TARGET = CEGUIBase
QT-=app_bundle gui core
CONFIG -= app_bundle gui core
CONFIG += c++11

# DEFINES += XXX_EXPORT
include(CEGUIBase.pri)
include($$(PROJROOT)/cegui/external/freetype/freetype.pri)
include($$(PROJROOT)/cegui/external/zlib/zlib.pri)
# LIBS += -lXXX -lXXX -lXXX -lXXX
DESTDIR  = $$(PROJROOT)/out
LIBS += -L$$DESTDIR -lz -lfreetype
LIBS += -ldl -lpcre

HEADERS += \
    include/CEGUI/Affector.h \
    include/CEGUI/Animation.h \
    include/CEGUI/AnimationInstance.h \
    include/CEGUI/AnimationManager.h \
    include/CEGUI/Animation_xmlHandler.h \
    include/CEGUI/Base.h \
    include/CEGUI/BasicImage.h \
    include/CEGUI/BasicRenderedStringParser.h \
    include/CEGUI/BidiVisualMapping.h \
    include/CEGUI/BoundSlot.h \
    include/CEGUI/CEGUI.h \
    include/CEGUI/CentredRenderedString.h \
    include/CEGUI/ChainedXMLHandler.h \
    include/CEGUI/Clipboard.h \
    include/CEGUI/Colour.h \
    include/CEGUI/ColourRect.h \
    include/CEGUI/CompositeResourceProvider.h \
    include/CEGUI/Config.h.in \
    include/CEGUI/Config_xmlHandler.h \
    include/CEGUI/CoordConverter.h \
    include/CEGUI/DataContainer.h \
    include/CEGUI/DefaultLogger.h \
    include/CEGUI/DefaultRenderedStringParser.h \
    include/CEGUI/DefaultResourceProvider.h \
    include/CEGUI/DynamicModule.h \
    include/CEGUI/Element.h \
    include/CEGUI/Event.h \
    include/CEGUI/EventArgs.h \
    include/CEGUI/EventSet.h \
    include/CEGUI/Exceptions.h \
    include/CEGUI/FactoryModule.h \
    include/CEGUI/FactoryRegisterer.h \
    include/CEGUI/Font.h \
    include/CEGUI/FontGlyph.h \
    include/CEGUI/FontManager.h \
    include/CEGUI/Font_xmlHandler.h \
    include/CEGUI/FormattedRenderedString.h \
    include/CEGUI/ForwardRefs.h \
    include/CEGUI/FreeFunctionSlot.h \
    include/CEGUI/FreeTypeFont.h \
    include/CEGUI/FribidiVisualMapping.h \
    include/CEGUI/FunctorCopySlot.h \
    include/CEGUI/FunctorPointerSlot.h \
    include/CEGUI/FunctorReferenceBinder.h \
    include/CEGUI/FunctorReferenceSlot.h \
    include/CEGUI/GeometryBuffer.h \
    include/CEGUI/GlobalEventSet.h \
    include/CEGUI/GUIContext.h \
    include/CEGUI/GUILayout_xmlHandler.h \
    include/CEGUI/IconvStringTranscoder.h \
    include/CEGUI/Image.h \
    include/CEGUI/ImageCodec.h \
    include/CEGUI/ImageFactory.h \
    include/CEGUI/ImageManager.h \
    include/CEGUI/InjectedInputReceiver.h \
    include/CEGUI/InputEvent.h \
    include/CEGUI/Interpolator.h \
    include/CEGUI/IteratorBase.h \
    include/CEGUI/JustifiedRenderedString.h \
    include/CEGUI/KeyFrame.h \
    include/CEGUI/LeftAlignedRenderedString.h \
    include/CEGUI/LinkedEvent.h \
    include/CEGUI/Logger.h \
    include/CEGUI/MemberFunctionSlot.h \
    include/CEGUI/MemoryAllocatedObject.h \
    include/CEGUI/MemoryAllocation.h \
    include/CEGUI/MemoryOgreAllocator.h \
    include/CEGUI/MemoryStdAllocator.h \
    include/CEGUI/MemorySTLWrapper.h \
    include/CEGUI/MinibidiVisualMapping.h \
    include/CEGUI/MinizipResourceProvider.h \
    include/CEGUI/ModuleConfig.h \
    include/CEGUI/ModuleConfig.h.in \
    include/CEGUI/MouseCursor.h \
    include/CEGUI/NamedElement.h \
    include/CEGUI/NamedXMLResourceManager.h \
    include/CEGUI/PCRERegexMatcher.h \
    include/CEGUI/PixmapFont.h \
    include/CEGUI/Property.h \
    include/CEGUI/PropertyHelper.h \
    include/CEGUI/PropertySet.h \
    include/CEGUI/Quaternion.h \
    include/CEGUI/Rect.h \
    include/CEGUI/RefCounted.h \
    include/CEGUI/RegexMatcher.h \
    include/CEGUI/RenderedString.h \
    include/CEGUI/RenderedStringComponent.h \
    include/CEGUI/RenderedStringImageComponent.h \
    include/CEGUI/RenderedStringParser.h \
    include/CEGUI/RenderedStringTextComponent.h \
    include/CEGUI/RenderedStringWidgetComponent.h \
    include/CEGUI/RenderedStringWordWrapper.h \
    include/CEGUI/RenderEffect.h \
    include/CEGUI/RenderEffectFactory.h \
    include/CEGUI/RenderEffectManager.h \
    include/CEGUI/Renderer.h \
    include/CEGUI/RenderingContext.h \
    include/CEGUI/RenderingSurface.h \
    include/CEGUI/RenderingWindow.h \
    include/CEGUI/RenderQueue.h \
    include/CEGUI/RenderTarget.h \
    include/CEGUI/ResourceProvider.h \
    include/CEGUI/RightAlignedRenderedString.h \
    include/CEGUI/Scheme.h \
    include/CEGUI/SchemeManager.h \
    include/CEGUI/Scheme_xmlHandler.h \
    include/CEGUI/ScriptModule.h \
    include/CEGUI/SimpleTimer.h \
    include/CEGUI/Singleton.h \
    include/CEGUI/Size.h \
    include/CEGUI/SlotFunctorBase.h \
    include/CEGUI/String.h \
    include/CEGUI/StringTranscoder.h \
    include/CEGUI/SubscriberSlot.h \
    include/CEGUI/System.h \
    include/CEGUI/SystemKeys.h \
    include/CEGUI/Texture.h \
    include/CEGUI/TextureTarget.h \
    include/CEGUI/TextUtils.h \
    include/CEGUI/TplInterpolators.h \
    include/CEGUI/TplProperty.h \
    include/CEGUI/TplWindowFactory.h \
    include/CEGUI/TplWindowFactoryRegisterer.h \
    include/CEGUI/TplWindowProperty.h \
    include/CEGUI/TplWindowRendererFactory.h \
    include/CEGUI/TplWindowRendererProperty.h \
    include/CEGUI/TplWRFactoryRegisterer.h \
    include/CEGUI/TypedProperty.h \
    include/CEGUI/UDim.h \
    include/CEGUI/Vector.h \
    include/CEGUI/Version.h \
    include/CEGUI/Version.h.in \
    include/CEGUI/Vertex.h \
    include/CEGUI/Win32StringTranscoder.h \
    include/CEGUI/Window.h \
    include/CEGUI/WindowFactory.h \
    include/CEGUI/WindowFactoryManager.h \
    include/CEGUI/WindowManager.h \
    include/CEGUI/WindowRenderer.h \
    include/CEGUI/WindowRendererManager.h \
    include/CEGUI/XMLAttributes.h \
    include/CEGUI/XMLHandler.h \
    include/CEGUI/XMLParser.h \
    include/CEGUI/XMLSerializer.h \
    include/CEGUI/Config.h \
    include/CEGUI/falagard/FalagardXMLHandler.h

HEADERS += \
    include/CEGUI/falagard/ComponentBase.h \
    include/CEGUI/falagard/Dimensions.h \
    include/CEGUI/falagard/Enums.h \
    include/CEGUI/falagard/EventAction.h \
    include/CEGUI/falagard/EventLinkDefinition.h \
    include/CEGUI/falagard/FalagardPropertyBase.h \
    include/CEGUI/falagard/FormattingSetting.h \
    include/CEGUI/falagard/FrameComponent.h \
    include/CEGUI/falagard/ImageryComponent.h \
    include/CEGUI/falagard/ImagerySection.h \
    include/CEGUI/falagard/LayerSpecification.h \
    include/CEGUI/falagard/NamedArea.h \
    include/CEGUI/falagard/NamedDefinitionCollator.h \
    include/CEGUI/falagard/PropertyDefinition.h \
    include/CEGUI/falagard/PropertyDefinitionBase.h \
    include/CEGUI/falagard/PropertyInitialiser.h \
    include/CEGUI/falagard/PropertyLinkDefinition.h \
    include/CEGUI/falagard/SectionSpecification.h \
    include/CEGUI/falagard/StateImagery.h \
    include/CEGUI/falagard/TextComponent.h \
    include/CEGUI/falagard/WidgetComponent.h \
    include/CEGUI/falagard/WidgetLookFeel.h \
    include/CEGUI/falagard/WidgetLookManager.h \
    include/CEGUI/falagard/XMLEnumHelper.h


SOURCES += \
    Affector.cpp \
    Animation.cpp \
    AnimationInstance.cpp \
    AnimationManager.cpp \
    Animation_xmlHandler.cpp \
    Base.cpp \
    BasicImage.cpp \
    BasicRenderedStringParser.cpp \
    BidiVisualMapping.cpp \
    BoundSlot.cpp \
    CentredRenderedString.cpp \
    ChainedXMLHandler.cpp \
    Clipboard.cpp \
    Colour.cpp \
    ColourRect.cpp \
    CompositeResourceProvider.cpp \
    Config_xmlHandler.cpp \
    CoordConverter.cpp \
    DataContainer.cpp \
    DefaultLogger.cpp \
    DefaultRenderedStringParser.cpp \
    DefaultResourceProvider.cpp \
    DynamicModule.cpp \
    Element.cpp \
    Event.cpp \
    EventArgs.cpp \
    EventSet.cpp \
    Exceptions.cpp \
    FactoryModule.cpp \
    FactoryRegisterer.cpp \
    Font.cpp \
    FontManager.cpp \
    Font_xmlHandler.cpp \
    FormattedRenderedString.cpp \
    FreeTypeFont.cpp \
    FribidiVisualMapping.cpp \
    GeometryBuffer.cpp \
    GlobalEventSet.cpp \
    GUIContext.cpp \
    GUILayout_xmlHandler.cpp \
    IconvStringTranscoder.cpp \
    Image.cpp \
    ImageCodec.cpp \
    ImageManager.cpp \
    JustifiedRenderedString.cpp \
    JustifiedRenderedStringWordWrapper.cpp \
    KeyFrame.cpp \
    LeftAlignedRenderedString.cpp \
    LinkedEvent.cpp \
    Logger.cpp \
    minibidi.cpp \
    MinibidiVisualMapping.cpp \
    MinizipResourceProvider.cpp \
    MouseCursor.cpp \
    NamedElement.cpp \
    NamedXMLResourceManager.cpp \
    PCRERegexMatcher.cpp \
    PixmapFont.cpp \
    Property.cpp \
    PropertyHelper.cpp \
    PropertySet.cpp \
    Quaternion.cpp \
    Rect.cpp \
    RenderedString.cpp \
    RenderedStringComponent.cpp \
    RenderedStringImageComponent.cpp \
    RenderedStringTextComponent.cpp \
    RenderedStringWidgetComponent.cpp \
    RenderEffectManager.cpp \
    RenderingSurface.cpp \
    RenderingWindow.cpp \
    RenderQueue.cpp \
    RenderTarget.cpp \
    RightAlignedRenderedString.cpp \
    Scheme.cpp \
    SchemeManager.cpp \
    Scheme_xmlHandler.cpp \
    ScriptModule.cpp \
    SimpleTimer.cpp \
    String.cpp \
    SubscriberSlot.cpp \
    System.cpp \
    SystemKeys.cpp \
    TextUtils.cpp \
    Window.cpp \
    WindowFactory.cpp \
    WindowFactoryManager.cpp \
    WindowManager.cpp \
    WindowRenderer.cpp \
    WindowRendererManager.cpp \
    XMLAttributes.cpp \
    XMLHandler.cpp \
    XMLParser.cpp \
    XMLSerializer.cpp \
    falagard/FalagardXMLHandler.cpp

SOURCES += \
        widgets/ButtonBase.cpp \
        widgets/ClippedContainer.cpp \
        widgets/ComboDropList.cpp \
        widgets/Combobox.cpp \
        widgets/DefaultWindow.cpp \
        widgets/DragContainer.cpp \
        widgets/Editbox.cpp \
        widgets/FrameWindow.cpp \
        widgets/GridLayoutContainer.cpp \
        widgets/GroupBox.cpp \
        widgets/HorizontalLayoutContainer.cpp \
        widgets/ItemEntry.cpp \
        widgets/ItemListBase.cpp \
        widgets/ItemListbox.cpp \
        widgets/LayoutCell.cpp \
        widgets/LayoutContainer.cpp \
        widgets/ListHeader.cpp \
        widgets/ListHeaderSegment.cpp \
        widgets/Listbox.cpp \
        widgets/ListboxItem.cpp \
        widgets/ListboxTextItem.cpp \
        widgets/MenuBase.cpp \
        widgets/MenuItem.cpp \
        widgets/Menubar.cpp \
        widgets/MultiColumnList.cpp \
        widgets/MultiLineEditbox.cpp \
        widgets/PopupMenu.cpp \
        widgets/ProgressBar.cpp \
        widgets/PushButton.cpp \
        widgets/RadioButton.cpp \
        widgets/ScrollablePane.cpp \
        widgets/Scrollbar.cpp \
        widgets/ScrolledContainer.cpp \
        widgets/ScrolledItemListBase.cpp \
        widgets/SequentialLayoutContainer.cpp \
        widgets/Slider.cpp \
        widgets/Spinner.cpp \
        widgets/TabButton.cpp \
        widgets/TabControl.cpp \
        widgets/Thumb.cpp \
        widgets/Titlebar.cpp \
        widgets/ToggleButton.cpp \
        widgets/Tooltip.cpp \
        widgets/Tree.cpp \
        widgets/TreeItem.cpp \
        widgets/VerticalLayoutContainer.cpp

SOURCES += \
        falagard/ComponentBase.cpp \
        falagard/Dimensions.cpp \
        falagard/EventAction.cpp \
        falagard/EventLinkDefinition.cpp \
        falagard/FormattingSetting.cpp \
        falagard/FrameComponent.cpp \
        falagard/ImageryComponent.cpp \
        falagard/ImagerySection.cpp \
        falagard/LayerSpecification.cpp \
        falagard/NamedArea.cpp \
        falagard/PropertyDefinitionBase.cpp \
        falagard/PropertyInitialiser.cpp \
        falagard/SectionSpecification.cpp \
        falagard/StateImagery.cpp \
        falagard/TextComponent.cpp \
        falagard/WidgetComponent.cpp \
        falagard/WidgetLookFeel.cpp \
        falagard/WidgetLookManager.cpp \
        falagard/XMLEnumHelper.cpp


DISTFILES += \
    nutshell.mk
