TARGET = CEGUI
TEMPLATE = subdirs

SUBDIRS += CEGUIBase
CEGUIBase.file = CEGUIBase/CEGUIBase.pro

SUBDIRS += CEGUIOpenGLRenderer
CEGUIOpenGLRenderer.file = CEGUIOpenGLRenderer/CEGUIOpenGLRenderer.pro

SUBDIRS += CEGUILibXMLParser
CEGUILibXMLParser.file = CEGUILibXMLParser/CEGUILibXMLParser.pro

SUBDIRS += CEGUIFreeImageImageCodec
CEGUIFreeImageImageCodec.file = CEGUIFreeImageImageCodec/CEGUIFreeImageImageCodec.pro

SUBDIRS += CEGUICoreWindowRendererSet
CEGUICoreWindowRendererSet.file = CEGUICoreWindowRendererSet/CEGUICoreWindowRendererSet.pro

SUBDIRS += external
external.file = external/external.pro

