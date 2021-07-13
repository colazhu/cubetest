#ifndef _Model3DInstance_h_
#define _Model3DInstance_h_

#include "CEGUI/Model3D.h"
#include "MEM_map.h"
#include "CEGUI/Model3DDefine.h"
#include "CEGUI/Model3DAnimation.h"
#include "CEGUI/Model3DRenderer.h"

namespace CEGUI
{
class Model3DCallback
{
public:
    Model3DCallback() {};
    virtual void handleCallback(String type, String name) = 0;
};

class CEGUIEXPORT Model3DInstance : public AllocatedObject<Model3DInstance>
{
public:
    struct NodeInfo
    {
        Mat4x4f transform;
        Vector4f color;
        Mat4x4f boneTransform;
        bool isBoneUpdated;
    };
    
    friend class Model3D;
    friend class Model3DAnimation;
    friend class AnimationController;
    friend class AnimationProperty;
    
    Model3DInstance(Model3D& model3d, Model3DRenderer& renderer);
    ~Model3DInstance(){};
    void initialise();
    
    bool isAnimationRunning();
    void startAnimation(const String& animName, bool isForward = true);
    void stopAnimation(const String& animName);
    void pauseAnimation(const String& animName);
    void resumeAnimation(const String& animName);
    void killAllAnimation();
    void setPosition(String animName, float position);
    
    Model3D& getModel3D();
    Model3DRenderer& getModel3DRenderer() { return m_model3DRenderer; }
    
    void setRenderSize(int width, int height) { m_model3DRenderer.setTextureSize(width, height); }
    Texture& getTexture() { return m_model3DRenderer.getTexture(); }
    
    void moveModel(const Vector2f& offset);
    void clickModel(const Vector2f& pos);
    
    bool step(float deltatime);
    void render();
    
    const NodeInfo& getNodeInfo(const String& nodeName) { return m_nodeInfo[nodeName]; }
    void setNodeInfo(const String& nodeName, const NodeInfo& info) {
        memcpy(m_nodeInfo[nodeName].transform, info.transform, sizeof(info.transform));
        m_nodeInfo[nodeName].color = info.color;
        memcpy(m_nodeInfo[nodeName].boneTransform, info.boneTransform, sizeof(info.boneTransform));
        m_nodeInfo[nodeName].isBoneUpdated = info.isBoneUpdated;
    }

    const Model3DAnimation::AnimationData& getAnimationData(const String& animName) { return d_animations[animName]; }
    const String& getDefaultAnimation() { return d_animations.begin()->first; }
    bool hasAnimation() { return d_animations.size(); }
    
    const Camera& getCamera() const { return m_camera; }
    void setCamera(const Camera& camera) { m_camera = camera; }
    
    void registerCallback(Model3DCallback* callback) { m_callback = callback; }
    
    bool m_forceUpdate;
    
private:
    Model3D& m_model3D;
    Model3DRenderer& m_model3DRenderer;
    
    Camera m_camera;
    std::map<String, NodeInfo>::type m_nodeInfo;
    
    Model3DCallback* m_callback;

    std::map<String, Model3DAnimation::AnimationData>::type d_animations;
    

};

}

#endif	// end of guard _CEGUIMemoryAllocatedObject_h_


