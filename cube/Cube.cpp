#include "Cube.h"
#include "LightCache.h"
#include "Camera.h"

class GyroTransition : public IntervalAction
{
public:
    GyroTransition(Node* node) : IntervalAction(node),
        m_normalAngle(0),
        m_upAngle(0)
    {}

    ~GyroTransition() {}

    virtual void onUpdate(Node* target, float dt, float time) {
        LOG_BASE("onUpdate:%p %.1f %.1f", target, dt, time);
        Matrix matRotate1, matRotate2;
        matRotate1.rotate(Quaternion(m_normalAsix, m_normalAngle * time));
        matRotate2.rotate(Quaternion(m_upAsix, m_upAngle * time));
        target->setAdditionalTransform(matRotate2 * matRotate1 * m_matrixAdd);
    }

    Matrix m_matrixAdd;
    Vector3 m_normalAsix;
    float m_normalAngle;
    Vector3 m_upAsix;
    float m_upAngle;
};

Cube::Cube(const std::string& name, int stepsPerPlane, Node* parent):
    Node(name, parent),
    m_intersectVertexIdx(-1),
    m_intersectPlane(PLANE_INVALID),
    m_stepsPerPlane(stepsPerPlane),
    m_verticesPerPlane(VERTICES_PER_CELL * stepsPerPlane* stepsPerPlane),
    m_verticesNum(PLANE_NUM * VERTICES_PER_CELL * stepsPerPlane* stepsPerPlane),
    m_vertices(NULL),
    m_isDummy(false),
    m_touchDownPlane(-1)
{
    m_vertices = new V3F_N3F_T2F_C4F[m_verticesNum];
    Vector4 color(1.0, 1.0, 1.0, 1.0);
    m_material.setAmbient(color * 1.0);
    m_material.setDiffuse(color * 1.0);
    m_material.setSpecular(color * 1.0, 4.0);
    // m_material.setEmssion(color);
}

Cube::~Cube()
{
    SAFE_DELETE_ARRAY(m_vertices)
}

void Cube::onInitGeometryBuffer()
{

    int offset = 0;
    float stepSize = 1.0 / m_stepsPerPlane;

    //-------------- front --------------
    for (int i = offset; i < m_verticesPerPlane + offset; ++i) {
        m_vertices[i].normal.set(0, 0, 1);        
        m_vertices[i].colors = Color4F::WHITE;
    }

    for (int i = 0; i < m_stepsPerPlane; ++i) {
        float offset_i = stepSize*i;
        for (int j = 0; j < m_stepsPerPlane; ++j) {
            float offset_j = stepSize*j;
            m_vertices[0+offset].position.set(offset_i, offset_j, 0); // left-bottom
            m_vertices[0+offset].texCoords = Tex2F(offset_i, 1.0 - offset_j);
            m_vertices[1+offset].position.set(offset_i, offset_j + stepSize, 0); // left-top
            m_vertices[1+offset].texCoords = Tex2F(offset_i, 1.0 - offset_j - stepSize);
            m_vertices[2+offset].position.set(offset_i + stepSize, offset_j + stepSize, 0); // right-top
            m_vertices[2+offset].texCoords = Tex2F(offset_i + stepSize, 1.0 - offset_j - stepSize);
            m_vertices[3+offset].position.set(m_vertices[0+offset].position); // left-bottom
            m_vertices[3+offset].texCoords = m_vertices[0+offset].texCoords;
            m_vertices[4+offset].position.set(m_vertices[2+offset].position); // right-top
            m_vertices[4+offset].texCoords = m_vertices[2+offset].texCoords;
            m_vertices[5+offset].position.set(offset_i + stepSize, offset_j, 0); // right-bottom
            m_vertices[5+offset].texCoords = Tex2F(offset_i + stepSize, 1.0 - offset_j);

            offset += VERTICES_PER_CELL;
        }
    }


    //-------------- back --------------
    for (int i = offset; i < m_verticesPerPlane + offset; ++i) {
        m_vertices[i].normal.set(0, 0, -1);
        m_vertices[i].colors = Color4F::GRAY;
    }

    for (int i = 0; i < m_stepsPerPlane; ++i) {
        float offset_i = stepSize*i;
        for (int j = 0; j < m_stepsPerPlane; ++j) {
            float offset_j = stepSize*j;
            m_vertices[0+offset].position.set(offset_i + stepSize, offset_j, -1); // left-bottom
            m_vertices[0+offset].texCoords = Tex2F(1.0 - (offset_i + stepSize), 1.0 - offset_j);
            m_vertices[1+offset].position.set(offset_i + stepSize, offset_j + stepSize, -1); // left-top
            m_vertices[1+offset].texCoords = Tex2F(1.0 - (offset_i + stepSize), 1.0 - offset_j - stepSize);
            m_vertices[2+offset].position.set(offset_i, offset_j + stepSize, -1); // right-top
            m_vertices[2+offset].texCoords = Tex2F(1.0 - offset_i, 1.0 - offset_j - stepSize);
            m_vertices[3+offset].position.set(m_vertices[0+offset].position); // left-bottom
            m_vertices[3+offset].texCoords = m_vertices[0+offset].texCoords;
            m_vertices[4+offset].position.set(m_vertices[2+offset].position); // right-top
            m_vertices[4+offset].texCoords = m_vertices[2+offset].texCoords;
            m_vertices[5+offset].position.set(offset_i, offset_j, -1); // right-bottom
            m_vertices[5+offset].texCoords = Tex2F(1.0 - offset_i, 1.0 - offset_j);


            offset += VERTICES_PER_CELL;
        }
    }

    // -------------- up --------------
    for (int i = offset; i < m_verticesPerPlane + offset; ++i) {
        m_vertices[i].normal.set(0, 1, 0);
        m_vertices[i].colors = Color4F::RED;
    }

    for (int i = 0; i < m_stepsPerPlane; ++i) {
        float offset_i = stepSize*i;
        for (int j = 0; j < m_stepsPerPlane; ++j) {
            float offset_j = stepSize*j;
            m_vertices[0+offset].position.set(offset_i, 1, -offset_j); // left-bottom
            m_vertices[0+offset].texCoords = Tex2F(offset_i, 1.0 - offset_j);

            m_vertices[1+offset].position.set(offset_i, 1, -(offset_j + stepSize)); // left-top
            m_vertices[1+offset].texCoords = Tex2F(offset_i, 1.0 - offset_j - stepSize);

            m_vertices[2+offset].position.set(offset_i + stepSize, 1, -(offset_j + stepSize)); // right-top
            m_vertices[2+offset].texCoords = Tex2F(offset_i + stepSize, 1.0 - offset_j - stepSize);

            m_vertices[3+offset].position.set(m_vertices[0+offset].position); // left-bottom
            m_vertices[3+offset].texCoords = m_vertices[0+offset].texCoords;
            m_vertices[4+offset].position.set(m_vertices[2+offset].position); // right-top
            m_vertices[4+offset].texCoords = m_vertices[2+offset].texCoords;

            m_vertices[5+offset].position.set(offset_i + stepSize, 1, -offset_j); // right-bottom
            m_vertices[5+offset].texCoords = Tex2F(offset_i + stepSize, 1.0 - offset_j);

            offset += VERTICES_PER_CELL;
        }
    }

    //-------------- down --------------
    for (int i = offset; i < m_verticesPerPlane + offset; ++i) {
        m_vertices[i].normal.set(0, -1, 0);
        m_vertices[i].colors = Color4F::BLUE;
    }

    for (int i = 0; i < m_stepsPerPlane; ++i) {
        float offset_i = stepSize*i;
        for (int j = 0; j < m_stepsPerPlane; ++j) {
            float offset_j = stepSize*j;
            m_vertices[0+offset].position.set(offset_i, 0, -(offset_j + stepSize)); // left-bottom
            m_vertices[0+offset].texCoords = Tex2F(1.0 - offset_i, 1.0 - (offset_j + stepSize));
            m_vertices[1+offset].position.set(offset_i, 0, -offset_j); // left-top
            m_vertices[1+offset].texCoords = Tex2F(1.0 - offset_i, 1.0 - offset_j);
            m_vertices[2+offset].position.set(offset_i + stepSize, 0, -offset_j); // right-top
            m_vertices[2+offset].texCoords = Tex2F(1.0 - (offset_i + stepSize), 1.0 - offset_j);
            m_vertices[3+offset].position.set(m_vertices[0+offset].position); // left-bottom
            m_vertices[3+offset].texCoords = m_vertices[0+offset].texCoords;
            m_vertices[4+offset].position.set(m_vertices[2+offset].position); // right-top
            m_vertices[4+offset].texCoords = m_vertices[2+offset].texCoords;
            m_vertices[5+offset].position.set(offset_i + stepSize, 0, -(offset_j + stepSize)); // right-bottom
            m_vertices[5+offset].texCoords = Tex2F(1.0 - (offset_i + stepSize), 1.0 - (offset_j + stepSize));

//            m_vertices[0+offset].texCoords = Tex2F(offset_i, 1.0 - offset_j);
//            m_vertices[1+offset].texCoords = Tex2F(offset_i, 1.0 - offset_j - stepSize);
//            m_vertices[2+offset].texCoords = Tex2F(offset_i + stepSize, 1.0 - offset_j - stepSize);
//            m_vertices[3+offset].texCoords = m_vertices[0+offset].texCoords;
//            m_vertices[4+offset].texCoords = m_vertices[2+offset].texCoords;
//            m_vertices[5+offset].texCoords = Tex2F(offset_i + stepSize, 1.0 - offset_j);

            offset += VERTICES_PER_CELL;
        }
    }


    //-------------- left --------------
    for (int i = offset; i < m_verticesPerPlane + offset; ++i) {
        m_vertices[i].normal.set(-1, 0, 0);
        m_vertices[i].colors = Color4F::YELLOW;
    }

    for (int i = 0; i < m_stepsPerPlane; ++i) {
        float offset_i = stepSize*i;
        for (int j = 0; j < m_stepsPerPlane; ++j) {
            float offset_j = stepSize*j;
            m_vertices[0+offset].position.set(0, offset_i,  -(offset_j + stepSize)); // left-bottom
            m_vertices[0+offset].texCoords = Tex2F(1.0-(offset_j + stepSize), 1.0 - offset_i);
            m_vertices[1+offset].position.set(0, offset_i + stepSize, -(offset_j + stepSize)); // left-top
            m_vertices[1+offset].texCoords = Tex2F(1.0-(offset_j + stepSize), 1.0 - offset_i - stepSize);
            m_vertices[2+offset].position.set(0, offset_i + stepSize, -offset_j); // right-top
            m_vertices[2+offset].texCoords = Tex2F( 1.0 - offset_j, 1.0 - offset_i - stepSize);
            m_vertices[3+offset].position.set(m_vertices[0+offset].position); // left-bottom
            m_vertices[3+offset].texCoords = m_vertices[0+offset].texCoords;
            m_vertices[4+offset].position.set(m_vertices[2+offset].position); // right-top
            m_vertices[4+offset].texCoords = m_vertices[2+offset].texCoords;
            m_vertices[5+offset].position.set(0, offset_i, -offset_j); // right-bottom
            m_vertices[5+offset].texCoords = Tex2F(1.0 - offset_j, 1.0 - offset_i);


//            m_vertices[0+offset].texCoords = Tex2F(offset_i, 1.0 - offset_j);
//            m_vertices[1+offset].texCoords = Tex2F(offset_i, 1.0 - offset_j - stepSize);
//            m_vertices[2+offset].texCoords = Tex2F(offset_i + stepSize, 1.0 - offset_j - stepSize);

           offset += VERTICES_PER_CELL;
        }
    }

    //--------------  right --------------
    for (int i = offset; i < m_verticesPerPlane + offset; ++i) {
        m_vertices[i].normal.set(1, 0, 0);
        m_vertices[i].colors = Color4F::ORANGE;
    }

    for (int i = 0; i < m_stepsPerPlane; ++i) {
        float offset_i = stepSize*i;
        for (int j = 0; j < m_stepsPerPlane; ++j) {
            float offset_j = stepSize*j;
            m_vertices[0+offset].position.set(1, offset_i,  -offset_j); // left-bottom
            m_vertices[0+offset].texCoords = Tex2F(offset_j, 1.0 - offset_i);
            m_vertices[1+offset].position.set(1, offset_i + stepSize, -offset_j); // left-top
            m_vertices[1+offset].texCoords = Tex2F(offset_j, 1.0 - offset_i - stepSize);
            m_vertices[2+offset].position.set(1, offset_i + stepSize, -(offset_j + stepSize)); // right-top
            m_vertices[2+offset].texCoords = Tex2F(offset_j + stepSize, 1.0 - offset_i - stepSize);
            m_vertices[3+offset].position.set(m_vertices[0+offset].position); // left-bottom
            m_vertices[3+offset].texCoords = m_vertices[0+offset].texCoords;
            m_vertices[4+offset].position.set(m_vertices[2+offset].position); // right-top
            m_vertices[4+offset].texCoords = m_vertices[2+offset].texCoords;
            m_vertices[5+offset].position.set(1, offset_i, -(offset_j + stepSize)); // right-bottom
            m_vertices[5+offset].texCoords = Tex2F(offset_j + stepSize, 1.0 - offset_i);

            offset += VERTICES_PER_CELL;
        }
    }


    // move center to zero
    for (int i = 0; i < m_verticesNum; ++i) {
        m_vertices[i].position -= Vector3(0.5, 0.5, -0.5);
#if 1 // OS_ANDROID
        m_vertices[i].colors = Color4F::WHITE;
#endif
    }

    GLHook::glBindBuffer(GL_ARRAY_BUFFER, m_buffersVBO[0]);
    GLHook::glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices[0]) * m_verticesNum, m_vertices, GL_STATIC_DRAW);
    GLHook::glBindBuffer(GL_ARRAY_BUFFER, 0);

    for (int i = 0; i < PLANE_NUM; ++i) {
        m_planeInfos[i].verticesStart = i * m_verticesPerPlane;
        m_planeInfos[i].verticesCount = m_verticesPerPlane;
    }

    // set plane normals
    m_planeInfos[PLANE_FRONT].normal.set(0, 0, 1);
    m_planeInfos[PLANE_BACK].normal.set(0, 0, -1);
    m_planeInfos[PLANE_UP].normal.set(0, 1, 0);
    m_planeInfos[PLANE_DOWN].normal.set(0, -1, 0);
    m_planeInfos[PLANE_LEFT].normal.set(-1, 0, 0);
    m_planeInfos[PLANE_RIGHT].normal.set(1, 0, 0);

    m_planeInfos[PLANE_FRONT].up.set(0, 1, 0); // up
    m_planeInfos[PLANE_BACK].up.set(0, 1, 0);  // up
    m_planeInfos[PLANE_UP].up.set(0, 0, -1);   // back
    m_planeInfos[PLANE_DOWN].up.set(0, 0, -1);  // back
    m_planeInfos[PLANE_LEFT].up.set(0, 1, 0);  // up
    m_planeInfos[PLANE_RIGHT].up.set(0, 1, 0); // up

    m_planeInfos[PLANE_FRONT].textureId = -1;
    m_planeInfos[PLANE_BACK].textureId = -1;
    m_planeInfos[PLANE_UP].textureId = -1;
    m_planeInfos[PLANE_DOWN].textureId = -1;
    m_planeInfos[PLANE_LEFT].textureId = -1;
    m_planeInfos[PLANE_RIGHT].textureId = -1;
}


void Cube::onUpdate(float dt)
{
    // LOG_BASE_TRACE("onUpdate dt:%lf", dt);
//    if (m_action.timeTotal > 0.0f) {
//        m_action.timeTotal -= 0.05;
//        Matrix matRotate1, matRotate2;
//        matRotate1.rotate(Quaternion(m_action.normalAsix, m_action.normalAngle * (1.0 - m_action.timeTotal)));
//        matRotate2.rotate(Quaternion(m_action.upAsix, m_action.upAngle * (1.0 - m_action.timeTotal)));
//        setAdditionalTransform(matRotate2 * matRotate1 * m_action.matrixAdd);
//    }
//    else {
//        // setAdditionalTransform(Matrix::identity());
//    }
}

void Cube::onTouch(TouchAction touchevent, float x, float y)
{
    Vector3 interSect;

    switch (touchevent) {
    case TouchAction_Down:
        if (Node::testIntersection(Vector2(x, y), interSect)) {
            Vector3 normalSrc, upSrc;
            getIntersectPlaneVector(normalSrc, upSrc);
            LOG_BASE("Test Intersection:%.1lf, %.1lf %d %d", x, y, m_intersectVertexIdx, m_intersectPlane);
            LOG_BASE("Test plane normal: %.1lf, %.1lf %.1lf", normalSrc.x, normalSrc.y, normalSrc.z);
            m_touchDownPlane = m_intersectPlane;
        }
        else {
            LOG_BASE("Test intersect plane None!!!");
            m_touchDownPlane = -1;
        }
        break;
    case TouchAction_Move:
    case TouchAction_Up:
    case TouchAction_Cancel:
    default:
//        m_intersectPlane = PLANE_INVALID;
//        m_intersectVertexIdx = -1;
        break;
    }
}

void Cube::onDraw()
{
    if (m_isDummy) {
        drawDummy();
    }
    else {
        drawCube();
        // drawIntersection();
    }
}

void Cube::setDummyMode(bool enable)
{
    if (m_isDummy == enable)
        return;

    m_isDummy = enable;
    invalidate(FLAGS_GEOMETRY_DIRTY);
}

void Cube::drawDummy()
{
    Program* p = Director::instance()->programCache().getProgram(POSITIONCOLORSHADER);
    if (!p) {
        return;
    }
    p->use();
    p->setUniformsForBuiltins();
    p->setUniformTime(1.0);
    Vector3 eye = Director::instance()->currentCamera()->eye();
    LightCache& lightcache = Director::instance()->lightCache();
    Matrix matrix2World = getNodeToWorldTransform();
    for (int i = 0; i < m_verticesNum; ++i) {
        Vector3 pos, normal;
        matrix2World.transformPoint(m_vertices[i].position, &pos);
        matrix2World.transformVector(m_vertices[i].normal, &normal);
        normal.normalize();
//        if (i == 0 && name() == "Cube") {
//            LOG_BASE("normal: %.1f %.1f %.1f", normal.x, normal.y, normal.z);
//        }
        m_vertices[i].colors = lightcache.doLighting(m_material, Vector4(pos), normal, eye);
        m_vertices[i].colors.a = 1.0;
//        LOG_BASE("color[i]: %.1f %.1f %.1f", i, m_vertices[i].colors.r, m_vertices[i].colors.g, m_vertices[i].colors.b);
    }

    const Color4F planeColors[] = {
//        Color4F::BLACK,
//        Color4F::BLACK,
//        Color4F::BLACK,
//        Color4F::BLACK,
//        Color4F::BLACK,
//        Color4F::BLACK

        Color4F::WHITE,
        Color4F::WHITE,
        Color4F::WHITE,
        Color4F::WHITE,
        Color4F::WHITE,
        Color4F::WHITE

//        Color4F::WHITE,
//        Color4F::GRAY,
//        Color4F::RED,
//        Color4F::BLUE,
//        Color4F::YELLOW,
//        Color4F::ORANGE
    };

    for (int i = 0; i < PLANE_NUM; ++i) {
        for (int j = i*m_verticesPerPlane; j < i*m_verticesPerPlane + m_verticesPerPlane; ++j) {
            // m_vertices[j].colors =  m_vertices[j].colors + planeColors[i];
            m_vertices[j].colors = Color4F::multiply(m_vertices[j].colors, planeColors[i]);
        }
    }

    GLHook::glBindBuffer(GL_ARRAY_BUFFER, m_buffersVBO[0]);
    GLHook::glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices[0]) * m_verticesNum, m_vertices, GL_STATIC_DRAW);
    ENABLE_ATTR_POSITION(V3F_N3F_T2F_C4F, Vector3, position)
    ENABLE_ATTR_COLOR(V3F_N3F_T2F_C4F, Color4F, colors);
    GLHook::glDrawArrays(GL_TRIANGLES, 0, m_verticesNum);

    GLHook::glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL_ERROR_DEBUG();
}

void Cube::drawCube()
{
    Program* p = Director::instance()->programCache().getProgram(TESTSHADER);
    if (!p) {
        return;
    }
    p->use();
    p->setUniformsForBuiltins();

    GLHook::glBindBuffer(GL_ARRAY_BUFFER, m_buffersVBO[0]);

    ENABLE_ATTR_POSITION(V3F_N3F_T2F_C4F, Vector3, position)
    ENABLE_ATTR_NORMAL(V3F_N3F_T2F_C4F, Vector3, normal)
    ENABLE_ATTR_TEX_COORD(V3F_N3F_T2F_C4F, Tex2F, texCoords)
    ENABLE_ATTR_COLOR(V3F_N3F_T2F_C4F, Color4F, colors)

    for (int i = 0; i < PLANE_NUM; ++i) {
        if (Texture* txt = Director::instance()->textureCache().getTexture(m_planeInfos[i].textureId)) {
            txt->bind(true);
        }
        GLHook::glDrawArrays(GL_TRIANGLES, i * m_verticesPerPlane, m_verticesPerPlane);
    }

    GLHook::glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL_ERROR_DEBUG();
}

void Cube::drawIntersection() {
    int idx = m_intersectVertexIdx;
    if (idx < 0) {
        return;
    }

    Program* p = Director::instance()->programCache().getProgram(POSITIONSHADER);
    if (!p) {
        return;
    }
    p->use();
    p->setUniformsForBuiltins();

    GLHook::glBindBuffer(GL_ARRAY_BUFFER, m_buffersVBO[0]);
    ENABLE_ATTR_POSITION(V3F_N3F_T2F_C4F, Vector3, position)
    GLHook::glDrawArrays(GL_LINE_LOOP, m_intersectVertexIdx, 3);
    GLHook::glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool Cube::onTestIntersection(const Vector3& orig, const Vector3& dir, Vector3& intersect) {
    //dir must be normalized
    bool ret = false;
    float fBary1, fBary2;
    float fDist = 100000;

    int intersectVertexIdx = -1;
    for (int i = 0; i < m_verticesNum; i += 3) {
        float fDistCur = 0;
        if (MathUtil::intersectTriangle(orig,
                                        dir,
                                        m_vertices[i].position,
                                        m_vertices[i + 1].position,
                                        m_vertices[i + 2].position,
                                        &fDistCur,
                                        &fBary1,
                                        &fBary2)) {
            if (fDistCur < fDist) {
                fDist = fDistCur;
                intersect = orig + dir * fDist;
                intersectVertexIdx = i;
            }
            ret = true;
        }
    }

    if (ret) {
        m_intersectVertexIdx = intersectVertexIdx;
        for (int i = 0; i < PLANE_NUM; ++i) {
            if (intersectVertexIdx >= m_planeInfos[i].verticesStart &&
                   intersectVertexIdx < m_planeInfos[i].verticesStart + m_planeInfos[i].verticesCount) {
                m_intersectPlane = i;
                break;
            }
        }
    }
    return ret;
}

bool Cube::getIntersectPlaneVector(Vector3& normal, Vector3& up)
{
    if (m_intersectPlane == PLANE_INVALID) {
        return false;
    }

    Vector3 normalWorld = m_planeInfos[m_intersectPlane].normal;
    Vector3 upWorld = m_planeInfos[m_intersectPlane].up;
    Matrix matNode2World = getNodeToWorldTransform();
    matNode2World.transformVector(&normalWorld);
    matNode2World.transformVector(&upWorld);
    normal = normalWorld.normalize();
    up = upWorld.normalize();
    return true;
}

void Cube::doGyro()
{
    if (m_intersectPlane == PLANE_INVALID) {
        return;
    }
    int layout = Director::instance()->getLayout();
    Vector3 normalDst(0, 0, 1);    
    Vector3 upDst(0, 1, 0);
    Matrix matUpDst;    
    if (LAYOUT_FULL_PORTRAIT == layout || LAYOUT_HALF_PORTRAIT == layout) {
        matUpDst.rotateZ(MATH_DEG_TO_RAD(90));
    }
    matUpDst.transformVector(getCamera()->up(), &upDst);
//    m_matUserRotate.transformVector(&upDst);
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

    GyroTransition* action = new GyroTransition(this);
    action->m_matrixAdd = m_transformAdditional;
    action->m_normalAsix = normalAsix;
    action->m_normalAngle = normalAngle;
    action->m_upAsix = upAsix;
    action->m_upAngle = upAngle;
    action->initTimeline(1000.0, true, Expo_EaseInOut);
    runAction(action);
}

void Cube::setPlaneTexture(int planeid, int txtid) {
    if (planeid == PLANE_INVALID) {
        planeid = m_intersectPlane;
    }

    if (planeid < PLANE_FRONT || planeid >= PLANE_NUM ) {
        return;
    }

    m_planeInfos[planeid].textureId = txtid;
//    Texture* txt = Director::instance()->textureCache().getTexture(txtid);
//    if (!txt) {
//        return;
//    }
//    m_planeInfos[planeid].texture = txt;
}

int Cube::getLastTouchDownPlane()
{
    return m_touchDownPlane;
}

int Cube::getIntersectPlane()
{
    return m_intersectPlane;
}

int Cube::getPlaneTexture(int planeid)
{
    if (planeid < PLANE_FRONT || planeid >= PLANE_NUM ) {
        return -1;
    }

    int layout = Director::instance()->getLayout();
    int offset = 0;
    switch (layout) {
    case LAYOUT_HALF_PORTRAIT:
    case LAYOUT_HALF_LANDSCAPE:
        offset = 0;
        break;
    case LAYOUT_FULL_PORTRAIT:
        offset = 1;
        break;
    case LAYOUT_FULL_LANDSCAPE:
        offset = 2;
        break;
    default:
        break;
    }
    return m_planeInfos[planeid].textureId + offset;
}
