TARGET = external
TEMPLATE = subdirs

SUBDIRS += zlib
zlib.file = zlib/zlib.pro

#SUBDIRS += glm
#glm.file = glm/glm.pro

SUBDIRS += freetype
freetype.file = freetype/freetype.pro

SUBDIRS += libpng
libpng.file = libpng/libpng.pro

SUBDIRS += FreeImage
FreeImage.file = FreeImage/FreeImage.pro

#SUBDIRS += json
#json.file = json/json.pro
