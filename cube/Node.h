#ifndef NODE_H
#define NODE_H

#include "NodeDef.h"
#include "Flags.h"
#include "Log.h"
#include "GLFunction.h"
#include "Director.h"
#include "ProgramCache.h"
#include "TextureCache.h"
#include "ActionCache.h"

class Camera;
class Light;
class Action;
class Node;
typedef std::list<Node*> NodeList;

class Node {
public:
    enum FlagDirty
    {
        FLAGS_MATERIAL_DIRTY = (1 << 0),
        FLAGS_GEOMETRY_DIRTY = (1 << 1),
        FLAGS_TEXTURE_DIRTY = (1 << 2),
        FLAGS_DIRTY_MASK = (FLAGS_MATERIAL_DIRTY | FLAGS_GEOMETRY_DIRTY | FLAGS_TEXTURE_DIRTY),
    };
    DECLARE_FLAGS(FlagDirtys, FlagDirty)

    Node(const std::string& name = "", Node *parent = 0);
    virtual ~Node();

    /**
     * Sets the scale (x) of the node.
     *
     * It is a scaling factor that multiplies the width of the node and its children.
     *
     * @param scaleX   The scale factor on X axis.
     *
     * @warning The physics body doesn't support this.
     */
    virtual void setScaleX(float scaleX);

    /**
     * Returns the scale factor on X axis of this node
     *
     * @see setScaleX(float)
     *
     * @return The scale factor on X axis.
     */
    virtual float scaleX() const;

    /**
     * Sets the scale (y) of the node.
     *
     * It is a scaling factor that multiplies the height of the node and its children.
     *
     * @param scaleY   The scale factor on Y axis.
     *
     * @warning The physics body doesn't support this.
     */
    virtual void setScaleY(float scaleY);

    /**
     * Returns the scale factor on Y axis of this node
     *
     * @see `setScaleY(float)`
     *
     * @return The scale factor on Y axis.
     */
    virtual float scaleY() const;

    /**
     * Changes the scale factor on Z axis of this node
     *
     * The Default value is 1.0 if you haven't changed it before.
     *
     * @param scaleY   The scale factor on Y axis.
     *
     * @warning The physics body doesn't support this.
     */
    virtual void setScaleZ(float scaleZ);

    /**
     * Returns the scale factor on Z axis of this node
     *
     * @see `setScaleZ(float)`
     *
     * @return The scale factor on Z axis.
     */
    virtual float scaleZ() const;

    /**
     * Returns the scale factor  of this node
     */
    virtual Vector3 scale() const;

    /**
     * Sets the scale (x,y,z) of the node.
     *
     * It is a scaling factor that multiplies the width, height and depth of the node and its children.
     *
     * @param scale     The scale factor for X, Y, Z axis.
     *
     * @warning The physics body doesn't support this.
     */
    virtual void setScale(float scale);
    virtual void setScale(const Vector3& scale);

    /**
     * Sets the scale (x,y,z) of the node.
     *
     * It is a scaling factor that multiplies the width and height of the node and its children.
     *
     * @param scaleX     The scale factor on X axis.
     * @param scaleY     The scale factor on Y axis.
     * @param scaleZ     The scale factor on Z axis.
     *
     * @warning The physics body doesn't support this.
     */
    virtual void setScale(float scaleX, float scaleY, float scaleZ);

    /**
     * Gets/Sets x or y or z coordinate individually for position.
     * These methods are used in Lua and Javascript Bindings
     */
    virtual void setPositionX(float x);
    virtual float positionX(void) const;
    virtual void setPositionY(float y);
    virtual float positionY(void) const;
    virtual void setPositionZ(float z);
    virtual float positionZ(void) const;
    /**
     * Sets the position (X, Y, and Z) in its parent's coordinate system
     */
    virtual void setPosition(float x, float y, float z);
    virtual void setPosition(const Vector3& position);

    /**
     * Gets the position (x,y,z) of the node in its parent's coordinate system.
     *
     * @see setPosition(const Vector3&)
     *
     * @return The position (x,y,z) of the node in OpenGL coordinates
     */
    virtual const Vector3& position() const
    {
        return m_position;
    }

    /**
     * Sets the rotation (X,Y,Z) in degrees.
     * Useful for 3d rotations
     */
    virtual void setRotation(float degreeX, float degreeY, float degreeZ);
    virtual float getRotationX() { return m_rotation.x; }
    virtual float getRotationY() { return m_rotation.y; }
    virtual float getRotationZ() { return m_rotation.z; }
    virtual void setRotationX(float degree);
    virtual void setRotationY(float degree);
    virtual void setRotationZ(float degree);

    /**
     * Sets the rotation (X,Y,Z) in degrees
     */
    virtual void setRotation(const Vector3& rotation);

    /**
     * returns the rotation (X,Y,Z) in degrees.
     */
    virtual Vector3 rotation() const;

    void setAdditionalTransform(const Matrix& addTransform);

    /**
     * Returns the matrix that transform the node's (local) space coordinates into the parent's space coordinates.
     * The matrix is in Pixels.
     */
    virtual Matrix getNodeToParentTransform();

    /**
     * Returns the matrix that transform parent's space coordinates to the node's (local) space coordinates.
     * The matrix is in Pixels.
     */
    virtual Matrix getParentToNodeTransform();

    /**
     * Returns the world affine transform matrix. The matrix is in Pixels.
     */
    virtual Matrix getNodeToWorldTransform();

    /**
     * Returns the inverse world transform matrix. The matrix is in Pixels.
     */
    virtual Matrix getWorldToNodeTransform();

    /**
     * Converts a Vector3 to node (local) space coordinates. The result is in Points.
     */
    Vector3 convertToNodeSpace(const Vector3& worldPoint);

    /**
     * Converts a Vector3 to world space coordinates. The result is in Points.
     */
    Vector3 convertToWorldSpace(const Vector3& nodePoint);

    /**
     * Convert screen coordinate to vector in 3d world
     * Returns the ray origin position and direction in the world transform
     */
    virtual bool screenCoorToRayInWorldTransform(const Vector2& position,
                                                 Vector3* orig,
                                                 Vector3* dir);

    /**
     * Convert screen coordinate to vector in 3d world
     * Returns the ray origin position and direction in the node transform
     */
    virtual bool screenCoorToRayInNodeTransform(const Vector2& coord,
                                                Vector3* ray,
                                                Vector3* dir);


    void setMaterial(const MaterialData& material)
    {
        m_material = material;
        materialUpdated();
    }

    void setMaterial(unsigned int color)
    {
        m_material.setColor(color);
        materialUpdated();
    }

    MaterialData material() const
    {
        return m_material;
    }

    /**
     * check detail in direct-X sample: pick
     */
    static bool intersectTriangle(const Vector3& orig,
                                  const Vector3& dir,
                                  Vector3& v0,
                                  Vector3& v1,
                                  Vector3& v2,
                                  float* t,
                                  float* u,
                                  float* v);



    // NGSIGNAL(int) updateNode;
    void invalidate(int flag);

    void setTimer(int id, float duration);
    virtual void onTimeOut(int id) { UNUSED(id); }

    Node* parent() const;
    const NodeList& children() const;
    void setParent(Node* parent);
    void bringToTop(Node* child);
    void pushToBottom(Node* child);
    Node* top() const;
    Node* getChild(const std::string& name, bool recursive = false);

    virtual void childAdded(Node* child) { UNUSED(child); }
    virtual void childRemoved(Node* child) { UNUSED(child); }
    virtual void notifyParentChanged(Node *oldParent, Node* newParent) { UNUSED(oldParent); UNUSED(newParent);}

    void pause();
    void resume();
    inline void show() { setVisible(true); }
    inline void hide() { setVisible(false); }

    inline std::string name() {return m_name;}
    inline void setName(const std::string& name) { m_name = name; }


    virtual void onTouch(TouchAction touchevent, float x, float y){}

    void initGeometryBuffer();
    virtual void onInitGeometryBuffer(){}

    void update(float dt);
    virtual void onUpdate(float dt) {  UNUSED(dt); }

    bool testIntersection(const Vector2& screenPoint, Vector3& intersect);
    virtual bool onTestIntersection(const Vector3& orig, const Vector3& dir, Vector3& intersect) { return false; }

    bool isDirty(bool recursive = true);
    void render();
    void render(const Matrix &parentTransform, uint32_t parentFlags);
    void draw();
    virtual void onDraw() {}

    void runAction(Action* action);

protected:
    void static bind(Node* child, Node* parent);
    void static unbind(Node* child);
    void setVisible(bool visible);
    Matrix transform(const Matrix& parentTransform);
    uint32_t processParentFlags(const Matrix & parentTransform, uint32_t parentFlags);

    Camera* getCamera();
    void materialUpdated();

    Node* m_parent;
    NodeList m_children;

    std::string m_name;

    Vector3 m_rotation;         ///< rotation on X-axis,Y-axis,Z-axis
    Vector3 m_scale;            ///< scaling factor on X-axis,Y-axis,Z-axis
    Vector3 m_position;         ///< position of the node

    bool m_visible;

    enum TransformDirty {
        TransformDirty_Self = 0x0001,        ///< transform dirty flag
        TransformDirty_SelfInverse = 0x0002, ///< inverse transform dirty flag
        TransformDirty_ModelView = 0x0004,    ///< global modelview transform

        TransformDirty_All = TransformDirty_Self | TransformDirty_SelfInverse | TransformDirty_ModelView,
    };
    DECLARE_FLAGS(TransformDirtys, TransformDirty)
    TransformDirtys m_transformDirtys;

    Matrix m_transform;         ///< transform
    Matrix m_transformInverse;  ///< inverse transform
    Matrix m_transformAdditional;
    Matrix m_transformMV;       ///< transform

    Camera* m_camera;

    bool m_materialUpdated;
    MaterialData m_material;

    FlagDirtys m_dirtys;
    GLuint m_buffersVBO[1];
    DISABLE_COPY(Node)
};

#endif // NODE_H
