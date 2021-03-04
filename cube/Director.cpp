#include "Director.h"
#include "Camera.h"
#include "Scene.h"
#include "Matrix.h"
#include "Flags.h"
#include "GLFunction.h"
#include "MatrixStack.h"
#include "DataTypes.h"
#include "TextureCache.h"
#include "ProgramCache.h"
#include "LightCache.h"
#include "ActionCache.h"
#include "RectNode.h"
#include "gestureManager.h"
#include "gestureObject.h"
#include "gestureCommonFun.h"
#include <unistd.h>

#define CAMERA_DEFAULT "camera_default"
#define SCENE_DEFAULT "scene_default"

#define CAMERA_EYE Vector3(0, 0, 15)
#define CAMERA_DIR Vector3(0, 0,-1)

void onGestureEvent(const struct Gesture_Event_Data_List* list, void *data)
{
    if (!list) {
        return;
    }

    LOG_BASE("onGestureEvent: eventlist:%p data:%p +++", list, data);
    // GestureCommonFun::printGestureDataList(list);

    const Gesture_Event_Data_List* curList = list;
    while (curList) {
        GestureObject* gesture = GestureCommonFun::gestureToObject(curList->data);
        if (gesture) {
            Director::instance()->injectGesture(*gesture);
            delete gesture;
            gesture = NULL;
        }
        curList = curList->next;
    }
    // printf("\n onGestureEvent --- \n");

}

struct MouseState
{
    enum MouseButtonState{
        MouseButtonLeft = 0x0001,
        MouseButtonRight = 0x0002,
        MouseButtonMiddle = 0x0004,
    };
    DECLARE_FLAGS(MouseButtonStates, MouseButtonState)

    void reset() {
        mousestates = 0;
        x = 0;
        y = 0;
    }

    MouseButtonStates mousestates;
    float x;
    float y;
};

enum {
    TEXTURE_SCREEN = 1000,
    TEXTURE_LIGHT = 1001,
};

class DirectorPrivate
{
public:
    DirectorPrivate():
      totalFrames(0),
      lastTickCount(0),
      inited(false),
      paused(false),
      layout(LAYOUT_HALF_LANDSCAPE),
      winSize(0, 0, 800, 480),
      curcamera(0),
      curscene(0),
      frameBufferScreen(0),
      fboNodeScreen(0),
      frameBufferLight(0),
      fboNodeLight(0),
      fboBmp(0),
      fboBmpSize(0)
    {

    }

    ~DirectorPrivate()
    {

    }

    int totalFrames;
    unsigned long lastTickCount;
    bool inited;
    bool paused;
    int layout;

    Rect winSize;
    MatrixStack matrixStack;
    MouseState mouseState;

    TextureCache textureCache;
    ProgramCache programCache;
    LightCache lightCache;
    ActionCache actionCache;

    std::map<std::string, Camera*> cameras;
    Camera* curcamera;

    std::map<std::string, Scene*> scenes;
    Scene* curscene;

    GLuint frameBufferScreen;
    GLuint frameBufferLight;
    RectNode* fboNodeScreen;
    RectNode* fboNodeLight;
    unsigned char* fboBmp;
    int fboBmpSize;

    DISABLE_COPY(DirectorPrivate)
};


Director* Director::s_instance = NULL;

Director* Director::instance() {
    if (s_instance == NULL) {
        s_instance = new Director();
    }
    return s_instance;
}

void Director::destroy() {
    if (s_instance) {
        delete s_instance;
        s_instance = NULL;
    }
}

Director::Director():
    m_data(new DirectorPrivate)
{

}

Director::~Director()
{
    if (m_data) {
        delete m_data;
        m_data = NULL;
    }
}

void Director::init()
{
    if (!m_data || m_data->inited) {
        return;
    }
    m_data->inited = true;

    GestureManager::instance()->setNotifyFunc(onGestureEvent);
    GestureManager::instance()->enableSystemGesture(WL_SYSTEM_GESTURE_TYPE_FLAG_ALL);
    GestureManager::instance()->enableCommonGesture(this, WL_COMMON_GESTURE_TYPE_FLAG_ALL);

    GLHook::loadGL();
    // textureCache().addImage("CubeImage", "ic_launcher.png");
    // textureCache().addImage("CubeImage1", "BackgroundSampleBrowser.jpg");
    // textureCache().addImage("CubeImage2", "RestaurantGameSampleGameOver.png");
    // textureCache().addImage("CubeImage3", "Cube3.bmp");
    // textureCache().addImage("CubeImage4", "Cube4.png");
    // textureCache().addImage("CubeImage5", "Cube5.png");
  
    programCache().loadDefaultPrograms();


//    lightcache.setGlobalAmbientColor(Color4F::WHITE);



    // init camera
    Camera* camera = Camera::createCamera3D(CAMERA_DEFAULT, CAMERA_EYE, CAMERA_DIR, m_data->winSize);
    m_data->curcamera = camera;
    m_data->cameras[CAMERA_DEFAULT] = camera;

    // init scene
    m_data->curscene = new Scene(SCENE_DEFAULT);
    m_data->curscene->init();
    m_data->scenes[SCENE_DEFAULT] = m_data->curscene;

    initFBO();
}

void Director::deinit()
{
    m_data->actionCache.stopAll();

    m_data->curscene = NULL;
    for (std::map<std::string, Scene*>::iterator it = m_data->scenes.begin();  it != m_data->scenes.end(); ++it) {
        Scene* scene = it->second;
        if (scene) {
            scene->deinit();
            delete scene;
        }
    }
    m_data->scenes.clear();
}

void Director::pause()
{
    m_data->paused = true;
}

void Director::resume()
{
    m_data->paused = false;
}

bool Director::loop()
{
    GestureManager::instance()->update();

    if (0 == m_data->lastTickCount) {
        // skip first time
        m_data->lastTickCount = Log::tickCount();
        return false;
    }

    unsigned long curTickCount = Log::tickCount();
    float dtms = (curTickCount - m_data->lastTickCount);
    update(dtms);
    bool ret = render();
    m_data->lastTickCount = curTickCount;
    ++m_data->totalFrames;
    return ret;
}

float Director::getFps()
{
    return 0;
}

void Director::addTexture(int txtid, int gltxtid, int w, int h)
{
    textureCache().addImage(txtid, gltxtid, w, h);
}

void Director::addTexture(int txtid, void* buff, int w, int h)
{
    textureCache().addImage(txtid, buff, w, h);
}

void Director::setPlaneTexture(int planeid, int txtid)
{
    if (currentScene()) {
        currentScene()->setCubePlaneTexture(planeid, txtid);
    }
}

void Director::rotateCube(float degrees, float ms)
{
    if (currentScene()) {
        currentScene()->rotateScene(degrees, ms);
    }
}

void Director::setMode(int mode)
{
    if (currentScene()) {
        currentScene()->setGyroMode(mode);
    }
}

void Director::setLayout(int layout)
{
    if (layout == m_data->layout) {
        return;
    }

    m_data->layout = layout;
    if (currentScene()) {
        currentScene()->setLayout(layout);
    }
}

int Director::getLayout()
{
    return m_data->layout;
}

int Director::longclickScene()
{
    if (currentScene()) {
        return currentScene()->longclickScene();
    }
    return -1;
}

void Director::addCubeCallback(CubeCallback* cb)
{
    if (currentScene()) {
        currentScene()->addCubeCallback(cb);
    }
}

int Director::popSelectedTexture()
{
    if (currentScene()) {
        return currentScene()->popSelectedTexture();
    }
    return -1;
}

void Director::injectTouch(const TouchEvent& event)
{
    int motionEvent = 0;
    switch (event.action) {
    case TouchAction_Down:
        motionEvent = MOTION_EVENT_ACTION_DOWN;
        break;
    case TouchAction_Up:
        motionEvent = MOTION_EVENT_ACTION_UP;
        break;
    case TouchAction_Move:
        motionEvent = MOTION_EVENT_ACTION_MOVE;
        break;
    default:
        return;
    }
    GestureManager::instance()->processTouchEvent(motionEvent, 0, event.id, event.x, event.y);

    if (currentScene()) {
        currentScene()->onTouch((TouchAction)event.action, event.x, event.y);
    }
}

void Director::injectGesture(const GestureObject& ev)
{
    if (currentScene()) {
        currentScene()->onGesture(const_cast<GestureObject&>(ev));
    }
}

void Director::setWindowSize(int width, int height)
{
    setTouchRegion(0, 0, width, height);
    m_data->winSize = Rect(0, 0, width, height);
    m_data->curcamera->init(CAMERA_EYE, CAMERA_DIR, Rect(0.0f, 0.0f, width, height));
    m_data->matrixStack.loadMatrix(MATRIX_STACK_PROJECTION, m_data->curcamera->projection());
    m_data->matrixStack.loadIdentityMatrix(MATRIX_STACK_MODELVIEW);
    if (currentScene()) {
        currentScene()->updateArcball();
    }
}

Rect Director::getWindowSize()
{
    return m_data->winSize;
}

void Director::setTouchRegion(int x, int y, int width, int height)
{
    GestureManager::instance()->setFoucsSurfaceRegion(GestureRegion(x, y, width, height));
}

MatrixStack& Director::matrixStack()
{
    return m_data->matrixStack;
}

LightCache& Director::lightCache()
{
    return m_data->lightCache;
}

TextureCache& Director::textureCache()
{
    return m_data->textureCache;
}

ProgramCache& Director::programCache()
{
    return m_data->programCache;
}

ActionCache& Director::actionCache()
{
    return m_data->actionCache;
}

Camera* Director::getCamera(const std::string& name)
{
    if (m_data->cameras.count(name)) {
        return m_data->cameras[name];
    }
    return NULL;
}

Camera* Director::currentCamera()
{
    return m_data->curcamera;
}

Scene* Director::getScene(const std::string& name)
{
    if (m_data->scenes.count(name)) {
        return m_data->scenes[name];
    }
    return NULL;
}

Scene* Director::currentScene()
{
    return m_data->curscene;
}

void Director::update(float dt)
{
    actionCache().update(dt);
    if (currentScene()) {
        currentScene()->update(dt);
    }
}

bool Director::render()
{
    static int s_drawcount = 0;
    Scene* scene = currentScene();
    Camera* camera = currentCamera();

    if (!scene->isDirty() && !camera->isDirty()) {
        if (s_drawcount > 0) {
            --s_drawcount;
        }
        else {
            return false;
        }
    }
    else {
        s_drawcount = 10;
    }
    //    LOG_BASE(" render ");
    GLHook::glEnable(GL_LINE_SMOOTH);
    GLHook::glEnable(GL_MULTISAMPLE);

#if 1
    GLHook::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLHook::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    GLHook::glClearDepthf(1.0f);

    setAlphaBlending(true);
    setDepthTest(true);

    matrixStack().loadMatrix(MATRIX_STACK_PROJECTION, currentCamera()->projection());
    matrixStack().pushMatrix(MATRIX_STACK_MODELVIEW);

    drawScene();
    matrixStack().popMatrix(MATRIX_STACK_MODELVIEW);

#else
    {
        Texture* txt = textureCache().getTexture(TEXTURE_SCREEN);
        GLHook::glBindFramebuffer(GL_FRAMEBUFFER, m_data->frameBufferScreen);
        GLHook::glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, txt->ogltxtid, 0);

            GLHook::glViewport(0, 0, m_data->winSize.size.width, m_data->winSize.size.height);
            GLHook::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            GLHook::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            GLHook::glClearDepthf(1.0f);

            setAlphaBlending(true);
            setDepthTest(true);

            matrixStack().loadMatrix(MATRIX_STACK_PROJECTION, currentCamera()->projection());
            matrixStack().pushMatrix(MATRIX_STACK_MODELVIEW);

            drawScene();
            matrixStack().popMatrix(MATRIX_STACK_MODELVIEW);

        GLHook::glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    {
        GLHook::glBindFramebuffer(GL_FRAMEBUFFER, m_data->frameBufferLight);
        GLHook::glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureCache().getTexture(TEXTURE_LIGHT)->ogltxtid, 0);
        GLHook::glViewport(0, 0, 256, 160);
        GLHook::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GLHook::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        m_data->fboNodeLight->render();
        // blit2Bmp();
        GLHook::glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    {
        GLHook::glViewport(0, 0, m_data->winSize.size.width, m_data->winSize.size.height);
        GLHook::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GLHook::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        m_data->fboNodeScreen->render();
    }

#endif
    return true;
}

void Director::cleanDeadPool()
{

}

void Director::drawScene()
{
    Scene* scene = currentScene();
    if (scene) {
        scene->render();
    }

    // skip one flame when _deltaTime equal to zero.
//    if (_deltaTime < FLT_EPSILON) {
//        return;
//    }

//    GLHook::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    GLHook::glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
//    GLHook::glClearDepthf(1.0f);
//    setAlphaBlending(true);

//    if (_nextScene) {
//        setNextScene();
//    }

//    loadMatrix(MATRIX_STACK_PROJECTION, _camera->getProjection3D());
//    pushMatrix(MATRIX_STACK_MODELVIEW);

//    setViewport();
//    if (_actionManager) {
//        _actionManager->update(_deltaTime);
//    }
//    // draw the scene
//    if (_runningScene) {
//        _runningScene->step(_deltaTime);
//        _runningScene->visit(_renderer, Matrix::IDENTITY, false);
//    }

//    popMatrix(MATRIX_STACK_MODELVIEW);

//    ++m_data->totalFrames;
    // calculateSPF();
}

//void Director::setGLDefaultValues(void) {
//    // ASSERT(_openGLView, "opengl view should not be null");
//    setAlphaBlending(true);
//    setDepthTest(false);
//    setProjection(_projection);
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//}

static void SetBlending(GLenum sfactor, GLenum dfactor) {
    if (sfactor == GL_ONE && dfactor == GL_ZERO) {
        GLHook::glDisable(GL_BLEND);
    }
    else {
        GLHook::glEnable(GL_BLEND);
        GLHook::glBlendFunc(sfactor, dfactor);
    }
}

void Director::setAlphaBlending(bool on) {
    if (on) {
        SetBlending(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else {
        SetBlending(GL_ONE, GL_ZERO);
    }
}

void Director::setDepthTest(bool on) {
    if (on) {
        GLHook::glClearDepthf(1.0f);
        GLHook::glEnable(GL_DEPTH_TEST);
        GLHook::glDepthFunc(GL_LEQUAL);
    }
    else {
        GLHook::glDisable(GL_DEPTH_TEST);
    }
    CHECK_GL_ERROR_DEBUG();
}

void Director::initFBO()
{
    GLHook::glGenFramebuffers(1, &m_data->frameBufferScreen);
    textureCache().addImage(TEXTURE_SCREEN, textureCache().createTexture(m_data->winSize.size.width, m_data->winSize.size.height),
                            m_data->winSize.size.width, m_data->winSize.size.height);

    GLHook::glGenFramebuffers(1, &m_data->frameBufferLight);
    textureCache().addImage(TEXTURE_LIGHT, textureCache().createTexture(256, 160), 256, 160);

    const unsigned char bmphead[] = {
        //  00    01    02    03    04    05    06    07    08    09    0A    0B    0C    0D    0E    0F
            0x42, 0x4D, 0x36, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, // 00
            0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0xA0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00, // 01
            0x00, 0x00, 0x00, 0x70, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 02
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };                                                           // 03
    //00: 02 ~ 05 : 54(0x36) + width*height*4
    //01: 02 ~ 05 : width
    //02: 06 ~ 09 : height

    size_t bmpheadsize = sizeof(bmphead);
    m_data->fboBmpSize = 256 * 160 * 4 + bmpheadsize;
    m_data->fboBmp = new unsigned char[m_data->fboBmpSize];
    memcpy(m_data->fboBmp, bmphead, bmpheadsize);

    m_data->fboNodeScreen = new RectNode("FBOScreen", 0);
    m_data->fboNodeScreen->initGeometryBuffer();
    m_data->fboNodeScreen->setTexture(TEXTURE_SCREEN);

    m_data->fboNodeLight = new RectNode("FBOLight", 0);
    m_data->fboNodeLight->initGeometryBuffer();
    m_data->fboNodeLight->setTexture(TEXTURE_SCREEN);
}

void Director::blit2Bmp()
{
    GLint pack_alignment_old;
    GLHook::glGetIntegerv(GL_PACK_ALIGNMENT, &pack_alignment_old);
    GLHook::glPixelStorei(GL_PACK_ALIGNMENT, 1);
    GLHook::glReadPixels(0, 0, static_cast<GLsizei>(256), static_cast<GLsizei>(160), GL_RGBA, GL_UNSIGNED_BYTE, m_data->fboBmp+54);
    GLHook::glPixelStorei(GL_PACK_ALIGNMENT, pack_alignment_old);

    for (size_t i = 54; i <  m_data->fboBmpSize; i = i + 4) {
            unsigned char tmp = m_data->fboBmp[i];
            m_data->fboBmp[i] = m_data->fboBmp[i + 2];
            m_data->fboBmp[i + 2] = tmp;
    }

    static int s_count = 0;
    char filePath[256] = {0};
    snprintf(filePath, 256, "/home/colazhu/Engines/BmpClip/AAA%04d.bmp", s_count);
    FILE* handle = fopen(filePath, "wb");
    fwrite(m_data->fboBmp, m_data->fboBmpSize, 1, handle);
    fclose(handle);
    ++s_count;

}

//void Director::setProjection(PROJECTION_TYPE proj) {
//    Size size = _winSize;
//    _camera->init(Vec3(0,0,0), Vec3(0,0,-1), Rect(0.0f,0.0f,size.width,size.height));
//    setViewport();
//    switch (proj) {
//    case _2D: {
//        loadMatrix(MATRIX_STACK_PROJECTION, _camera->getProjection2D());
//        loadIdentityMatrix(MATRIX_STACK_MODELVIEW);
//        break;
//    }
//    case _3D: {
//        loadMatrix(MATRIX_STACK_PROJECTION, _camera->getProjection3D());
//        loadIdentityMatrix(MATRIX_STACK_MODELVIEW);
//        break;
//    }
//    default:
//        LOG_BASE("unrecognized projection");
//        break;
//    }
//    _projection = proj;
//}

