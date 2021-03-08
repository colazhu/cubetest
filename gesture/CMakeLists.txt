cmake_minimum_required(VERSION 3.5)

## -------------------------------------------------------------------- ##
# libgesture
## -------------------------------------------------------------------- ##
set(PROJ_NAME libgesture)

set(GESTURE_ROOT /home/colazhu/takoyaki/takoyaki)

set(INC_DIR_LIST
/usr/lib/x86_64-linux-gnu
./data
./
)

set(SRC_LIST
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
    gestureCommonFun.cpp \
    gestureTimer.cpp \
    gestureObject.cpp \
    gestureRecognizer.cpp \
    gestureFactory.cpp \
    gestureManager.cpp \
    gestureRecognizerFactory.cpp \
    gestureRecognizerManager.cpp \
    gestureTimerManager.cpp
   )
set(CFLAGS -fPIC -frtti -std=c++11 -Werror=return-type)	
set(CXXFLAGS -fexceptions -D_LINUX -Werror=return-type)
set(LINK_DIR )
set(LINK_SHAREDLIBS )
set(LINK_STATICLIBS )
set(LINK_FLAGS )
set(LIBTYPE SHARED) # STATIC SHARED

## ----- cmake set ----- ##
message("${PROJ_NAME} begin cmake")
message("${PROJ_NAME} INC_DIR_LIST=" ${INC_DIR_LIST})
message("${PROJ_NAME} SRC_LIST=" ${SRC_LIST})
project(${PROJ_NAME})
include_directories(${INC_DIR_LIST})
link_directories(${LINK_DIR})
add_compile_options(${CFLAGS})
#set(CMAKE_C_FLAGS ${CFLAGS})
#set(CMAKE_CXX_FLAGS ${CXXFLAGS})
set(CMAKE_SHARED_LINKER_FLAGS ${LINK_FLAGS})
# todo build static lib or shared list
add_library(${PROJ_NAME} ${LIBTYPE} ${SRC_LIST})
# todo build executable app
# add_executable(${PROJ_NAME} ${SRC_LIST})
# target_link_libraries(${PROJ_NAME} ${LINK_SHAREDLIBS} ${LINK_STATICLIBS})

## -------------------------------------------------------------------- ##

