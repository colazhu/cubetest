TEMPLATE = lib
TARGET = cube

CONFIG -= app_bundle gui core
QT -= app_bundle gui core
CONFIG += c++11

DEFINES+=USE_CEGUI

DESTDIR  = $$(PROJROOT)/out

LIBS += -lglfw -lGLESv2 -lEGL
LIBS += -L$$DESTDIR
include($$(PROJROOT)/cegui/CEGUIBase/CEGUIBase.pri)
include($$(PROJROOT)/cegui/CEGUIOpenGLRenderer/CEGUIOpenGLRenderer.pri)
CUBE_PATH = \
    $$(PROJROOT)/base \
    $$(PROJROOT)/gesture \
    $$(PROJROOT)/gesture/data \
    math \
    opengl

DEPENDPATH += $$CUBE_PATH
INCLUDEPATH += $$CUBE_PATH

HEADERS += \
    math/BoundingBox.h \
    math/BoundingSphere.h \
    math/Frustum.h \
    math/Geometry.h \
    math/MathInc.h \
    math/MathUtil.h \
    math/Matrix.h \
    math/Plane.h \
    math/Quaternion.h \
    math/Ray.h \
    math/Vector2.h \
    math/Vector3.h \
    math/Vector4.h \
    math/CurveFunction.h \
    opengl/Light.h \
    opengl/Program.h \
    opengl/GLFunction.h \
    opengl/TextureCache.h \
    opengl/ProgramCache.h \
    opengl/Shaders.h \
    opengl/LightCache.h \
    Arcball.h \
    Camera.h \
    Director.h \
    Node.h \
    Scene.h \
    Cube.h \
    NodeDef.h \
    MatrixStack.h \
    DataTypes.h \
    ActionCache.h \
    Action.h \
    CubePlane.h \
    RectNode.h \
    CubeDef.h

SOURCES += \
    math/BoundingBox.cpp \
    math/BoundingSphere.cpp \
    math/Frustum.cpp \
    math/Geometry.cpp \
    math/MathUtil.cpp \
    math/Matrix.cpp \
    math/Plane.cpp \
    math/Quaternion.cpp \
    math/Ray.cpp \
    math/Vector2.cpp \
    math/Vector3.cpp \
    math/Vector4.cpp \
    math/CurveFunction.cpp \
    opengl/TextureCache.cpp \
    opengl/ProgramCache.cpp \
    opengl/Shaders.cpp \
    opengl/LightCache.cpp \
    opengl/Program.cpp \
    opengl/GLFunction.cpp \
    opengl/Light.cpp \
    Arcball.cpp \
    Camera.cpp \
    Director.cpp \
    Node.cpp \
    Scene.cpp \
    Cube.cpp \
    MatrixStack.cpp \
    DataTypes.cpp \
    ActionCache.cpp \
    Action.cpp \
    CubePlane.cpp \
    RectNode.cpp

DISTFILES += \
    Android.mk \
    opengl/PositionColor.frag \
    opengl/PositionColor.vert \
    opengl/TestShader.frag \
    opengl/Lighting.vert \
    opengl/TestShader.vert \
    opengl/Position.frag \
    opengl/PositionColorSize.frag \
    opengl/Position.vert \
    opengl/PositionColorSize.vert \
    opengl/PositionTexture.frag \
    opengl/PositionTexture.vert \
    opengl/PositionTextureNoMVP.frag \
    opengl/PositionTextureNoMVP.vert \
    opengl/TestMirrorShader.vert
