#include "Node.h"
#include "Director.h"
#include "MatrixStack.h"
#include "LightCache.h"
#include "ActionCache.h"
#include "Camera.h"

Node::Node(const std::string& name, Node *parent):
m_parent(0),
m_name(name),
m_rotation(),
m_scale(1.0, 1.0, 1.0),
m_position(),
m_visible(true),
m_transformDirtys(TransformDirty_All),
m_transform(),
m_transformInverse(),
m_transformAdditional(),
m_transformMV(),
m_camera(0),
m_materialUpdated(false),
m_material(),
m_dirtys(FLAGS_DIRTY_MASK)
// m_transformUpdated(false),
// m_globalAmbientColor(),
// m_transformData(),
//m_lightingData()
// m_actions()
{

    setParent(parent);
    // m_material.setColor(Color4F(1.0, 1.0, 1.0, 1.0));
    // connect(&m_camera, SIGNAL(cameraChanged()), this, SLOT(cameraUpdated()));
}

Node::~Node()
{
    int count = sizeof(m_buffersVBO) / sizeof (m_buffersVBO[0]);
    GLHook::glDeleteBuffers(count, &m_buffersVBO[0]);
//    qDeleteAll(m_actions);
//    qDeleteAll(m_lights);
}

Node* Node::parent() const
{
    return m_parent;
}

const NodeList& Node::children() const
{
    return m_children;
}

void Node::setParent(Node *_parent)
{
    bind(this, _parent);
}

void Node::bringToTop(Node *child)
{
    if (NULL == child || child->parent() != this) {
        return;
    }

    Node* result = NULL;
    for (NodeList::iterator iter = m_children.begin(); iter != m_children.end(); ++iter) {
        if (child == *iter) {
            result = child;
            m_children.erase(iter);
            break;
        }
    }

    if (NULL != result) {
        m_children.push_back(result);
    }
}

void Node::pushToBottom(Node *child)
{
    if (NULL == child || child->parent() != this) {
        return;
    }


    Node* result = NULL;
    for (NodeList::iterator iter = m_children.begin(); iter != m_children.end(); ++iter) {
        if (child == *iter) {
            result = child;
            m_children.erase(iter);
            break;
        }
    }

    if (NULL != result) {
        m_children.push_front(result);
    }
}

Node* Node::top() const
{
    const Node* top = this;
    while (top->parent() != NULL) {
        top = top->parent();
    }
    return const_cast<Node*>(top);
}

Node* Node::getChild(const std::string& name, bool recursive)
{
    // todo
    for (NodeList::iterator iter = m_children.begin(); iter != m_children.end(); ++iter) {
        if (Node* child = *iter) {
            if (name == child->name()) {
                return child;
            }
        }
    }
    return NULL;
}


void Node::bind(Node* _child, Node* _parent)
{
    if (NULL == _child || _child == _parent || _child->m_parent == _parent) {
        return;
    }

    Node* oldParent = _child->parent();

    unbind(_child);

    _child->m_parent = _parent;

    if (NULL != _parent) {
        _parent->m_children.push_back(_child);
        _parent->childAdded(_child);
    }

    _child->notifyParentChanged(oldParent, _parent);
}

void Node::unbind(Node* _child)
{
    if (NULL == _child || NULL == _child->m_parent) {
        return;
    }

    NodeList& _children = _child->m_parent->m_children;
    NodeList::iterator iter = std::find(_children.begin(), _children.end(), _child);
    if (iter != _children.end()) {
        _children.erase(iter);
    }

    Node* _parent = _child->parent();
    _child->m_parent = NULL;
    _parent->childRemoved(_child);
}

void Node::pause()
{

}

void Node::resume()
{

}

/// scale setter
void Node::setScale(float scale)
{
    if (m_scale == Vector3(scale, scale, scale)) {
        return;
    }

    m_scale = Vector3(scale, scale, scale);
    m_transformDirtys.setFlag(TransformDirty_All);
    invalidate(FLAGS_GEOMETRY_DIRTY);
}

void Node::setScale(const Vector3& scale)
{
    if (m_scale == scale) {
        return;
    }

    m_scale = scale;
    m_transformDirtys.setFlag(TransformDirty_All);
    invalidate(FLAGS_GEOMETRY_DIRTY);
}

/// scale setter
void Node::setScale(float scaleX, float scaleY, float scaleZ)
{
    if (m_scale == Vector3(scaleX, scaleY, scaleZ)) {
        return;
    }

    m_scale = Vector3(scaleX, scaleY, scaleZ);
    m_transformDirtys.setFlag(TransformDirty_All);
    invalidate(FLAGS_GEOMETRY_DIRTY);
}

/// scaleX getter
float Node::scaleX() const
{
    return m_scale.x;
}

/// scaleX setter
void Node::setScaleX(float scaleX)
{
//    if (m_scale.x == scaleX) {
//        return;
//    }

    m_scale.x = scaleX;
    m_transformDirtys.setFlag(TransformDirty_All);
    invalidate(FLAGS_GEOMETRY_DIRTY);
}

/// scaleY getter
float Node::scaleY() const
{
    return m_scale.y;
}

/// scaleY setter
void Node::setScaleY(float scaleY)
{
//    if (m_scale.y == scaleY) {
//        return;
//    }

    m_scale.y = scaleY;
    m_transformDirtys.setFlag(TransformDirty_All);
    invalidate(FLAGS_GEOMETRY_DIRTY);
}

/// scaleZ getter
float Node::scaleZ() const
{
    return m_scale.z;
}

/// scaleZ setter
void Node::setScaleZ(float scaleZ)
{
//    if (m_scale.z == scaleZ) {
//        return;
//    }

    m_scale.z = scaleZ;
    m_transformDirtys.setFlag(TransformDirty_All);
    invalidate(FLAGS_GEOMETRY_DIRTY);
}

Vector3 Node::scale() const
{
    return m_scale;
}

void Node::setRotation(float x, float y, float z)
{
    Vector3 rotation(x, y, z);
    if (m_rotation == rotation) {
        return;
    }

    m_rotation = rotation;
    m_transformDirtys.setFlag(TransformDirty_All);
    invalidate(FLAGS_GEOMETRY_DIRTY);
}

void Node::setRotationX(float degree)
{
//     if (m_rotation.x == degree) {
//        return;
//    }

    m_rotation.x = degree;
    m_transformDirtys.setFlag(TransformDirty_All);
    invalidate(FLAGS_GEOMETRY_DIRTY);
}

void Node::setRotationY(float degree)
{
//     if (m_rotation.y == degree) {
//        return;
//    }

    m_rotation.y = degree;
    m_transformDirtys.setFlag(TransformDirty_All);
    invalidate(FLAGS_GEOMETRY_DIRTY);
}

void Node::setRotationZ(float degree)
{
//    if (m_rotation.z == degree) {
//        return;
//    }

    m_rotation.z = degree;
    m_transformDirtys.setFlag(TransformDirty_All);
    invalidate(FLAGS_GEOMETRY_DIRTY);
}

void Node::setRotation(const Vector3& rotation)
{
    if (m_rotation == rotation) {
        return;
    }

    m_rotation = rotation;
    m_transformDirtys.setFlag(TransformDirty_All);
    invalidate(FLAGS_GEOMETRY_DIRTY);
}

Vector3 Node::rotation() const
{
    return m_rotation;
}

void Node::setPosition(float x, float y, float z)
{
    setPosition(Vector3(x, y, z));
}

void Node::setPosition(const Vector3& position)
{
    if (m_position == position) {
        return;
    }

    m_position = position;
    m_transformDirtys.setFlag(TransformDirty_All);
    invalidate(FLAGS_GEOMETRY_DIRTY);
}

float Node::positionX() const
{
    return m_position.x;
}

void Node::setPositionX(float x)
{
    if (m_position.x == x) {
        return;
    }

    m_position.x = x;
    m_transformDirtys.setFlag(TransformDirty_All);
    invalidate(FLAGS_GEOMETRY_DIRTY);
}

float Node::positionY() const
{
    return m_position.y;
}

void Node::setPositionY(float y)
{
    if (m_position.y == y) {
        return;
    }

    m_position.y = y;
    m_transformDirtys.setFlag(TransformDirty_All);
    invalidate(FLAGS_GEOMETRY_DIRTY);
}

float Node::positionZ() const
{
    return m_position.z;
}

void Node::setPositionZ(float z)
{
    if (m_position.z == z) {
        return;
    }

    m_position.z = z;
    m_transformDirtys.setFlag(TransformDirty_All);
    invalidate(FLAGS_GEOMETRY_DIRTY);
}

void Node::setAdditionalTransform(const Matrix& additionalTransform)
{
//    if (m_transformAdditional == additionalTransform) {
//        return;
//    }
    m_transformAdditional = additionalTransform;
    m_transformDirtys.setFlag(TransformDirty_All);
    invalidate(FLAGS_GEOMETRY_DIRTY);
}

Matrix Node::getNodeToParentTransform()
{
    if (m_transformDirtys.testFlag(TransformDirty_Self)) {
        m_transform.setIdentity();
        Matrix matTransform;
        matTransform.translate(m_position);

        Matrix matRotation;
        matRotation.rotate(Quaternion(Vector3(1, 0, 0), MATH_DEG_TO_RAD(m_rotation.x))
                * Quaternion(Vector3(0, 1, 0), MATH_DEG_TO_RAD(m_rotation.y))
                * Quaternion(Vector3(0, 0, 1), MATH_DEG_TO_RAD(m_rotation.z)));

        Matrix matScale;
        matScale.scale(m_scale);
        // m_transform = m_transformAdditional * matTransform * matRotation * matScale;
        m_transform = matTransform * m_transformAdditional * matRotation * matScale;
        m_transformDirtys.setFlag(TransformDirty_Self, false);
    }

    return m_transform;
}

Matrix Node::getParentToNodeTransform()
{
    Matrix transform = getNodeToParentTransform();
    if (m_transformDirtys.testFlag(TransformDirty_SelfInverse)) {
        transform.invert(&m_transformInverse);
        m_transformDirtys.setFlag(TransformDirty_SelfInverse, false);
    }

    return m_transformInverse;
}

Matrix Node::getNodeToWorldTransform()
{
    Matrix t = getNodeToParentTransform();
    for (Node *p = m_parent; p != NULL; p = p->parent()) {
        t = p->getNodeToParentTransform() * t;
    }
    return t;
}

Matrix Node::getWorldToNodeTransform()
{
    Matrix matInvert;
    getNodeToWorldTransform().invert(&matInvert);
    return matInvert;
}

Vector3 Node::convertToNodeSpace(const Vector3& worldPoint)
{
    Vector3 p;
    getWorldToNodeTransform().transformPoint(worldPoint, &p);
    return p;
}

Vector3 Node::convertToWorldSpace(const Vector3& nodePoint)
{
    Vector3 p;
    getNodeToWorldTransform().transformPoint(nodePoint, &p);
    return p;
}

bool Node::screenCoorToRayInWorldTransform(const Vector2& position, Vector3* orig, Vector3* dir)
{
    if (!orig || !dir) {
        return false;
    }
    Camera* camera = getCamera();
    if (!camera) {
        return false;
    }

    Quaternion q;
    *orig = camera->eye();
    Rect viewport = camera->viewPort();
    Vector3 cameraDir_in_camera(0, 0, -1);
    Vector3 rayDir_in_camera;
    rayDir_in_camera.z = (-(0.5f * viewport.size.height) / tanf(MATH_DEG_TO_RAD(camera->fieldOfView() / 2.0f)));
    rayDir_in_camera.x = (position.x - 0.5f * viewport.size.width);
    rayDir_in_camera.y = (0.5f * viewport.size.height - position.y);

    Vector3 cameraDir_in_world = camera->direction();

    float angleDir = Vector3::angle(cameraDir_in_camera, cameraDir_in_world);
    Vector3 axisDir;
    Vector3::cross(cameraDir_in_camera, cameraDir_in_world, &axisDir);
    q *=Quaternion(axisDir, angleDir);


    Vector3 cameraUp_in_camera(0, 1, 0);
    Vector3 cameraUp_in_world = camera->up();

    float angleUp = Vector3::angle(cameraUp_in_camera, cameraUp_in_world);
    Vector3 axisUp;
    Vector3::cross(cameraUp_in_camera, cameraUp_in_world, &axisUp);
    q *=Quaternion(axisUp, angleUp);

    q.rotatePoint(rayDir_in_camera, dir);
    return true;
}

bool Node::screenCoorToRayInNodeTransform(const Vector2& position,
                                              Vector3* orig,
                                              Vector3* dir)
{
    if (!screenCoorToRayInWorldTransform(position, orig, dir)) {
        return false;
    }

    Matrix matWorldToNode = getWorldToNodeTransform();
    matWorldToNode.transformPoint(orig);
    matWorldToNode.transformVector(dir);
    return true;
}

bool Node::intersectTriangle(const Vector3& orig,
                             const Vector3& dir,
                             Vector3& v0,
                             Vector3& v1,
                             Vector3& v2,
                             float* t,
                             float* u,
                             float* v)
{

    // Find vectors for two edges sharing vert0
    Vector3 edge1 = v1 - v0;
    Vector3 edge2 = v2 - v0;

    // Begin calculating determinant - also used to calculate U parameter
    Vector3 pvec;
    Vector3::cross(dir, edge2, &pvec);

    // If determinant is near zero, ray lies in plane of triangle
    float det = Vector3::dot(edge1, pvec);

    Vector3 tvec;
    if (det > 0) {
        tvec = orig - v0;
    }
    else {
        tvec = v0 - orig;
        det = -det;
    }

    if (det < 0.0001f) {
        return false;
    }

    // Calculate U parameter and test bounds
    *u = Vector3::dot(tvec, pvec);
    if (*u < 0.0f || *u > det) {
        return false;
    }

    // Prepare to test V parameter
    Vector3 qvec;
    Vector3::cross(tvec, edge1, &qvec);

    // Calculate V parameter and test bounds
    *v = Vector3::dot(dir, qvec);
    if (*v < 0.0f || *u + *v > det) {
        return false;
    }

    // Calculate t, scale parameters, ray intersects triangle
    *t = Vector3::dot(edge2, qvec);
    float fInvDet = 1.0f / det;
    *t *= fInvDet;
    *u *= fInvDet;
    *v *= fInvDet;

    return true;
}

Camera* Node::getCamera()
{
    Node* node = this;
    while (node) {
        if (node->m_camera) {
            return node->m_camera;
        }
        node = node->m_parent;
    }

    return Director::instance()->currentCamera();
}

void Node::materialUpdated()
{
    m_materialUpdated = true;
    invalidate(FLAGS_MATERIAL_DIRTY);
}


void Node::setVisible(bool visible)
{
    if (m_visible == visible) {
        return;
    }

    m_visible = visible;
    invalidate(FLAGS_GEOMETRY_DIRTY);
}

void Node::update(float dt)
{
    // updateActions(dt);
    onUpdate(dt);
    for (NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it) {
        if (Node* child = *it) {
            child->update(dt);
        }
    }
}


Matrix Node::transform(const Matrix& parentTransform)
{
    Matrix ret = getNodeToParentTransform();
    ret = parentTransform * ret;
    return ret;
}

uint32_t Node::processParentFlags(const Matrix & parentTransform, uint32_t parentFlags)
{
    uint32_t flags = (m_transformDirtys.testFlag(TransformDirty_ModelView) ? FLAGS_GEOMETRY_DIRTY : 0) | parentFlags;

//     flags |= (_contentSizeDirty ? FLAGS_CONTENT_SIZE_DIRTY : 0);
//     if(_usingNormalizedPosition && (flags & FLAGS_CONTENT_SIZE_DIRTY)) {
//         CCASSERT(_parent, "setNormalizedPosition() doesn't work with orphan nodes");
//         Size s = _parent->getContentSize();
//         _position.x = _normalizedPosition.x * s.width;
//         _position.y = _normalizedPosition.y * s.height;
//         _transformUpdated = _transformDirty = _inverseDirty = true;
//     }

    if (flags) {
        // LOG_BASE("[%s]update Transform:flg:%08x", m_name.c_str(), flags);
        m_transformMV = transform(parentTransform);
        m_transformDirtys.setFlag(TransformDirty_ModelView, false);
    }

    return flags;
}

void Node::invalidate(int flag)
{
    m_dirtys.setFlag((FlagDirty)flag);
}

bool Node::isDirty(bool recursive)
{
    if (m_dirtys != 0) {
        return true;
    }

    if (recursive) {
        for (NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it) {
            Node* child = *it;
            if (child && child->isDirty(true)) {
                return true;
            }
        }
    }
    return false;
}

void Node::render() {
    Matrix parentTransform = Director::instance()->matrixStack().getMatrix(MATRIX_STACK_MODELVIEW);
    render(parentTransform, 0);
}

void Node::render(const Matrix &parentTransform, uint32_t parentFlags) {
    // quick return if not visible. children won't be drawn.
    m_dirtys = 0;

    if (!m_visible) {
        return;
    }

    uint32_t flags = processParentFlags(parentTransform, parentFlags);
    MatrixStack& matrixStack = Director::instance()->matrixStack();
    matrixStack.pushMatrix(MATRIX_STACK_MODELVIEW);
    matrixStack.loadMatrix(MATRIX_STACK_MODELVIEW, m_transformMV);

    Director::instance()->lightCache().setMaterial(m_material);

    draw();

    for (NodeList::iterator it = m_children.begin(); it != m_children.end(); ++it) {
        (*it)->render(m_transformMV, flags);
    }

    matrixStack.popMatrix(MATRIX_STACK_MODELVIEW);

}

void Node::draw() {
    // LOG_BASE("Draw:%s", m_name.c_str());
    onDraw();
}

void Node::initGeometryBuffer()
{
    int count = sizeof(m_buffersVBO) / sizeof (m_buffersVBO[0]);
    // GLHook::glDeleteBuffers(count, &m_buffersVBO[0]);
    GLHook::glGenBuffers(count, &m_buffersVBO[0]);
    onInitGeometryBuffer();
//    GLHook::glBindBuffer(GL_ARRAY_BUFFER, m_buffersVBO[0]);
//    GLHook::glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices[0]) * 4, m_vertices, GL_STATIC_DRAW);
//    GLHook::glBindBuffer(GL_ARRAY_BUFFER, 0);

//    GLHook::glBindBuffer(GL_ARRAY_BUFFER, m_buffersVBO[1]);
//    GLHook::glBufferData(GL_ARRAY_BUFFER, sizeof(_indices[0]) * 6, _indices, GL_STATIC_DRAW);
//    GLHook::glBindBuffer(GL_ARRAY_BUFFER, 0);
    invalidate(FLAGS_GEOMETRY_DIRTY);
}

bool Node::testIntersection(const Vector2& screenPoint, Vector3& intersect)
{
    Vector3 rayOrig, rayDir;
    screenCoorToRayInNodeTransform(screenPoint, &rayOrig, &rayDir);
    Matrix matNode2World = getNodeToWorldTransform();

    Vector3 intersectInNode;
    if (onTestIntersection(rayOrig, rayDir, intersectInNode)){
        matNode2World.transformPoint(intersectInNode, &intersect);
        LOG_BASE("[node:%lf %lf %lf][world:%lf %lf %lf]",intersectInNode.x,intersectInNode.y,intersectInNode.z,intersect.x,intersect.y,intersect.z);
        return true;
    }
    else {
        rayDir.normalize();
        if (fabs(rayDir.z) > MATH_EPSILON){
            // if there is no intersection, test intersection with surface z=-0.85
            float factorZ = (-0.85f - rayOrig.z)/rayDir.z;
            intersectInNode = rayOrig + rayDir*factorZ;
            matNode2World.transformPoint(intersectInNode, &intersect);
            return true;
        }
        else {
            return false;
        }
    }

    return false;
}

void Node::runAction(Action* action)
{
    Director::instance()->actionCache().addAction(action);
}

class TimerTransition : public IntervalAction
{
public:
    TimerTransition(Node* node, int id, float duration) :
        IntervalAction(node),
        m_id(id)
    {
        initTimeline(duration);
    }

    ~TimerTransition() {

    }

    virtual void onEnd(Node* target)
    {
        if (target) {
            target->onTimeOut(m_id);
        }
    }

public:
    int m_id;
};

void Node::setTimer(int id, float duration)
{
    runAction(new TimerTransition(this, id, duration));
}

