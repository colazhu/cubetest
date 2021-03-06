/* DO NOT EDIT THIS FILE - it is machine generated */
#include "com_example_cube_OpenGLJniLib.h"
#include "sys/types.h"
#include "stdio.h"
#include "unistd.h"

#include <android/log.h>
#define LOGJNI(...) __android_log_print(ANDROID_LOG_DEBUG, "Cube", __VA_ARGS__)

#include "Director.h"

class CubeCbImpl : public CubeCallback
{
public:
    virtual ~CubeCbImpl() {}
    virtual void onTexturePicked(int txtid);
};

#ifdef __cplusplus
extern "C" {
#endif

static JavaVM* 	    s_jvm = NULL;
static jclass       s_cls = NULL;
static jmethodID    s_methodid_textureSelected = NULL;


void Callback_OnTextureSelected(int txtid)
{
    JNIEnv* env = NULL;
    s_jvm->AttachCurrentThread(&env, 0);
    LOGJNI("CallStaticVoidMethod env:%p cls:%p method:%p ++", env, s_cls, s_methodid_textureSelected);
    if (env && s_cls && s_methodid_textureSelected) {
        env->CallStaticVoidMethod(s_cls, s_methodid_textureSelected, txtid);
    }
}

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    getFPS
 * Signature: ()F
 */
jfloat Java_com_example_cube_OpenGLJniLib_getFPS
(JNIEnv *env, jclass cls)
{
    LOGJNI("getFPS");
    return Director::instance()->getFps();
}

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    longclickScene
 * Signature: ()I
 */
jint JNICALL Java_com_example_cube_OpenGLJniLib_longclickScene
(JNIEnv *env, jclass cls)
{
    LOGJNI("longclickScene +++");
    int ret = Director::instance()->longclickScene();
    LOGJNI("longclickScene:%d ---", ret);
    return ret;
}

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    addTexture
 * Signature: (II)V
 */
void Java_com_example_cube_OpenGLJniLib_addTexture
(JNIEnv *env, jclass cls, jint id, jint txtid, jint w, jint h)
{
    LOGJNI("addTexture id:%d txt:%d w:%d h:%d", id, txtid, w, h);
    Director::instance()->addTexture(id, txtid, w, h);
}

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    addTextureData
 * Signature: (ILjava/nio/ByteBuffer;II)V
 */
void Java_com_example_cube_OpenGLJniLib_addTextureData
(JNIEnv *env, jclass cls, jint id, jobject obj, jint w, jint h)
{
    return;
    // LOGJNI("addTextureData 000");
    // void* buff = env->GetDirectBufferAddress(obj);    
    // jlong buffsize = env->GetDirectBufferCapacity(obj);
    // LOGJNI("addTextureData :%p %d", buff, buffsize);

    // if (buff == NULL) {
    //     char* data = new char[w*h*4];
    //     for (int i = 0; i < w*h*4; i=i+4) {
    //         data[i] = 0x8f;
    //         data[i+1] = 0;
    //         data[i+2] = 0;
    //         data[i+3] = 0x8f;
    //     }
    //     buff = (void*)data;
    //     LOGJNI("addTextureData badcase!!!");
    // }
    // LOGJNI("addTextureData id:%d buff:%p w:%d h:%d", id, buff, w, h);
    // Director::instance()->addTexture(id, buff, w, h);
}

void Java_com_example_cube_OpenGLJniLib_addTextureByArray
(JNIEnv *env, jclass cls, jint id, jbyteArray data, jint size, jint w, jint h)
{
    jbyte *bytes = env->GetByteArrayElements(data, 0);
    int chars_len = env->GetArrayLength(data);
    char *chars = new char[chars_len + 1];
    memcpy(chars, bytes, chars_len);
    chars[chars_len] = 0;
    env->ReleaseByteArrayElements(data, bytes, 0);

    LOGJNI("addTextureData id:%d buff:%p w:%d h:%d", id, chars, w, h);
    Director::instance()->addTexture(id, (void*)chars, w, h);
    delete [] chars;
    chars = NULL; 
}

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    addTexture
 * Signature: (II)V
 */
void Java_com_example_cube_OpenGLJniLib_setPlaneTexture
(JNIEnv *env, jclass cls, jint id, jint txtid)
{
    LOGJNI("setPlaneTexture plane:%d txt:%d", id, txtid);
    Director::instance()->setPlaneTexture(id, txtid);
}

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    rotateCube
 * Signature: (FF)V
 */
void Java_com_example_cube_OpenGLJniLib_rotateCube
(JNIEnv *env, jclass cls, jfloat degrees, jfloat ms)
{
    LOGJNI("rotateCube degrees:%.2f time:%.1f", degrees, ms);
    Director::instance()->rotateCube(degrees, ms);
}

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    isEnable
 * Signature: ()Z
 */
jboolean JNICALL Java_com_example_cube_OpenGLJniLib_isEnable
  (JNIEnv *env, jclass cls)
{  
    bool enable = Director::instance()->isEnable(); 
    LOGJNI("isEnable:%d", enable);
    return enable;
}

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    setMode
 * Signature: (I)V
 */
void Java_com_example_cube_OpenGLJniLib_setMode
(JNIEnv *env, jclass cls, jint mode)
{
    LOGJNI("setMode mode:%d", mode);
    Director::instance()->setMode(mode);
}
  
/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    setLayout
 * Signature: (I)V
 */
void Java_com_example_cube_OpenGLJniLib_setLayout
(JNIEnv *env, jclass cls, jint layout)
{
    LOGJNI("setLayout layout:%d", layout);
    Director::instance()->setLayout(layout);
}

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    initWindow
 * Signature: (II)V
 */
void Java_com_example_cube_OpenGLJniLib_initWindow
(JNIEnv *env, jclass cls, jint w, jint h)
{
    LOGJNI("initWindow");
    setuid(0);
    Director::instance()->init();
    LOGJNI("setWindowSize %d %d", w, h);
    Director::instance()->setWindowSize(w, h);
    // Director::instance()->addCubeCallback(new CubeCbImpl());
}

void JNICALL Java_com_example_cube_OpenGLJniLib_setTouchRegion
  (JNIEnv *env, jclass cls, jint x, jint y, jint w, jint h)
{
    LOGJNI("setTouchRegion: x:%d y:%d w:%d h:%d", x, y, w, h);
    Director::instance()->setTouchRegion(x, y, w, h);
}

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    injectTouch
 * Signature: (IFF)V
 */
void Java_com_example_cube_OpenGLJniLib_injectTouch
  (JNIEnv *env, jclass cls, jint id, jint action, jfloat x, jfloat y)
{
    // LOGJNI("injectTouch id:%d action:%d x:%.1f y:%.1f", id, action, x, y);
    TouchEvent event;
    event.action = action;
    event.id = id;
    event.x = x;
    event.y = y;
    Director::instance()->injectTouch(event);
}

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    loopOnce
 * Signature: ()I
 */
jint Java_com_example_cube_OpenGLJniLib_loopOnce
  (JNIEnv *env, jclass cls)
{
    // LOGJNI("loop");
    Director::instance()->loop();
    return Director::instance()->popSelectedTexture();
}

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    pause
 * Signature: ()V
 */
void Java_com_example_cube_OpenGLJniLib_pause
  (JNIEnv *env, jclass cls)
{
    LOGJNI("pause");
    // Director::instance()->pause();
}

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    resume
 * Signature: ()V
 */
void Java_com_example_cube_OpenGLJniLib_resume
  (JNIEnv *env, jclass cls)
{
    LOGJNI("resume");
    // Director::instance()->resume();
}

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    init
 * Signature: ()V
 */
void JNICALL Java_com_example_cube_OpenGLJniLib_init
  (JNIEnv *env, jclass cls)
{
    LOGJNI("init");
    env->GetJavaVM(&s_jvm);
    s_cls = (jclass)(env->NewGlobalRef(cls));
    LOGJNI("init jvm:%p cls:%p", s_jvm, s_cls);
    // s_methodid_textureSelected = env->GetStaticMethodID(cls, "textureselected", "(I)V");
    // s_methodid_textureSelected = env->GetMethodID(cls, "textureselected", "(I)V");
}

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    deinit
 * Signature: ()V
 */
void Java_com_example_cube_OpenGLJniLib_deinit
  (JNIEnv *env, jclass cls)
{
    LOGJNI("deinit");
    // Director::instance()->deinit();
}

/*
 * Class:     com_example_cube_OpenGLJniLib
 * Method:    getEvent
 * Signature: ()I
 */
jint JNICALL Java_com_example_cube_OpenGLJniLib_getEvent
  (JNIEnv *env, jclass cls)
{    
    int event = Director::instance()->popEvent();
    // LOGJNI("getEvent:%d", event);
    return event;
}

#ifdef __cplusplus
}

void CubeCbImpl::onTexturePicked(int txtid) {
        Callback_OnTextureSelected(txtid);
}

#endif
