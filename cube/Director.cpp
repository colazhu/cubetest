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
      touchRegion(0, 0, 800, 480),
      curcamera(0),
      curscene(0)
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
    Rect touchRegion;
    MatrixStack matrixStack;

    TextureCache textureCache;
    ProgramCache programCache;
    LightCache lightCache;
    ActionCache actionCache;

    std::map<std::string, Camera*> cameras;
    Camera* curcamera;

    std::map<std::string, Scene*> scenes;
    Scene* curscene;

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

    programCache().loadDefaultPrograms();


    // init camera
    Camera* camera = Camera::createCamera3D(CAMERA_DEFAULT, CAMERA_EYE, CAMERA_DIR, m_data->winSize);
    m_data->curcamera = camera;
    m_data->cameras[CAMERA_DEFAULT] = camera;

    // init scene
    m_data->curscene = new Scene(SCENE_DEFAULT);
    m_data->curscene->init();
    m_data->scenes[SCENE_DEFAULT] = m_data->curscene;

#ifdef USE_CEGUI
    textureCache().addImage(0, "home.png");
    textureCache().addImage(1, "home1600.png");
    textureCache().addImage(2, "home2560.png");

    textureCache().addImage(3, "ac.png");
    textureCache().addImage(4, "ac1600.png");
    textureCache().addImage(5, "ac2560.png");;

    textureCache().addImage(6, "mediaplayer.png");
    textureCache().addImage(7, "mediaplayer1600.png");
    textureCache().addImage(8, "mediaplayer2560.png");

    textureCache().addImage(9,  "navi.png");
    textureCache().addImage(10, "navi1600.png");
    textureCache().addImage(11, "navi2560.png");

    textureCache().addImage(12, "phone.png");
    textureCache().addImage(13, "phone1600.png");
    textureCache().addImage(14, "phone2560.png");

    textureCache().addImage(15, "vehicle.png");
    textureCache().addImage(16, "vehicle1600.png");
    textureCache().addImage(17, "vehicle2560.png");

    textureCache().addImage(100, "background.png");
    textureCache().addImage(101, "drag1.png");

    setPlaneTexture(0, 0);
    setPlaneTexture(1, 3);
    setPlaneTexture(2, 6);
    setPlaneTexture(3, 9);
    setPlaneTexture(4, 12);
    setPlaneTexture(5, 15);
#endif
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

bool Director::isEnable()
{
    if (currentScene()) {
        return currentScene()->isEnable();
    }
    return true;
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

int Director::popEvent()
{
    if (currentScene()) {
        return currentScene()->popEvent();
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
    if (event.id >= 0) {
        GestureManager::instance()->processTouchEvent(motionEvent, 0, event.id, event.x, event.y);
        if (GestureManager::instance()->isPrimary(event.id)
            && !GestureManager::instance()->isMultiTouching()) {
            currentScene()->onTouch((TouchAction)event.action, event.x, event.y);
        }
    }
    else {

    }
}

void Director::injectGesture(const GestureObject& event)
{
    if (currentScene()) {
        currentScene()->onGesture(event);
    }
}

bool Director::isPortait() {
    return ((m_data->layout&LAYOUT_PORTRAIT) != 0);
}

bool Director::isFullScreen() {
    return ((m_data->layout&LAYOUT_FULL) != 0);
}

void Director::updateLayout(bool isPortrait, bool isFull)
{
    bool curFull = isFullScreen();
    bool curPortrait = isPortait();

    if (curPortrait == isPortrait && curFull == isFull) {
        return;
    }
    LOG_BASE("isPortrait:%d->%d isFull:%d->%d", curPortrait, isPortrait, curFull, isFull);
    int layout = 0;
    if (curPortrait != isPortrait) {
        curPortrait = isPortrait;
        if (curPortrait) {
            rotateCube(90, 1000);
        }
        else {
            rotateCube(-90, 1000);

        }
    }
    layout |= curPortrait ? LAYOUT_PORTRAIT : LAYOUT_LANDSCAPE;

    if (curFull != isFull) {
        curFull = isFull;
        if (curFull) {
            // setTouchRegion(0, 0, m_data->winSize.size.width, m_data->winSize.size.height);
        }
        else {
            // setTouchRegion((m_data->winSize.size.width + m_data->winSize.size.height)/2, 0, m_data->winSize.size.height, m_data->winSize.size.height);
        }
    }
    layout |= curFull ? LAYOUT_FULL : LAYOUT_HALF;
    setLayout(layout);
}

void Director::injectKey(const KeyEvent& event)
{
    LOG_BASE("injectKey: keycode:%d action:%d", event.key, event.action);
    if (event.action == KEYACTION_DOWN) {
        switch (event.key) {
            case KEYCODE_LEFT:
                updateLayout(isPortait(), true);
            break;
            case KEYCODE_RIGHT:
                updateLayout(isPortait(), false);
            break;
            case KEYCODE_UP:
                updateLayout(true, isFullScreen());
            break;
            case KEYCODE_DOWN:
                updateLayout(false, isFullScreen());
            break;
        }
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
    GLHook::glViewport(0, 0, width, height);
}

Rect Director::getWindowSize()
{
    return m_data->winSize;
}

void Director::setTouchRegion(int x, int y, int width, int height)
{
    m_data->touchRegion.setRect(x, y, width, height);
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
    GLHook::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLHook::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    GLHook::glClearDepthf(1.0f);

    setAlphaBlending(true);
    setDepthTest(true);
    GLHook::glEnable(GL_LINE_SMOOTH);
    GLHook::glEnable(GL_MULTISAMPLE);
    GLHook::glEnable(GL_SCISSOR_TEST);


    matrixStack().loadMatrix(MATRIX_STACK_PROJECTION, currentCamera()->projection());
    matrixStack().pushMatrix(MATRIX_STACK_MODELVIEW);

    drawScene();

    matrixStack().popMatrix(MATRIX_STACK_MODELVIEW);

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

