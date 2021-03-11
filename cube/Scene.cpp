#include "Scene.h"
#include "Arcball.h"
#include "Director.h"
#include "LightCache.h"
#include "Camera.h"
#include "Cube.h"
#include "CubePlane.h"
#include "RectNode.h"
#include "gesturePublicDef.h"
#include "gestureDef.h"
#include "gestureObject.h"
#include "pinchGesture.h"
#include "threeFlickGesture.h"
#include "twoDragGesture.h"

#define CUBE_SCALE 6.0f
#define CHILD_CUBE "Cube"
#define CHILD_PLANE "CubePlane"
#define AMBIENTCOLOR Color4F(0.5, 0.5, 0.5, 1.0)
#define LIGHTCOLOR Color4F(1.0, 1.0, 1.0, 1.0)

#define AMBIENTCOLOR_TEST Color4F(1.0, 1.0, 1.0, 1.0)
#define LIGHTCOLOR1 Color4F(1.0, 0.0, 0.0, 1.0)
#define LIGHTCOLOR2 Color4F(0.0, 0.0, 1.0, 1.0)
#define CAMERA_Z_MAX (20.0f)
#define CAMERA_Z_MIN (12.0f)

class CameraZTransition : public IntervalAction
{
public:
    CameraZTransition(Node* node, float startZ, float endZ):
        IntervalAction(node),
        m_startZ(startZ),
        m_endZ(endZ)
    {
        m_camera = Director::instance()->currentCamera();
    }

    ~CameraZTransition() {}

    virtual void onUpdate(Node* target, float dt, float time) {
        LOG_BASE("onUpdate:%p %.1f %.1f z:%.1f %.1f %.1f", target, dt, time, m_startZ, m_endZ, m_startZ*(1.0 - time) + m_endZ * time);
        m_camera->setEye(0, 0, m_startZ*(1.0 - time) + m_endZ * time);
    }
    Camera* m_camera;
    float m_startZ;
    float m_endZ;
};

Scene::Scene(const std::string& name):
    Node(name, 0),
    m_arcball(0),
    m_gyroMode(GYROMODE_ONE),
    m_enable(true),
    m_isCubeMode(true),
    m_flicking(false),
    m_cb(NULL),
    m_txtIdSelected(-1),
    m_eventId(-1)
{

}

Scene::~Scene()
{
    if (m_arcball) {
        delete m_arcball;
        m_arcball = NULL;
    }
}

void Scene::init()
{
    updateArcball();

    Director* director = Director::instance();
    RectNode* background = new RectNode("background", this);
    Rect winrect = director->getWindowSize();
    background->initGeometryBuffer();
    background->setRatio(winrect.size.width/winrect.size.height);
    background->setTexture(100); // background textureid
    
    LightCache& lightcache = director->lightCache();
    lightcache.setGlobalAmbientColor(AMBIENTCOLOR);

    float posscale = 1.5;
    Cube* cb_light0 = new Cube("cb_light0", 1, this);
    cb_light0->initGeometryBuffer();
    cb_light0->setScale(1);
    cb_light0->setPosition(Vector3(-posscale*0.8*CUBE_SCALE, -posscale*0.5*CUBE_SCALE, 0.5*posscale*CUBE_SCALE));
    cb_light0->setDummyMode(true);
    cb_light0->hide();

    Cube* cb_light1 = new Cube("cb_light1", 1, this);
    cb_light1->initGeometryBuffer();
    cb_light1->setScale(1);
    cb_light1->setPosition(Vector3(posscale*0.8*CUBE_SCALE, -posscale*0.5*CUBE_SCALE, 0.5*posscale*CUBE_SCALE));
    cb_light1->setDummyMode(true);
    cb_light1->hide();

//    Cube* cb_light2 = new Cube("cb_light2", this);
//    cb_light2->initGeometryBuffer();
//    cb_light2->setScale(1);
//    cb_light2->setPosition(Vector3(0, posscale*0.5*CUBE_SCALE, -posscale*CUBE_SCALE));
//    cb_light2->setDummyMode(true);
//    // cb_light2->hide();

    Light* pointlight0 = Light::createPointLight(Vector3::ZERO, cb_light0);
    pointlight0->setColor(LIGHTCOLOR);
    pointlight0->setSpotExponent(0.5);
    pointlight0->setIntensity(5);
    lightcache.addLight(LIGHT_ID_00, pointlight0);

    Light* pointlight1 = Light::createPointLight(Vector3::ZERO, cb_light1);
    pointlight1->setColor(LIGHTCOLOR);
    pointlight1->setSpotExponent(0.5);
    pointlight1->setIntensity(5);
    lightcache.addLight(LIGHT_ID_01, pointlight1);

//    lightcache.setGlobalAmbientColor(AMBIENTCOLOR_TEST);
//    pointlight0->setColor(LIGHTCOLOR1);
//    pointlight1->setColor(LIGHTCOLOR2);
//    cb_light0->show();
//    cb_light1->show();

//    Light* pointlight2 = Light::createPointLight(Vector3::ZERO, cb_light2);
//    pointlight2->setColor(LIGHTCOLOR);
//    pointlight2->setSpotExponent(0.5);
//    pointlight2->setIntensity(5);
//    lightcache.addLight(LIGHT_ID_02, pointlight2);

    lightcache.setFog(Vector4(0.0, 0, 0, 1.0), 25, 1.0);
    lightcache.useFog(true);

    Cube* cube = new Cube(CHILD_CUBE, 32, this);
    cube->initGeometryBuffer();
    cube->setScale(CUBE_SCALE);
    // cube->hide();

    CubePlane* cubeplane = new CubePlane(CHILD_PLANE, this);
    cubeplane->initGeometryBuffer();
    cubeplane->setScale(CUBE_SCALE);

//        Cube* cube2 = new Cube(CHILD_CUBE, this);
//        cube2->initGeometryBuffer();
//        cube2->setScale(CUBE_SCALE);
//        cube2->setPosition(10, 0, -15);
//        cube2->setRotation(30, 20, 50);

//    RectNode* rect = new RectNode("TestRect", this);
//    rect->initGeometryBuffer();
//    rect->setScale(1.0);

//    rotateScene(225, 2000);
}

void Scene::deinit()
{

}

bool Scene::isEnable()
{
    return m_enable;
}

void Scene::setGyroMode(int mode)
{
    m_gyroMode = mode;
}

void Scene::setLayout(int layout)
{
    if (!m_isCubeMode) {
        gyroCube(m_gyroMode, m_isCubeMode, 2000);
        m_isCubeMode = !m_isCubeMode;
    }
}

void Scene::updateArcball()
{
    Rect winRect = Director::instance()->getWindowSize();
    if (!m_arcball) {
        m_arcball = new Arcball(winRect.size.width, winRect.size.height);
        // m_arcball->setMode(MODE_HORIZONTAL_AND_VERTICAL);
    }
    else {
        m_arcball->setWindow(winRect.size.width, winRect.size.height);
    }
    m_arcball->reset();
    invalidate(FLAGS_GEOMETRY_DIRTY);
}

void Scene::onTouch(TouchAction touchevent, float x, float y)
{
    if (!m_enable || !m_arcball) {
        return;
    }

    if (m_isCubeMode) {
        Cube* cube = static_cast<Cube*>(getChild(CHILD_CUBE));

        switch (touchevent) {
        case TouchAction_Down:
            if (m_flicking) {
                flickCube(false);
                m_arcball->reset(m_flickPreQuat);
            }
            m_arcball->onBegin(x, y);
            break;
        case TouchAction_Move:
            m_arcball->onMove(x, y);
            break;
        case TouchAction_Up:
            m_arcball->onEnd();
            break;
        case TouchAction_Cancel:
            break;
        default:
            break;
        }

        Matrix matrix = m_arcball->getRotationMatrix()* m_rotateMat;
        matrix.invert();
        cube->setAdditionalTransform(matrix);
        cube->onTouch(touchevent, x, y);
    }
}

void Scene::onGesture(const GestureObject& ev_const)
{
    if (!m_enable) {
        return;
    }

    GestureObject& ev = const_cast<GestureObject&>(ev_const);

#define PRINT_GESTRUE(GETSTURE_NAME) \
    case GETSTURE_NAME: { LOG_BASE("%s state[%d]", #GETSTURE_NAME, ev.getState()); break; }
    
    switch(ev.getType()) {
        case GESTURE_TYPE_3FLICK:
        {
            LOG_BASE("GESTURE_TYPE_3FLICK state[%d]", ev.getState());
            ThreeFlickGesture& threeflick = static_cast<ThreeFlickGesture&>(ev);
            if (ev.getState() == GESTURE_STATE_UPDATED) {
                 GesturePoint& point = threeflick.getOffset();
                 LOG_BASE("threeflick offset:x:%d y:%d", point.x, point.y);
                 if (MATH_ABS(point.x) > MATH_ABS(point.y)*2) {
                     m_eventId = point.x > 0 ? CUBE_EVENT_3FLICK_RIGHT : CUBE_EVENT_3FLICK_LEFT;
                 }
            }
        }
            break;
        case GESTURE_TYPE_DOUBLECLICK:
        {
            LOG_BASE("GESTURE_TYPE_DOUBLECLICK");
            gyroCube(m_gyroMode, m_isCubeMode, 2000);
            m_isCubeMode = !m_isCubeMode;
            // rotateScene(90, 1000);
        }
            break;
        case GESTURE_TYPE_FLICK:
        {
            LOG_BASE("GESTURE_TYPE_FLICK state[%d]", ev.getState());
            flickCube(true);
        }
            break;
        case GESTURE_TYPE_LONGPRESS:
        {
            LOG_BASE("GESTURE_TYPE_LONGPRESS state[%d]", ev.getState());
            int txtid = -1;
            if (GESTURE_STATE_STARTED == ev.getState()) {
                txtid = longclickScene();
            }

            if (-1 != txtid) {
                m_txtIdSelected = txtid;
                if (m_cb) {
                    m_cb->onTexturePicked(txtid);
                }
            }
        }
            break;
        case GESTURE_TYPE_PINCH:
        {
            LOG_BASE("GESTURE_TYPE_PINCH state[%d]", ev.getState());
            PinchGesture& pinch = static_cast<PinchGesture&>(ev);
            LOG_BASE("pinch scale:%.2f, last:%.2f, total:%.2f", pinch.getScaleFactor(), pinch.getLastScaleFactor(), pinch.getTotalScaleFactor());
            if (GESTURE_STATE_FINISHED == ev.getState()) {
                if (m_isCubeMode) {
                    Camera* camera = Director::instance()->currentCamera();
                    if (camera->eye().z < CAMERA_Z_MIN + 0.5f && pinch.getTotalScaleFactor() > 1.0f) {
                        gyroCube(GYROMODE_TWO, m_isCubeMode, 2000);
                        m_isCubeMode = !m_isCubeMode;
                    }
                    else {                     
                        float deltaZ = (pinch.getTotalScaleFactor() - 1.0f) * 10;
                        float cameraZ = MATH_MIN(camera->eye().z - deltaZ, CAMERA_Z_MAX);
                        cameraZ = MATH_MAX(cameraZ, CAMERA_Z_MIN);                    
                        zoomCamera(camera->eye().z, cameraZ, 200);
                    }                    
                }
                else {
                    if (pinch.getTotalScaleFactor() < 1.0f) {
                        gyroCube(GYROMODE_TWO, m_isCubeMode, 2000);
                        m_isCubeMode = !m_isCubeMode;
                    }
                }
            }    
        }
        break;
        case GESTURE_TYPE_2DRAG:
        {
            LOG_BASE("GESTURE_TYPE_2DRAG state[%d]", ev.getState());
            TwoDragGesture& twoDrag = static_cast<TwoDragGesture&>(ev);
            if (GESTURE_STATE_FINISHED == ev.getState()) {
                GesturePoint& point = twoDrag.getOffset();
                LOG_BASE("twoDrag offset x:%.2f, y:%.2f", point.x, point.y);

                 if (MATH_ABS(point.x) > MATH_ABS(point.y)*2) {
                     m_eventId = point.x > 0 ? CUBE_EVENT_3FLICK_RIGHT : CUBE_EVENT_3FLICK_LEFT;
                 }
            }
        }
        break;
        PRINT_GESTRUE(GESTURE_TYPE_SCRATCH)
//        PRINT_GESTRUE(GESTURE_TYPE_3FLICK)
        PRINT_GESTRUE(GESTURE_TYPE_MULTILONGPRESS)
        PRINT_GESTRUE(GESTURE_TYPE_TAP)
        PRINT_GESTRUE(GESTURE_TYPE_DRAG)
//        PRINT_GESTRUE(GESTURE_TYPE_FLICK)
//        PRINT_GESTRUE(GESTURE_TYPE_PINCH)
//        PRINT_GESTRUE(GESTURE_TYPE_DOUBLECLICK)
        PRINT_GESTRUE(GESTURE_TYPE_2FLICK)
        PRINT_GESTRUE(GESTURE_TYPE_2ROTARY)
//        PRINT_GESTRUE(GESTURE_TYPE_2DRAG)
//        PRINT_GESTRUE(GESTURE_TYPE_LONGPRESS)
        PRINT_GESTRUE(GESTURE_TYPE_2LONGPRESS)
        PRINT_GESTRUE(GESTURE_TYPE_2TAP)
    default:
        break;
    }    
}

void Scene::onUpdate(float dt)
{
    if (m_flicking) {
        m_flickPreQuat = m_flickPreQuat * Quaternion(m_flickAxis, m_flickSpeed*dt);
        Matrix rotatematrix;
        Matrix::createRotation(m_flickPreQuat*m_rotateMat, &rotatematrix);
        rotatematrix.invert();
        Cube* cube = static_cast<Cube*>(getChild(CHILD_CUBE));
        cube->setAdditionalTransform(rotatematrix);
        // LOG_BASE("update");
        m_flickSpeed = m_flickSpeed - dt * m_flickSpeed / 200.0 ;
        if (m_flickSpeed < MATH_DEG_TO_RAD(1) / 1000.0) {
            m_flickSpeed = 0.0;
            m_flicking = false;
        }
        invalidate(FLAGS_GEOMETRY_DIRTY);
    }

}

int Scene::longclickScene()
{
    int txtid = -1;
    if (m_isCubeMode) {
        Cube* cube = static_cast<Cube*>(getChild(CHILD_CUBE));
        int planeid = cube->getLastTouchDownPlane();
        txtid = cube->getPlaneTexture(planeid);
        LOG_BASE("CubeMode Plane[%d][txt:%d] on LongPressed", planeid, txtid);
        if (-1 != planeid) {
            gyroCube(GYROMODE_TWO, m_isCubeMode, 2000);
            m_isCubeMode = !m_isCubeMode;
        }

    }
    else {
        CubePlane* plane = static_cast<CubePlane*>(getChild(CHILD_PLANE));
        int planeid = plane->getIntersectPlane();
        txtid = plane->getPlaneTexture(planeid);
        LOG_BASE("PlaneMode Plane[%d][txt:%d] on LongPressed", planeid, txtid);
    }
    return txtid;
}

void Scene::zoomCamera(float from, float to, float ms)
{
    CameraZTransition* action = new CameraZTransition(this, from, to);
    action->initTimeline(ms, true);
    runAction(action);
    disableInTime(ms);
}


void Scene::flickCube(bool run)
{
    if (!m_arcball) {
        return;
    }

    if (run) {
        Quaternion dq = m_arcball->getRotationQuatIncreament();
        Vector3 axis;
        float radians = dq.toAxisAngle(&axis);
//        Matrix mat;
//        m_rotateMat.invert(&mat);
//        mat.transformVector(&axis);
        // m_rotateMat.transformVector(&axis);
        int dt = m_arcball->getTimeIncreament();
        if (dt > 1 && radians > 0.01) {
            float speed = radians / (float)dt;
            LOG_BASE("degrees:%.3f %.3f in %d ms speed:%.3f > %.3f?", MATH_RAD_TO_DEG(radians), radians, dt, speed, MATH_DEG_TO_RAD(30) / 1000.0);
            if (speed > MATH_DEG_TO_RAD(30) / 1000.0) {
                m_flickSpeed = speed;
                m_flickAxis = axis;
                m_flicking = true;
                m_flickPreQuat = m_arcball->getRotationQuaternion();
            }
        }
    }
    else {
        m_flicking = false;
        m_flickSpeed = 0;
        m_flickAxis = Vector3::ZERO;
    }
}

void Scene::gyroCube(int mode, bool forward, float ms)
{
    Cube* cube = static_cast<Cube*>(getChild(CHILD_CUBE));
    CubePlane* plane = static_cast<CubePlane*>(getChild(CHILD_PLANE));
    if (forward) {
        switch (mode) {
        case GYROMODE_ONE:
            cube->doGyro();
            plane->setTarget(cube);
            plane->doGyro();
            break;
        case GYROMODE_TWO:
            plane->syncTarget(cube);
            plane->doGyro();
            break;
        default:
            break;
        }
    }
    else {
        plane->syncTarget(cube);
        plane->doBack();
    }

     disableInTime(ms);
}

void Scene::rotateScene(float degrees, float ms) {

    m_rotateMat.rotateZ(MATH_DEG_TO_RAD(degrees));

    getCamera()->roll(degrees, ms);

    disableInTime(ms);
}

void Scene::setCubePlaneTexture(int planeid, int txtid) {
    Cube* cube = static_cast<Cube*>(getChild(CHILD_CUBE));
    if (cube) {
        cube->setPlaneTexture(planeid, txtid);
    }

    CubePlane* plane = static_cast<CubePlane*>(getChild(CHILD_PLANE));
    if (plane) {
        plane->setPlaneTexture(planeid, txtid);
    }
}

void Scene::addCubeCallback(CubeCallback* cb)
{
    m_cb = cb;
}

int Scene::popSelectedTexture()
{
    int ret = m_txtIdSelected;
    m_txtIdSelected = -1;
    if (-1 != ret) {
        LOG_BASE("popSelectedTexture:%d", ret);
    }
    return ret;
}

int Scene::popEvent()
{
    int ret = m_eventId;
    m_eventId = -1;
    return ret;
}

void Scene::disableInTime(float ms)
{
    m_enable = false;
    setTimer(0, ms);
}

void Scene::onTimeOut(int id)
{
    UNUSED(id)
    m_enable = true;
}

