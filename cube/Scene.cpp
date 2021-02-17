#include "Scene.h"
#include "Arcball.h"
#include "Director.h"
#include "LightCache.h"
#include "Camera.h"
#include "Cube.h"
#include "CubePlane.h"
#include "RectNode.h"
#include "gesturePublicDef.h"

#define CUBE_SCALE 6.0f
#define CHILD_CUBE "Cube"
#define CHILD_PLANE "CubePlane"
#define LIGHTCOLOR Color4F(0.8, 0.8, 0.8, 0.8)

//class RotateZTransition : public IntervalAction
//{
//public:
//    RotateZTransition(Node* node, float angle, float deltaAngle) : IntervalAction(node),
//        m_angle(angle),
//        m_deltaAngle(deltaAngle)
//    {}

//    ~RotateZTransition() {}

//    virtual void onUpdate(Node* target, float dt, float time) {
//        // LOG_BASE("onUpdate:%p %.1f %.1f", target, dt, time);
//        target->setRotationZ(m_angle + m_deltaAngle * time);
//    }

//    float m_angle;
//    float m_deltaAngle;
//};

Scene::Scene(const std::string& name):
    Node(name, 0),
    m_arcball(0),
    m_gyroMode(GYROMODE_ONE),
    m_enable(true),
    m_isCubeMode(true),
    m_flicking(false)
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

    LightCache& lightcache = Director::instance()->lightCache();
    lightcache.setGlobalAmbientColor(Color4F(0.5, 0.5, 0.5, 1.0));

    float posscale = 1.5;
    Cube* cb_light0 = new Cube("cb_light0", 1, this);
    cb_light0->initGeometryBuffer();
    cb_light0->setScale(1);
    cb_light0->setPosition(Vector3(-posscale*0.8*CUBE_SCALE, -posscale*0.5*CUBE_SCALE, 0.5*posscale*CUBE_SCALE));
    cb_light0->setDummyMode(true);
//    cb_light0->hide();

    Cube* cb_light1 = new Cube("cb_light1", 1, this);
    cb_light1->initGeometryBuffer();
    cb_light1->setScale(1);
    cb_light1->setPosition(Vector3(posscale*0.8*CUBE_SCALE, -posscale*0.5*CUBE_SCALE, 0.5*posscale*CUBE_SCALE));
    cb_light1->setDummyMode(true);
//    cb_light1->hide();

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

//    Light* pointlight2 = Light::createPointLight(Vector3::ZERO, cb_light2);
//    pointlight2->setColor(LIGHTCOLOR);
//    pointlight2->setSpotExponent(0.5);
//    pointlight2->setIntensity(5);
//    lightcache.addLight(LIGHT_ID_02, pointlight2);

//    lightcache.setFog(Vector4(0.0, 0, 0, 1.0), 25, 1.0);
    lightcache.useFog(true);

    Cube* cube = new Cube(CHILD_CUBE, 10, this);
    cube->initGeometryBuffer();
    cube->setScale(CUBE_SCALE);
    // cube->doRotateZ(90);
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

    rotateScene(225, 2000);
}

void Scene::deinit()
{

}

void Scene::setMode(int mode)
{
    m_gyroMode = mode;
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
    if (!m_enable || !m_isCubeMode || !m_arcball) {
        return;
    }

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

void Scene::onGesture(const GestureEvent& ev)
{
    if (!m_enable) {
        return;
    }

    switch(ev.gtype) {
    case WL_COMMON_GESTURE_TYPE_DOUBLECLICK:
    {
        LOG_BASE("COMMON_GESTURE_TYPE_DOUBLECLICK gyroCube");
        gyroCube(m_gyroMode, m_isCubeMode, 2000);
        m_isCubeMode = !m_isCubeMode;
        // rotateScene(90, 1000);
    }
        break;
    case WL_COMMON_GESTURE_TYPE_FLICK:
    {
        LOG_BASE("COMMON_GESTURE_TYPE_FLICK flickCube");
        flickCube(true);
    }
        break;
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

