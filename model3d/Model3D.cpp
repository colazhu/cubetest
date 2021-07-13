#include "CEGUI/Model3D.h"
#include "CEGUI/Model3DManager.h"
#include "CEGUI/Model3DAnimation.h"
#include "CEGUI/Model3DInstance.h"
#include "CEGUI/Model3DDefine.h"
#include "CEGUI/System.h"
#include "CEGUI/DefaultResourceProvider.h"
#include "glm_3d/glm/glm.hpp"
#include "glm_3d/glm/gtc/matrix_transform.hpp"
#include "glm_3d/glm/gtc/type_ptr.hpp"
#include "glm_3d/glm/gtx/matrix_decompose.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/cimport.h"
#include "MEM_stack.h"
#include "CEGUI/Texture.h"
#include <iostream>
#include <sys/stat.h>
// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
void aiMatrix4_to_mat4(glm::mat4& des, aiMatrix4x4 src)
{
    src.Transpose();
    float ff[16];
    memcpy(ff, &src, 16*sizeof(ai_real));
    float f[16];
    ai_real* ptr = &src.a1;
    for(int i = 0; i < 16; i++) {
        f[i] =* (ptr++);
    }
    des = glm::make_mat4(f);
}
    
//----------------------------------------------------------------------------//
void processMaterial(const aiScene& scene, std::vector<Material*>::type& materials, const String& folderPath, const String& modelName)
{
    for (int i = 0; i < scene.mNumMaterials; i++)
    {
        aiMaterial* matOrignal = scene.mMaterials[i];
        Material* matTarget = new Material();
        matTarget->name = matOrignal->GetName().C_Str();
        materials.push_back(matTarget);
        
        C_STRUCT aiColor4D diffuse;
        C_STRUCT aiColor4D specular;
        C_STRUCT aiColor4D ambient;
        C_STRUCT aiColor4D emissive;
        C_STRUCT aiColor4D transparent;
        ai_real shininess, strength;
        aiString fileName;
        
#define loadTexture(a, b, c, d) \
    fileName.Clear(); \
    if (AI_SUCCESS == aiGetMaterialTexture(matOrignal, a, 0, &fileName)) { \
        String finalName = folderPath + fileName.C_Str(); \
        String finalFileName = "Model3D_" + modelName + "_" + (String)fileName.C_Str(); \
        struct stat buffer; \
        if (stat(finalName.c_str(), &buffer) == 0) { \
            Texture* tex = &CEGUI::System::getSingleton().getRenderer()->createTexture(finalFileName.c_str(), finalName.c_str(), ""); \
            matTarget->b.tex = tex; \
        } \
    } \
    else if (AI_SUCCESS == aiGetMaterialColor(matOrignal, c, &b)) b.toArray(d); \
    matTarget->b.color = Vector4f(d[0], d[1], d[2], d[3]);
        
        float d[4] = {0.8f, 0.8f, 0.8f, 1.0f};
        loadTexture(aiTextureType_DIFFUSE, diffuse, AI_MATKEY_COLOR_DIFFUSE, d);
        
        float a[4] = {0.2f, 0.2f, 0.2f, 1.0f};
        loadTexture(aiTextureType_AMBIENT, ambient, AI_MATKEY_COLOR_AMBIENT, a);
        
        float s[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        loadTexture(aiTextureType_SPECULAR, specular, AI_MATKEY_COLOR_SPECULAR, s);
        
        float e[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        loadTexture(aiTextureType_EMISSIVE, emissive, AI_MATKEY_COLOR_EMISSIVE, e);

#undef loadTexture
        
        fileName.Clear();
        if (AI_SUCCESS == aiGetMaterialTexture(matOrignal, aiTextureType_NORMALS, 0, &fileName)) {
            String finalName = folderPath + fileName.C_Str();
            struct stat buffer;
            if (stat(finalName.c_str(), &buffer) == 0) {
                Texture* tex = &CEGUI::System::getSingleton().getRenderer()->createTexture(fileName.C_Str(), finalName.c_str(), "");
                matTarget->normals.tex = tex;
            }
        }
        
        float f[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        if (AI_SUCCESS == aiGetMaterialColor(matOrignal, AI_MATKEY_COLOR_TRANSPARENT, &transparent)) transparent.toArray(f);
        matTarget->transparent = f[3];
        
        if (AI_SUCCESS == aiGetMaterialFloat(matOrignal, AI_MATKEY_SHININESS, &shininess)) matTarget->shininess = shininess;
        else matTarget->shininess = 0.0f;
        
        if (AI_SUCCESS == aiGetMaterialFloat(matOrignal, AI_MATKEY_SHININESS_STRENGTH, &strength)) matTarget->strength = strength;
        else matTarget->strength = 0.0f;
    }
}
    
//----------------------------------------------------------------------------//
Mesh* processMesh(aiMesh& meshOrigin, std::map<String, Node>::type& nodeOriginList)
{
    Mesh* meshTarget = new Mesh();
    
    for (unsigned int i = 0; i < meshOrigin.mNumVertices; i++) {
        Mesh::Vertex vertex;
        Vector3f vector;
        Vector4f vector4;
        // positions
        vector.d_x = meshOrigin.mVertices[i].x;
        vector.d_y = meshOrigin.mVertices[i].y;
        vector.d_z = meshOrigin.mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (meshOrigin.mNormals) {
            vector.d_x = meshOrigin.mNormals[i].x;
            vector.d_y = meshOrigin.mNormals[i].y;
            vector.d_z = meshOrigin.mNormals[i].z;
            vertex.Normal = vector;
        }
        //color
        if (meshOrigin.mColors[0]) {
            vector4.d_x = meshOrigin.mColors[0][i].r;
            vector4.d_y = meshOrigin.mColors[0][i].g;
            vector4.d_z = meshOrigin.mColors[0][i].b;
            vector4.d_w = meshOrigin.mColors[0][i].a;
            vertex.Color = vector4;
        }
        // texture coordinates
        if (meshOrigin.mTextureCoords[0]) {
            Vector2f vec;
            vec.d_x = meshOrigin.mTextureCoords[0][i].x;
            vec.d_y = meshOrigin.mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else {
            vertex.TexCoords = Vector2f(0.0f, 0.0f);
        }
        // tangent
        if (meshOrigin.mTangents) {
            vector.d_x = meshOrigin.mTangents[i].x;
            vector.d_y = meshOrigin.mTangents[i].y;
            vector.d_z = meshOrigin.mTangents[i].z;
            vertex.Tangent = vector;
        }
        // bitangent
        if (meshOrigin.mBitangents) {
            vector.d_x = meshOrigin.mBitangents[i].x;
            vector.d_y = meshOrigin.mBitangents[i].y;
            vector.d_z = meshOrigin.mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        meshTarget->m_vertices.push_back(vertex);
    }
    // indice
    for (unsigned int i = 0; i < meshOrigin.mNumFaces; i++) {
        aiFace& face = meshOrigin.mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            meshTarget->m_indices.push_back(face.mIndices[j]);
        }
    }
    // bone
    int boneID = 0;
    for (int i = 0; i < meshOrigin.mNumBones; i++) {
        aiBone* bone = meshOrigin.mBones[i];
        auto it = nodeOriginList.find(bone->mName.C_Str());
        if (it == nodeOriginList.end()) continue;
        // get bone position
        int id = -1;
        if (it->second.m_boneID == -1) {
            id = boneID++;
            it->second.m_boneID = id;
        }
        else id = it->second.m_boneID;
        // insert bone weight to vertex
        for (int j = 0; j < bone->mNumWeights; j++) {
            Mesh::Vertex& vertex = meshTarget->m_vertices[bone->mWeights[j].mVertexId];
            for (int k = 0; k < 9; k++) {
                if (vertex.BoneWeight[k] != 0) continue;

                vertex.BoneID[k] = id;
                vertex.BoneWeight[k] = bone->mWeights[j].mWeight;
                break;
            }
        }
        memcpy(nodeOriginList[bone->mName.C_Str()].m_offsetMatrix, &bone->mOffsetMatrix.Transpose(), 16*sizeof(ai_real));
    }
    // material
    meshTarget->m_materialID = meshOrigin.mMaterialIndex;
    
    return meshTarget;
}
    
//----------------------------------------------------------------------------//
Node* processNode(const aiScene& scene, std::map<String, Node>::type& nodes, std::vector<Mesh*>::type& meshes)
{
    aiNode* t_ainode = scene.mRootNode;
    StdStack<aiNode*>::type t_nodes;
    Node* rootNode = NULL;
    std::vector<std::pair<aiMesh*, Node*>>::type t_mesh2Node;
    
    while (t_ainode) {
        String nodeName = t_ainode->mName.C_Str();
        Node* t_node = &(nodes)[nodeName];
        t_node->m_name = nodeName;
        
        Mat4x4f t_trans;
        memcpy(t_trans, &t_ainode->mTransformation.Transpose(), 16*sizeof(ai_real));
        memcpy(t_node->m_transformation, t_trans, sizeof(t_trans));;
        
        if (t_ainode->mParent) {
            t_node->m_parent = &(nodes.at(t_ainode->mParent->mName.C_Str()));
        }
        else rootNode = t_node;
        
        for (unsigned int i = 0; i < t_ainode->mNumMeshes; i++) {
            t_mesh2Node.push_back(std::pair<aiMesh*, Node*>(scene.mMeshes[t_ainode->mMeshes[i]], t_node));
        }
        for (int i = t_ainode->mNumChildren - 1; i >= 0; i--) {
            String childName = t_ainode->mChildren[i]->mName.C_Str();
            if (nodes.find(childName) != nodes.end()) childName = childName.append("_1");
            t_ainode->mChildren[i]->mName = childName.c_str();
            t_node->m_children.push_back(&nodes[childName]);
            t_nodes.push(t_ainode->mChildren[i]);
        }
        if(t_nodes.empty()) break;
        
        t_ainode = t_nodes.top();
        t_nodes.pop();
    }

    for (int i = 0; i < t_mesh2Node.size(); i++) {
        meshes.push_back(processMesh(*(t_mesh2Node[i].first), nodes));
        meshes.back()->m_parent = t_mesh2Node[i].second;
        t_mesh2Node[i].second->m_meshides.push_back(meshes.size() - 1);
    }
    
    return rootNode;
}

//----------------------------------------------------------------------------//
void processLight(const aiScene& scene, std::vector<Light*>::type& lights)
{
    for (int i = 0; i < scene.mNumLights; i++) {
        Light* light = new Light();
        aiLight* aLight = scene.mLights[i];
        light->name = aLight->mName.C_Str();
        light->position = Vector3f(aLight->mPosition.x, aLight->mPosition.y, aLight->mPosition.z);
        lights.push_back(light);
    }
    
}
void processAnimation(const aiScene& scene, std::map<String, Node>::type& nodes, Model3DAnimation& model3DAnimation)
{
    for (int i = 0; i < scene.mNumAnimations; i++) {
        aiAnimation* animation = scene.mAnimations[i];

        for(int j = 0; j < animation->mNumChannels; j++) {
            aiNodeAnim* channel = animation->mChannels[j];
            NodeProperty* nodeProperty = new NodeProperty();
            nodeProperty->d_nodeName = (String)channel->mNodeName.C_Str();
            nodeProperty->d_duration = animation->mDuration;
            nodeProperty->d_ticksPerSecond = animation->mTicksPerSecond;
            nodes[(String)channel->mNodeName.C_Str()].m_anims.push_back((String)animation->mName.C_Str());
            
            glm::mat4 nodeTrans = glm::make_mat4(nodes.at(channel->mNodeName.C_Str()).m_transformation);
            glm::vec3 scale;
            glm::quat rotation;
            glm::vec3 translation;
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose(nodeTrans, scale, rotation, translation, skew, perspective);
            
            for(int k = 0; k < channel->mNumPositionKeys; k++) {
                NodeProperty::KeyFramePosition key;
                key.tick = channel->mPositionKeys[k].mTime;
                key.position.d_x = channel->mPositionKeys[k].mValue.x - translation.x;
                key.position.d_y = channel->mPositionKeys[k].mValue.y - translation.y;
                key.position.d_z = channel->mPositionKeys[k].mValue.z - translation.z;
                nodeProperty->addKeyFramePosition(key);
            }
            for(int k = 0; k < channel->mNumRotationKeys; k++) {
                NodeProperty::KeyFrameRotation key;
                key.tick = channel->mRotationKeys[k].mTime;
                glm::quat tmp(channel->mRotationKeys[k].mValue.w, channel->mRotationKeys[k].mValue.x, channel->mRotationKeys[k].mValue.y, channel->mRotationKeys[k].mValue.z);
                glm::quat result = glm::conjugate(rotation) * tmp;
                key.rotation.d_w = result.w;
                key.rotation.d_x = result.x;
                key.rotation.d_y = result.y;
                key.rotation.d_z = result.z;
                nodeProperty->addKeyFrameRotation(key);
            }
            for(int k = 0; k < channel->mNumScalingKeys; k++) {
                NodeProperty::KeyFrameScaling key;
                key.tick = channel->mScalingKeys[k].mTime;
                key.scaling.d_x = channel->mScalingKeys[k].mValue.x / scale.x;
                key.scaling.d_y = channel->mScalingKeys[k].mValue.y / scale.y;
                key.scaling.d_z = channel->mScalingKeys[k].mValue.z / scale.z;
                nodeProperty->addKeyFrameScaling(key);
            }
            model3DAnimation.d_animationControllers[(String)animation->mName.C_Str()].d_animationProperties.push_back(nodeProperty);
            model3DAnimation.d_animationControllers[(String)animation->mName.C_Str()].animName = (String)animation->mName.C_Str();
            model3DAnimation.d_animationControllers[(String)animation->mName.C_Str()].d_parent = &model3DAnimation;
        }
    }
}

//----------------------------------------------------------------------------//
void processCamera(const aiScene& scene, std::vector<Camera*>::type& cameras)
{
    for (int i = 0; i < scene.mNumCameras; ++i) {
        Camera* camera = new Camera();
        camera->angle = 0;
        camera->position.d_x = scene.mCameras[i]->mPosition.x;
        camera->position.d_y = scene.mCameras[i]->mPosition.y;
        camera->position.d_z = scene.mCameras[i]->mPosition.z;
        camera->up.d_x = scene.mCameras[i]->mUp.x;
        camera->up.d_y = scene.mCameras[i]->mUp.y;
        camera->up.d_z = scene.mCameras[i]->mUp.z;
        camera->fovy = scene.mCameras[i]->mHorizontalFOV / scene.mCameras[i]->mAspect;
        camera->near = scene.mCameras[i]->mClipPlaneNear;
        camera->far = scene.mCameras[i]->mClipPlaneFar;
        
        glm::vec3 move = glm::vec4(scene.mCameras[i]->mLookAt.x, scene.mCameras[i]->mLookAt.y, scene.mCameras[i]->mLookAt.z, 0) * glm::rotate(glm::mat4(1.0f), (float)glm::radians(90.0f), glm::vec3(camera->up.d_x, camera->up.d_y, camera->up.d_z));
        camera->target.d_x = camera->position.d_x + move.x;
        camera->target.d_y = camera->position.d_y + move.y;
        camera->target.d_z = camera->position.d_z + move.z;
        
        cameras.push_back(camera);
    }
}

//----------------------------------------------------------------------------//
void Node::initialiseTarget(Model3DInstance& target)
{
    Model3DInstance::NodeInfo info;
    // initialise transform
    if (m_parent) {
        glm::mat4 tempTransform = glm::make_mat4(target.getNodeInfo(m_parent->m_name).transform) * glm::make_mat4(m_transformation);
        memcpy(info.transform, glm::value_ptr(tempTransform), 16 * sizeof(float));
    }
    else {
        memcpy(info.transform, m_transformation, 16 * sizeof(float));
    }
    // initialise color
    info.color = Vector4f(0, 0, 0, 0);
    // initialise bone
    if (m_boneID != -1) {
        memcpy(info.boneTransform, glm::value_ptr(glm::make_mat4(info.transform) * glm::make_mat4(m_offsetMatrix)), 16 * sizeof(float));
        info.isBoneUpdated = true;
    }
    else info.isBoneUpdated = false;

    target.setNodeInfo(m_name, info);

    // initialise children
    for (int i = 0; i < m_children.size(); ++i) {
        m_children[i]->initialiseTarget(target);
    }
}

//----------------------------------------------------------------------------//
void Node::update(Model3DInstance& target, bool needUpdate)
{
    // check this node's info need to be updated
    bool isAnimUpdated = false;
    if (needUpdate) isAnimUpdated = true;
    else {
        for (int i = 0; i < m_anims.size(); ++i) {
            if (target.getAnimationData(m_anims[i]).updated) {
                isAnimUpdated = true;
                break;
            }
        }
    }

    // update node info
    if (!isAnimUpdated) {}
    else {
        Model3D::GlobalInfo globalInfo;
        for (int i = 0; i < m_anims.size(); ++i) {
            target.getModel3D().getModel3DAnimation().getPropertyByPosition(target, m_anims[i], m_name, target.getAnimationData(m_anims[i]).position, globalInfo);
        }
        // add anim trans
        glm::mat4 TranslationM(1.0f);
        TranslationM = glm::translate(TranslationM, glm::vec3(globalInfo.position.d_x, globalInfo.position.d_y, globalInfo.position.d_z));
        // add anim rotate
        aiQuaternion rotation(globalInfo.rotation.d_w, globalInfo.rotation.d_x, globalInfo.rotation.d_y, globalInfo.rotation.d_z);
        glm::mat4 RotationM(1.0f);
        aiMatrix4_to_mat4(RotationM, aiMatrix4x4(rotation.GetMatrix()));
        // add anim scale
        glm::mat4 ScalingM(1.0f);
        ScalingM = glm::scale(ScalingM, glm::vec3(globalInfo.scaling.d_x, globalInfo.scaling.d_y, globalInfo.scaling.d_z));
        
        glm::mat4 globalTransform = TranslationM * RotationM * ScalingM;
        glm::mat4 nodeTransform = glm::make_mat4(m_transformation) * globalTransform;
        // update transform property
        glm::mat4 tempTransform = glm::mat4(1.0f);
        // add parent
        if (m_parent) {
            tempTransform = glm::make_mat4(target.getNodeInfo(m_parent->m_name).transform) * nodeTransform;
        }
        else {
            tempTransform = nodeTransform;
        }

        // update color property
        Vector4f tempColor;
        if (m_parent) {
            tempColor = target.getNodeInfo(m_parent->m_name).color + globalInfo.color;
        }
        else {
            tempColor = globalInfo.color;
        }
        
        Model3DInstance::NodeInfo info;
        memcpy(info.transform, glm::value_ptr(tempTransform), 16 * sizeof(float));
        info.color = tempColor;
        if (m_boneID != -1) {
            // update bone
            memcpy(info.boneTransform, glm::value_ptr(tempTransform * glm::make_mat4(m_offsetMatrix)), 16 * sizeof(float));
            info.isBoneUpdated = true;
        }
        
        target.setNodeInfo(m_name, info);
    }
    
    // update children node
    for (int i = 0; i < m_children.size(); ++i) {
        m_children[i]->update(target, isAnimUpdated);
    }
}

//----------------------------------------------------------------------------//
Model3DCamera::~Model3DCamera()
{
    for (int i = 0; i < m_cameras.size(); ++i) {
        delete m_cameras[i];
    }
}

//----------------------------------------------------------------------------//
void Model3DCamera::initialiseTarget(Model3DInstance& target)
{
    if (m_cameras.size()) target.setCamera(*m_cameras[0]);
}

//----------------------------------------------------------------------------//
void Model3DCamera::update(Model3DInstance& target)
{
    for (int i = 0; i < m_anims.size(); ++i) {
        if (target.getAnimationData(m_anims[i]).updated) {
            Model3D::GlobalInfo globalInfo;
            String nodeName = "";
            target.getModel3D().getModel3DAnimation().getPropertyByPosition(target, m_anims[i], nodeName, target.getAnimationData(m_anims[i]).position, globalInfo);
            target.setCamera(globalInfo.camera);
            break;
        }
    }
}
    
//----------------------------------------------------------------------------//
Model3D::Model3D(const String& modelName, const String& filePath) : d_model3DAnimation(new Model3DAnimation())
{
    m_name = modelName;
    createFromFile(filePath);
}

//----------------------------------------------------------------------------//
Model3D::~Model3D()
{
    for (int i = 0; i < m_lights.size(); ++i) {
        delete m_lights[i];
    }
    for (int i = 0; i < m_materials.size(); ++i) {
        if (m_materials[i]->ambient.tex) CEGUI::System::getSingleton().getRenderer()->destroyTexture(m_materials[i]->ambient.tex->getName());
        if (m_materials[i]->diffuse.tex) CEGUI::System::getSingleton().getRenderer()->destroyTexture(m_materials[i]->diffuse.tex->getName());
        if (m_materials[i]->specular.tex) CEGUI::System::getSingleton().getRenderer()->destroyTexture(m_materials[i]->specular.tex->getName());
        if (m_materials[i]->emissive.tex) CEGUI::System::getSingleton().getRenderer()->destroyTexture(m_materials[i]->emissive.tex->getName());
        if (m_materials[i]->normals.tex) CEGUI::System::getSingleton().getRenderer()->destroyTexture(m_materials[i]->normals.tex->getName());
        delete m_materials[i];
    }
    for (int i = 0; i < m_meshes.size(); ++i) {
        delete m_meshes[i];
    }
    delete d_model3DAnimation;
}
    
//----------------------------------------------------------------------------//
void Model3D::initialiseTarget(Model3DInstance& target)
{
    // initialise default camera
    m_camera.initialiseTarget(target);
    
    // initialise node info
    if (d_rootNode) d_rootNode->initialiseTarget(target);
    
    // initialise animation
    if (d_model3DAnimation) d_model3DAnimation->initializeTarget(target);
    
}

//----------------------------------------------------------------------------//
void Model3D::startAnimation(Model3DInstance& target, const String& name, bool isForward)
{
    d_model3DAnimation->startAnimation(target, name, (Model3DAnimation::ANIM_DIRECTION)isForward);
}

//----------------------------------------------------------------------------//
void Model3D::pauseAnimation(Model3DInstance& target, const String& name)
{
    d_model3DAnimation->pauseAnimation(target, name);
}

//----------------------------------------------------------------------------//
void Model3D::stopAnimation(Model3DInstance& target, const String& name)
{
    d_model3DAnimation->stopAnimation(target, name);
}

//----------------------------------------------------------------------------//
bool Model3D::step(Model3DInstance& target, float delta)
{
    return d_model3DAnimation->step(target, delta);
}

//----------------------------------------------------------------------------//
void Model3D::render(Model3DInstance& target)
{
    // update camera
    m_camera.update(target);
    // update node
    if (d_rootNode) d_rootNode->update(target, false);
    
    // start render
    target.getModel3DRenderer().beginRendering(target);
    for (int i = 0; i < m_meshes.size(); ++i) {
        target.getModel3DRenderer().render(target, i);
    }
    target.getModel3DRenderer().endRendering(target);
}

//----------------------------------------------------------------------------//
void Model3D::createFromFile(const String& filePath)
{
    Assimp::Importer importer;
    const aiScene* pScene = importer.ReadFile(filePath.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_FixInfacingNormals | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_CalcTangentSpace);
    if(!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode) // if is Not Zero
        return;
    int pos = filePath.find_last_of('/');
    String folderPath = filePath.substr(0, pos+1);
    processMaterial(*pScene, m_materials, folderPath, m_name);
    d_rootNode = processNode(*pScene, m_nodes, m_meshes);
    processAnimation(*pScene, m_nodes, *d_model3DAnimation);
    processLight(*pScene, m_lights);
    processCamera(*pScene, m_camera.m_cameras);
}

//----------------------------------------------------------------------------//
void Model3D::createFromMemory(const void *pBuffer, const size_t pLength)
{
    Assimp::Importer importer;
    const aiScene* pScene = importer.ReadFileFromMemory(pBuffer, pLength, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices  | aiProcess_FixInfacingNormals | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_CalcTangentSpace);
    if(!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode) // if is Not Zero
        return;

    processMaterial(*pScene, m_materials, "", m_name);
    d_rootNode = processNode(*pScene, m_nodes, m_meshes);
    processAnimation(*pScene, m_nodes, *d_model3DAnimation);
    processLight(*pScene, m_lights);
    processCamera(*pScene, m_camera.m_cameras);
}

//----------------------------------------------------------------------------//
const Node& Model3D::getNode(const String& name) const
{
    return m_nodes.at(name);
}

//----------------------------------------------------------------------------//
void Model3D::addNodeAnim(const String& nodeName, const String& animName)
{
    m_nodes.at(nodeName).m_anims.push_back(animName);
}
} // End of  CEGUI namespace section
