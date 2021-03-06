/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_example_cube_OpenGLJniLib */

#ifndef _Included_com_example_cube_OpenGLJniLib
#define _Included_com_example_cube_OpenGLJniLib
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    getFPS
 * Signature: ()F
 */
JNIEXPORT jfloat JNICALL Java_com_example_cube_OpenGLJniLib_getFPS
  (JNIEnv *, jclass);

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    longclickScene
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_example_cube_OpenGLJniLib_longclickScene
  (JNIEnv *, jclass);

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    addTexture
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_example_cube_OpenGLJniLib_addTexture
  (JNIEnv *, jclass, jint, jint, jint, jint);

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    addTextureData
 * Signature: (ILjava/nio/ByteBuffer;II)V
 */
JNIEXPORT void JNICALL Java_com_example_cube_OpenGLJniLib_addTextureData
  (JNIEnv *, jclass, jint, jobject, jint, jint);

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    addTextureByArray
 * Signature: (I[BIII)V
 */
JNIEXPORT void JNICALL Java_com_example_cube_OpenGLJniLib_addTextureByArray
  (JNIEnv *, jclass, jint, jbyteArray, jint, jint, jint);

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    setPlaneTexture
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_example_cube_OpenGLJniLib_setPlaneTexture
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    rotateCube
 * Signature: (FF)V
 */
JNIEXPORT void JNICALL Java_com_example_cube_OpenGLJniLib_rotateCube
  (JNIEnv *, jclass, jfloat, jfloat);

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    isEnable
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_example_cube_OpenGLJniLib_isEnable
  (JNIEnv *, jclass);

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    setMode
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_example_cube_OpenGLJniLib_setMode
  (JNIEnv *, jclass, jint);

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    setLayout
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_example_cube_OpenGLJniLib_setLayout
  (JNIEnv *, jclass, jint);

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    initWindow
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_example_cube_OpenGLJniLib_initWindow
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    setTouchRegion
 * Signature: (IIII)V
 */
JNIEXPORT void JNICALL Java_com_example_cube_OpenGLJniLib_setTouchRegion
  (JNIEnv *, jclass, jint, jint, jint, jint);

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    injectTouch
 * Signature: (IIFF)V
 */
JNIEXPORT void JNICALL Java_com_example_cube_OpenGLJniLib_injectTouch
  (JNIEnv *, jclass, jint, jint, jfloat, jfloat);

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    loopOnce
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_example_cube_OpenGLJniLib_loopOnce
  (JNIEnv *, jclass);

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    pause
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_example_cube_OpenGLJniLib_pause
  (JNIEnv *, jclass);

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    resume
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_example_cube_OpenGLJniLib_resume
  (JNIEnv *, jclass);

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_example_cube_OpenGLJniLib_init
  (JNIEnv *, jclass);

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    deinit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_example_cube_OpenGLJniLib_deinit
  (JNIEnv *, jclass);

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    getEvent
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_example_cube_OpenGLJniLib_getEvent
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif
