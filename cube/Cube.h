#ifndef CUBE_H
#define CUBE_H

#include "Node.h"
#include "DataTypes.h"
class Texture;
class Cube : public Node
{
    friend class CubePlane;
public:
    Cube(const std::string& name = "", int stepsPerPlane = 1, Node *parent = 0);
    virtual ~Cube();

    virtual void onInitGeometryBuffer();
    virtual void onUpdate(float dt);
    virtual void onDraw();
    virtual void onTouch(TouchAction touchevent, float x, float y);
    virtual bool onTestIntersection(const Vector3& orig, const Vector3& dir, Vector3& intersect);


    void setDummyMode(bool enable);
    void doGyro();
    void setPlaneTexture(int planeid, int txtid);
    int getLastTouchDownPlane();
    int getIntersectPlane();
    int getPlaneTexture(int planeid);

protected:
    void drawCube();
    void drawCubeMirror();
    void drawIntersection();
    void drawDummy();
    bool getIntersectPlaneVector(Vector3& normal, Vector3& up);

    enum PLANE_ID {
        PLANE_INVALID = -1,
        PLANE_FRONT = 0,
        PLANE_BACK,
        PLANE_UP,
        PLANE_DOWN,
        PLANE_LEFT,
        PLANE_RIGHT,

        PLANE_NUM,
    };

    enum {
        VERTICES_PER_CELL = 6,
    };


    int m_stepsPerPlane;
    int m_verticesPerPlane;
    int m_verticesNum;

    struct V3F_N3F_T2F_C4F
    {
        Vector3 position;
        Vector3 normal;
        Tex2F   texCoords;
        Color4F colors;
    };
    V3F_N3F_T2F_C4F* m_vertices;

    struct PlaneInfo {
        // Texture* texture;
        Vector3 normal;
        Vector3 up;
        int verticesStart;
        int verticesCount;
        int textureId;
    } m_planeInfos[PLANE_NUM];

    int m_intersectVertexIdx;
    int m_intersectPlane;
    int m_touchDownPlane;

    bool m_isDummy;
};

#endif // CUBE_H
