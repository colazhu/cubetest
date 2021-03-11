LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_RTTI_FLAG := -frtti
LOCAL_MODULE := libCEGuiBase_nx
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES :=  \
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
	falagard/FalagardXMLHandler.cpp \
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
	widgets/VerticalLayoutContainer.cpp \
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
        falagard/XMLEnumHelper.cpp \
#        PCRERegexMatcher.cpp \

LOCAL_C_INCLUDES +=  \
	externals/freetype/include \
	externals/zlib/contrib \
	externals/zlib \
	framework/nxui/CEGUI/CEGUIBase/include
               


LOCAL_SHARED_LIBRARIES += libfreetype libz

LOCAL_CPPFLAGS := -fexceptions -DCONFIG_MACHINE=1

LOCAL_LDFLAGS := -lrt -lpthread -ldl

include $(BUILD_SHARED_LIBRARY)
