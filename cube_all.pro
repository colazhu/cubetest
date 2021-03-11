TARGET = CEGUI
TEMPLATE = subdirs

SUBDIRS += cegui
cegui.file = cegui/cegui.pro

SUBDIRS += gesture
gesture.file = gesture/gesture.pro

SUBDIRS += cube
cube.file = cube/cube.pro

SUBDIRS += test
test.file = test/test.pro
