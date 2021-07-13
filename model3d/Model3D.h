#ifndef _Model3D_h_
#define _Model3D_h_

#include "CEGUI/Vector.h"
#include "CEGUI/String.h"
#include "CEGUI/Quaternion.h"
#include "CEGUI/Model3DDefine.h"
#include "CEGUI/Model3D_xmlHandler.h"
#include <map>
#include <vector>
#include <string>
// Start of CEGUI namespace section

class Model3DAnimation;
class Node;
class Model3DInstance;

struct MaterialProperty
{
    Vector4f color = Vector4f(0.0, 0.0, 0.0, 0.0);
    Texture* tex = nullptr;
};

class Material
{
public:
    std::string name;
    float transparent;
    float shininess;
    float strength;
    
    MaterialProperty diffuse;
    MaterialProperty ambient;
    MaterialProperty specular;
    MaterialProperty emissive;
    MaterialProperty normals;
};

class Mesh
{
public:
    struct Vertex {
        Vector3f Position;
        Vector3f Normal = Vector3f(0,0,0);
        Vector2f TexCoords;
        Vector3f Tangent = Vector3f(0,0,0);
        Vector3f Bitangent = Vector3f(0,0,0);
        Vector4f Color = Vector4f(0,0,0,0);
        // for bone
        float BoneID[9] = {0};
        float BoneWeight[9] = {0};
    };
    
    Mesh() {}

    // vertices
    std::vector<Mesh::Vertex> m_vertices;
    // indices
    std::vector<unsigned int> m_indices;
    // material
    int m_materialID;
    // parent
    const Node* m_parent;
};
    
class Node
{
public:    
    Node() {m_parent = NULL; m_boneID = -1; }
    ~Node() {}

    void initialiseTarget(Model3DInstance& target);
    void update(Model3DInstance& target, bool needUpdate);

    // name
    std::string m_name;
    // parent
    Node* m_parent;
    // children
    std::vector<Node*> m_children;
    // meshes
    std::vector<int> m_meshides;
    // trasnform
    Mat4x4f m_transformation;
    // anim list
    std::vector<String> m_anims;
    // bone offset
    int m_boneID;
    Mat4x4f m_offsetMatrix;
};

// TODO: support different type of light
class Light
{
public:
    std::string name;
    Vector3f position;
    float constant;
    float linear;
    float quadratic;
    Vector3f ambient;
    Vector3f diffuse;
    Vector3f specular;
    Light() {
        position = Vector3f(-10.0f, 22.0f, -10.0f);
        constant = 1.0f;
        linear = 0.014f;
        quadratic = 0.0007f;
        Vector3f intansity(0.0f, 0.0f, 0.0f);
        diffuse = Vector3f(0.8f, 0.8f, 0.8f)*2.0f;
        ambient = Vector3f(0.0f, 0.0f, 0.0f)*2.0f;
        specular = Vector3f(0.5f, 0.5f, 0.5f)*2.0f;
    }
};

class Camera
{
public:
    Vector3f position;
    Vector3f target;
    Vector3f up;
    float fovy;
    float near;
    float far;
    
    float angle;
    
    Camera() {
        position = Vector3f(0.0f, 157.0f, 1111.0f);
        target = Vector3f(12.0f, 60.0f, 0.0f);
        up = Vector3f(0.0f, 1.0f, 0.0f);
        angle = 0;
        fovy = 0.26f;
        near = 100.0f;
        far = 100000.0f;
    }
};

class Model3DCamera
{
public:
    Model3DCamera() {}
    ~Model3DCamera();
    void initialiseTarget(Model3DInstance& target);
    void update(Model3DInstance& target);

    std::vector<Camera*> m_cameras;
    std::vector<String> m_anims;

};
    
class Model3D
{
public:
    struct GlobalInfo
    {
        Vector3f position;
        Quaternion rotation;
        Vector3f scaling;
        Vector4f color;
        Camera camera;
        GlobalInfo() {
            position = Vector3f(0.0, 0.0, 0.0);
            rotation = Quaternion(1.0, 0.0, 0.0, 0.0);
            scaling = Vector3f(1.0, 1.0, 1.0);
            color = Vector4f(0.0, 0.0, 0.0, 0.0);
        };
    };
    
    Model3D(const String& modelName, const String& filename);
    ~Model3D();
    
    void initialiseTarget(Model3DInstance& target);

    friend class Model3D_xmlHandler;

    const String& getName() const { return m_name; }
    const Node& getNode(const String& name) const;
    const std::vector<Light*>& getAllLights() const { return m_lights; }
    const std::vector<Mesh*>& getAllMeshes() const { return m_meshes; }
    const std::vector<Material*>& getAllMaterials() const { return m_materials; }
    const std::map<String, Node>& getAllNodes() const { return m_nodes; }

    // start animation    
    void startAnimation(Model3DInstance& target, const String& name, bool isForward = true);
    // pause animation
    void pauseAnimation(Model3DInstance& target, const String& name);
    // stop animation
    void stopAnimation(Model3DInstance& target, const String& name);
    // kill all animation
    void killAllRunningAnimation(Model3DInstance& target);
    // update animation for every frame
    bool step(Model3DInstance& target, float delta);
    // draw model
    void render(Model3DInstance& target);

    const Node& getRootNode() { return *d_rootNode; }
    const Model3DAnimation& getModel3DAnimation() { return *d_model3DAnimation; }

    void addNodeAnim(const String& nodeName, const String& animName);

private:
    void createFromFile(const String& filePath);
    void createFromMemory(const void *pBuffer, const size_t pLength);

    std::string m_name;
    std::map<String, Node> m_nodes;
    std::vector<Light*> m_lights;
    std::vector<Material*> m_materials;
    Model3DCamera m_camera;
    std::vector<Mesh*> m_meshes;
    
    Model3DAnimation* d_model3DAnimation;
    Node* d_rootNode;
};
#endif


