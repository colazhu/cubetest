TEMPLATE = lib
TARGET = gesture

CONFIG -= app_bundle gui core
QT -= app_bundle gui core
CONFIG += c++11

DESTDIR  = $$(PROJROOT)/out
LIBS += -L$$DESTDIR

GESTURE_PATH = \
    $$(PROJROOT)/base \
    data \

DEPENDPATH += $$GESTURE_PATH
INCLUDEPATH += $$GESTURE_PATH

HEADERS += \
    gestureDef.h \
    gestureObject.h \
    gesturePublicDef.h \
    gestureRecognizer.h \
    gestureFactory.h \
    gestureManager.h \
    gestureRecognizerFactory.h \
    gestureRecognizerManager.h \
    data/doubleClickGesture.h \
    data/doubleClickGestureRecognizer.h \
    data/dragGesture.h \
    data/dragGestureRecognizer.h \
    data/flickGesture.h \
    data/flickGestureRecognizer.h \
    data/longPressGesture.h \
    data/longPressGestureRecognizer.h \
    data/multiLongPressGesture.h \
    data/multiLongPressGestureRecognizer.h \
    data/pinchGesture.h \
    data/pinchGestureRecognizer.h \
    data/rotateGesture.h \
    data/rotateGestureRecognizer.h \
    data/scratchGesture.h \
    data/scratchGestureRecognizer.h \
    data/tapGesture.h \
    data/tapGestureRecognizer.h \
    data/threeFlickGesture.h \
    data/threeFlickGestureRecognizer.h \
    data/twoDragGesture.h \
    data/twoDragGestureRecognizer.h \
    data/twoFlickGesture.h \
    data/twoFlickGestureRecognizer.h \
    data/twoLongPressGesture.h \
    data/twoLongPressGestureRecognizer.h \
    data/twoTapGesture.h \
    data/twoTapGestureRecognizer.h \
    gestureCommonFun.h \
    gestureTimer.h \
    gestureTimerManager.h \


SOURCES += \
    gestureCommonFun.cpp \
    gestureTimer.cpp \
    gestureObject.cpp \
    gestureRecognizer.cpp \
    gestureFactory.cpp \
    gestureManager.cpp \
    gestureRecognizerFactory.cpp \
    gestureRecognizerManager.cpp \
    data/doubleClickGesture.cpp \
    data/doubleClickGestureRecognizer.cpp \
    data/dragGesture.cpp \
    data/dragGestureRecognizer.cpp \
    data/flickGesture.cpp \
    data/flickGestureRecognizer.cpp \
    data/longPressGesture.cpp \
    data/longPressGestureRecognizer.cpp \
    data/multiLongPressGesture.cpp \
    data/multiLongPressGestureRecognizer.cpp \
    data/pinchGesture.cpp \
    data/pinchGestureRecognizer.cpp \
    data/rotateGesture.cpp \
    data/rotateGestureRecognizer.cpp \
    data/scratchGesture.cpp \
    data/scratchGestureRecognizer.cpp \
    data/tapGesture.cpp \
    data/tapGestureRecognizer.cpp \
    data/threeFlickGesture.cpp \
    data/threeFlickGestureRecognizer.cpp \
    data/twoDragGesture.cpp \
    data/twoDragGestureRecognizer.cpp \
    data/twoFlickGesture.cpp \
    data/twoFlickGestureRecognizer.cpp \
    data/twoLongPressGesture.cpp \
    data/twoLongPressGestureRecognizer.cpp \
    data/twoTapGesture.cpp \
    data/twoTapGestureRecognizer.cpp \
    gestureTimerManager.cpp


DISTFILES += \
