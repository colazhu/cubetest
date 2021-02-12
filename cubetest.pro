#TEMPLATE = lib
#TARGET = libcube

TEMPLATE = app
TARGET = tst_cube

CONFIG -= app_bundle gui core
CONFIG += c++11

DESTDIR  = ../out

LIBS += -lglfw -lGLESv2 -lEGL
LIBS += -L$$DESTDIR

CUBE_PATH = \
    gesture \
    gesture/data \
    cube \
    cube/math \
    cube/opengl

DEPENDPATH += $$CUBE_PATH
INCLUDEPATH += $$CUBE_PATH

HEADERS += \
    gesture/gestureDef.h \
    gesture/gestureObject.h \
    gesture/gesturePublicDef.h \
    gesture/gestureRecognizer.h \
    gesture/gestureFactory.h \
    gesture/gestureManager.h \
    gesture/gestureRecognizerFactory.h \
    gesture/gestureRecognizerManager.h \
    gesture/data/doubleClickGesture.h \
    gesture/data/doubleClickGestureRecognizer.h \
    gesture/data/dragGesture.h \
    gesture/data/dragGestureRecognizer.h \
    gesture/data/flickGesture.h \
    gesture/data/flickGestureRecognizer.h \
    gesture/data/longPressGesture.h \
    gesture/data/longPressGestureRecognizer.h \
    gesture/data/multiLongPressGesture.h \
    gesture/data/multiLongPressGestureRecognizer.h \
    gesture/data/pinchGesture.h \
    gesture/data/pinchGestureRecognizer.h \
    gesture/data/rotateGesture.h \
    gesture/data/rotateGestureRecognizer.h \
    gesture/data/scratchGesture.h \
    gesture/data/scratchGestureRecognizer.h \
    gesture/data/tapGesture.h \
    gesture/data/tapGestureRecognizer.h \
    gesture/data/threeFlickGesture.h \
    gesture/data/threeFlickGestureRecognizer.h \
    gesture/data/twoDragGesture.h \
    gesture/data/twoDragGestureRecognizer.h \
    gesture/data/twoFlickGesture.h \
    gesture/data/twoFlickGestureRecognizer.h \
    gesture/data/twoLongPressGesture.h \
    gesture/data/twoLongPressGestureRecognizer.h \
    gesture/data/twoTapGesture.h \
    gesture/data/twoTapGestureRecognizer.h \
    gesture/gestureCommonFun.h \
    gesture/gestureTimer.h \
    gesture/gestureTimerManager.h \
    cube/Flags.h \
    cube/math/BoundingBox.h \
    cube/math/BoundingSphere.h \
    cube/math/Frustum.h \
    cube/math/Geometry.h \
    cube/math/MathInc.h \
    cube/math/MathUtil.h \
    cube/math/Matrix.h \
    cube/math/Plane.h \
    cube/math/Quaternion.h \
    cube/math/Ray.h \
    cube/math/Vector2.h \
    cube/math/Vector3.h \
    cube/math/Vector4.h \
    cube/Arcball.h \
    cube/Camera.h \
    cube/Director.h \
    cube/opengl/Light.h \
    cube/Node.h \
    cube/Scene.h \
    cube/Cube.h \
    cube/NodeDef.h \
    cube/Log.h \
    cube/opengl/Program.h \
    cube/opengl/GLFunction.h \
    cube/MatrixStack.h \
    cube/DataTypes.h \
    cube/opengl/TextureCache.h \
    cube/opengl/ProgramCache.h \
    cube/opengl/Shaders.h \
    cube/opengl/LightCache.h \
    cube/ActionCache.h \
    cube/Action.h \
    cube/math/CurveFunction.h \
    cube/CubePlane.h \
    cube/Base.h \
    cube/RectNode.h

SOURCES += \
    gesture/gestureCommonFun.cpp \
    gesture/gestureTimer.cpp \
    gesture/gestureObject.cpp \
    gesture/gestureRecognizer.cpp \
    gesture/gestureFactory.cpp \
    gesture/gestureManager.cpp \
    gesture/gestureRecognizerFactory.cpp \
    gesture/gestureRecognizerManager.cpp \
    gesture/data/doubleClickGesture.cpp \
    gesture/data/doubleClickGestureRecognizer.cpp \
    gesture/data/dragGesture.cpp \
    gesture/data/dragGestureRecognizer.cpp \
    gesture/data/flickGesture.cpp \
    gesture/data/flickGestureRecognizer.cpp \
    gesture/data/longPressGesture.cpp \
    gesture/data/longPressGestureRecognizer.cpp \
    gesture/data/multiLongPressGesture.cpp \
    gesture/data/multiLongPressGestureRecognizer.cpp \
    gesture/data/pinchGesture.cpp \
    gesture/data/pinchGestureRecognizer.cpp \
    gesture/data/rotateGesture.cpp \
    gesture/data/rotateGestureRecognizer.cpp \
    gesture/data/scratchGesture.cpp \
    gesture/data/scratchGestureRecognizer.cpp \
    gesture/data/tapGesture.cpp \
    gesture/data/tapGestureRecognizer.cpp \
    gesture/data/threeFlickGesture.cpp \
    gesture/data/threeFlickGestureRecognizer.cpp \
    gesture/data/twoDragGesture.cpp \
    gesture/data/twoDragGestureRecognizer.cpp \
    gesture/data/twoFlickGesture.cpp \
    gesture/data/twoFlickGestureRecognizer.cpp \
    gesture/data/twoLongPressGesture.cpp \
    gesture/data/twoLongPressGestureRecognizer.cpp \
    gesture/data/twoTapGesture.cpp \
    gesture/data/twoTapGestureRecognizer.cpp \
    gesture/gestureTimerManager.cpp \
    main.cpp \
    cube/math/BoundingBox.cpp \
    cube/math/BoundingSphere.cpp \
    cube/math/Frustum.cpp \
    cube/math/Geometry.cpp \
    cube/math/MathUtil.cpp \
    cube/math/Matrix.cpp \
    cube/math/Plane.cpp \
    cube/math/Quaternion.cpp \
    cube/math/Ray.cpp \
    cube/math/Vector2.cpp \
    cube/math/Vector3.cpp \
    cube/math/Vector4.cpp \
    cube/Arcball.cpp \
    cube/Camera.cpp \
    cube/Director.cpp \
    cube/opengl/Light.cpp \
    cube/Node.cpp \
    cube/Scene.cpp \
    cube/Cube.cpp \
    cube/Log.cpp \
    cube/opengl/Program.cpp \
    cube/opengl/GLFunction.cpp \
    cube/MatrixStack.cpp \
    cube/DataTypes.cpp \
    cube/opengl/TextureCache.cpp \
    cube/opengl/ProgramCache.cpp \
    cube/opengl/Shaders.cpp \
    cube/opengl/LightCache.cpp \
    cube/ActionCache.cpp \
    cube/Action.cpp \
    cube/math/CurveFunction.cpp \
    cube/CubePlane.cpp \
    cube/RectNode.cpp

DISTFILES += \
    Android.mk \
    cube/opengl/PositionColor.frag \
    cube/opengl/PositionColor.vert \
    cube/opengl/TestShader.frag \
    cube/opengl/Lighting.vert \
    cube/opengl/TestShader.vert \
    cube/opengl/Position.frag \
    cube/opengl/PositionColorSize.frag \
    cube/opengl/Position.vert \
    cube/opengl/PositionColorSize.vert \
    cube/opengl/PositionTexture.frag \
    cube/opengl/PositionTexture.vert
