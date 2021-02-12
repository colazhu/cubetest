#include "CubePlane.h"
#include "Camera.h"

class GyroZoomInTransition : public IntervalAction
{
public:
    GyroZoomInTransition(Node* node) : IntervalAction(node)
    {

    }

    ~GyroZoomInTransition() {

    }

    virtual void onStart(Node* target)
    {
        if (m_forward) {
            target->show();
        }
    }

    virtual void onUpdate(Node* target, float dt, float time) {
        LOG_BASE("onUpdate:%p %.1f %.1f", target, dt, time);
        Matrix matRotate1, matRotate2;
        matRotate1.rotate(Quaternion(m_normalAsix, m_normalAngle * time));
        matRotate2.rotate(Quaternion(m_upAsix, m_upAngle * time));
        target->setAdditionalTransform(matRotate2 * matRotate1 * m_matrixAdd);
        Vector3 newPos;
        Vector3::lerp(m_position[0], m_position[1], time, &newPos);
        target->setPosition(newPos);

        Vector3 newScale;
        Vector3::lerp(m_scale[0], m_scale[1], time, &newScale);
        target->setScale(newScale);
    }

    virtual void onEnd(Node* target)
    {
        if (!m_forward) {
            target->hide();
        }
    }

    Vector3 m_normalAsix;
    float m_normalAngle;
    Vector3 m_upAsix;
    float m_upAngle;
    Vector3 m_position[2];
    Vector3 m_scale[2];
    Matrix m_matrixAdd;
};

CubePlane::CubePlane(const std::string& name, Node *parent):
    Cube(name, 1, parent),
    m_target(0),
    m_transition(0)
{
    m_transition = new GyroZoomInTransition(this);
    m_transition->setAutoRelease(false);
}

CubePlane::~CubePlane()
{

}

void CubePlane::onDraw()
{
#if OS_ANDROID
    Program* p = Director::instance()->programCache().getProgram(POSITIONTEXTURESHADER);
    if (!p) {
        return;
    }
    p->use();
    p->setUniformsForBuiltins();
    // LOG_BASE("setUniformTime:%.4f", m_transition->getTime());
    p->setUniformTime(m_transition->getTime());
    GLHook::glBindBuffer(GL_ARRAY_BUFFER, m_buffersVBO[0]);

    ENABLE_ATTR_POSITION(V3F_N3F_T2F_C4F, Vector3, position)
    // ENABLE_ATTR_NORMAL(V3F_N3F_T2F_C4F, Vector3, normal)
    ENABLE_ATTR_TEX_COORD(V3F_N3F_T2F_C4F, Tex2F, texCoords)
    // ENABLE_ATTR_COLOR(V3F_N3F_T2F_C4F, Color4F, colors)
#else 
    Program* p = Director::instance()->programCache().getProgram(POSITIONCOLORSHADER);
    if (!p) {
        return;
    }
    p->use();
    p->setUniformsForBuiltins();
    // LOG_BASE("setUniformTime:%.4f", m_transition->getTime());
    p->setUniformTime(m_transition->getTime());
    GLHook::glBindBuffer(GL_ARRAY_BUFFER, m_buffersVBO[0]);

    ENABLE_ATTR_POSITION(V3F_N3F_T2F_C4F, Vector3, position)
    // ENABLE_ATTR_NORMAL(V3F_N3F_T2F_C4F, Vector3, normal)
    // ENABLE_ATTR_TEX_COORD(V3F_N3F_T2F_C4F, Tex2F, texCoords)
    ENABLE_ATTR_COLOR(V3F_N3F_T2F_C4F, Color4F, colors)
#endif

    for (int i = 0; i < PLANE_NUM; ++i) {
        if (i == m_intersectPlane) {
            if (m_planeInfos[i].texture) {
                m_planeInfos[i].texture->bind();
            }
            GLHook::glDrawArrays(GL_TRIANGLES, i * m_verticesPerPlane, m_verticesPerPlane);
        }
    }

    GLHook::glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL_ERROR_DEBUG();
}

void CubePlane::syncTarget(Cube* cube)
{
    if (!cube) {
        return;
    }

    m_rotation = cube->m_rotation;
    m_scale = cube->m_scale;
    m_position  = cube->m_position;

    m_transformAdditional = cube->m_transformAdditional;

    m_intersectPlane = cube->m_intersectPlane;
    m_intersectVertexIdx = cube->m_intersectVertexIdx;

    m_transformDirtys.setFlag(TransformDirty_All);
    invalidate(FLAGS_GEOMETRY_DIRTY);
}

void CubePlane::doBack()
{
    if (m_transition->isDone()) {
        m_transition->initTimeline(1000.0, false, Expo_EaseInOut);
    }
}

void CubePlane::doGyro()
{
    if (m_target) {
        setTimer(0, 1000);
    }
    else {
        doGyroZoomInTransition();
    }
}

void CubePlane::onTimeOut(int id)
{
    syncTarget(m_target);
    doGyroZoomInTransition();
}

void CubePlane::doGyroZoomInTransition()
{
    if (m_intersectPlane == PLANE_INVALID) {
        return;
    }

    Director* director = Director::instance();

    Vector3 normalDst(0, 0, 1);
    Vector3 upDst(1, 0, 0);
    Vector3 normalSrc, upSrc;
    getIntersectPlaneVector(normalSrc, upSrc);

    Matrix matRotate1;
    Vector3 normalAsix;
    Vector3::cross(normalSrc, normalDst, &normalAsix);
    float normalAngle = Vector3::angle(normalSrc, normalDst);
    matRotate1.rotate(Quaternion(normalAsix, normalAngle));

    matRotate1.transformVector(upSrc, &upSrc);
    Vector3 upAsix;
    Vector3::cross(upSrc, upDst, &upAsix);
    float upAngle = Vector3::angle(upSrc, upDst);

    m_transition->m_matrixAdd = m_transformAdditional;
    m_transition->m_normalAsix = normalAsix;
    m_transition->m_normalAngle = normalAngle;
    m_transition->m_upAsix = upAsix;
    m_transition->m_upAngle = upAngle;
    m_transition->m_position[0] = m_position;
    Camera* camera = director->currentCamera(); // camera must fix at z and look to -z
    float distEye = (0.5*m_scale.y / std::tan(MATH_DEG_TO_RAD(camera->fieldOfView()/2.0f)));
    m_transition->m_position[1] = Vector3(0, 0, camera->eye().z - distEye - 0.5*m_scale.z - 0.1);

    Rect winSize = director->getWindowSize();
    m_transition->m_scale[1] = m_transition->m_scale[0] = m_scale;
    if (PLANE_UP == m_intersectPlane ||
            PLANE_DOWN == m_intersectPlane) {
        m_transition->m_scale[1].z = m_transition->m_scale[1].z*winSize.size.width/winSize.size.height;
    }
    else {
        m_transition->m_scale[1].y = m_transition->m_scale[1].y*winSize.size.width/winSize.size.height;
    }
    //
    m_transition->initTimeline(1000.0, true, m_target ? Expo_EaseOut : Expo_EaseInOut);
    runAction(m_transition);
}
