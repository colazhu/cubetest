cmake_minimum_required(VERSION 3.5)

#readme
#mkdir out
#cd out
#cmake ../bk/corelib_new
#make


## -------------------------------------------------------------------- ##
# tst_cube
## -------------------------------------------------------------------- ##
set(PROJ_NAME tst_cube)

set(INC_DIR_LIST
/usr/lib/x86_64-linux-gnu
../
../cegui/CEGUIBase/include
../cegui/CEGUIOpenGLRenderer/include
../base
../gesture
../gesture/data
../cube
../cube/math
../cube/opengl
)

set(SRC_LIST
    ../base/Log.cpp
    main.cpp
    CEGuiRender.cpp
   )
set(CFLAGS -fPIC -frtti -std=c++11 -DCONFIG_MACHINE=1 -Werror=return-type)	
set(CXXFLAGS -fexceptions -DCONFIG_MACHINE=1 -D_LINUX -Werror=return-type)
set(LINK_DIR ../out)
set(LINK_SHAREDLIBS z freetype cube gesture CEGUIBase CEGUIOpenGLRenderer glfw GLESv2 EGL dl pcre)
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
# add_library(${PROJ_NAME} ${LIBTYPE} ${SRC_LIST})
# todo build executable app
add_executable(${PROJ_NAME} ${SRC_LIST})
target_link_libraries(${PROJ_NAME} ${LINK_SHAREDLIBS} ${LINK_STATICLIBS})

## -------------------------------------------------------------------- ##

