LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := libjniCubeRender
LOCAL_CFLAGS    := -Wno-error=unused-parameter
LOCAL_C_INCLUDES := \
        $(LOCAL_PATH)/gesture \
        $(LOCAL_PATH)/gesture/data \
        $(LOCAL_PATH)/math \
        $(LOCAL_PATH)/opengl

LOCAL_SRC_FILES := \
math/BoundingSphere.cpp \
math/BoundingBox.cpp \
math/MathUtil.cpp \
math/Plane.cpp \
math/Ray.cpp \
math/Vector2.cpp \
math/Vector3.cpp \
math/Frustum.cpp \
math/Vector4.cpp \
math/CurveFunction.cpp \
math/Geometry.cpp \
math/Quaternion.cpp \
math/Matrix.cpp \
opengl/Program.cpp \
opengl/Shaders.cpp \
opengl/LightCache.cpp \
opengl/GLFunction.cpp \
opengl/ProgramCache.cpp \
opengl/Light.cpp \
opengl/TextureCache.cpp \
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
ActionCache.cpp \
Cube.cpp \
DataTypes.cpp \
Action.cpp \
CubePlane.cpp \
Director.cpp \
Scene.cpp \
Arcball.cpp \
MatrixStack.cpp \
Camera.cpp \
Log.cpp \
RectNode.cpp \
Node.cpp \
com_example_cube_OpenGLJniLib.cpp \
com_example_cube_SerialPort.cpp \
com_example_cube_SocketJni.cpp


LOCAL_LDLIBS := -llog -lGLESv2 -lEGL
include $(BUILD_SHARED_LIBRARY)
