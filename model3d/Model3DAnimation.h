#ifndef _MODEL3DANIMATION_h_
#define _MODEL3DANIMATION_h_

#include "CEGUI.h"
#include "CEGUI/Model3D.h"
#include "MEM_vector.h"
#include "MEM_map.h"
#include "MEM_queue.h"
// Start of CEGUI namespace section
namespace CEGUI
{
class CEGUIEXPORT AnimationProperty : public AllocatedObject<AnimationProperty>
{
public:
    enum SpecialKeyFrame
    {
        LASTKEYFRAME = -1,
        FIRSTKEYFRAME = -2
    };
    
    String d_nodeName;

    virtual void apply(Model3DInstance& target, float position, Model3D::GlobalInfo& property) const = 0;
    virtual bool isFinished(Model3DInstance& target, float position, bool direction) = 0;
    virtual ~AnimationProperty() = 0;
};

class CEGUIEXPORT NodeProperty : public AnimationProperty, public AllocatedObject<NodeProperty>
{
public:
    struct KeyFramePosition
    {
        float tick = 0;
        Vector3f position;
    };
    struct KeyFrameRotation
    {
        float tick;
        Quaternion rotation;
    };
    struct KeyFrameScaling
    {
        float tick = 0;
        Vector3f scaling;
    };
    
    float d_duration;
    float d_ticksPerSecond;
    
    void addKeyFramePosition(KeyFramePosition key);
    void addKeyFrameRotation(KeyFrameRotation key);
    void addKeyFrameScaling(KeyFrameScaling key);
    
    virtual void apply(Model3DInstance& target, float position, Model3D::GlobalInfo& property) const;
    virtual bool isFinished(Model3DInstance& target, float position, bool direction);
    virtual ~NodeProperty() {}
    
private:
    int FindPosition(float AnimationTick) const;
    void CalcInterpolatedPosition(Vector3f& Out, float AnimationTick) const;
    int FindRotation(float AnimationTick) const;
    void CalcInterpolatedRotation(Quaternion& Out, float AnimationTick) const;
    int FindScaling(float AnimationTick) const;
    void CalcInterpolatedScaling(Vector3f& Out, float AnimationTick) const;
    std::vector<KeyFramePosition>::type d_keyFramePositions;
    std::vector<KeyFrameRotation>::type d_keyFrameRotations;
    std::vector<KeyFrameScaling>::type d_keyFrameScalings;
};
    
class CEGUIEXPORT ColorProperty : public AnimationProperty, public AllocatedObject<NodeProperty>
{
public:
    struct KeyFrame
    {
        int tick;
        Vector4f color;
    };
    
    float d_duration;
    float d_ticksPerSecond;
    
    void addKeyFrame(KeyFrame key);
    
    virtual void apply(Model3DInstance& target, float position, Model3D::GlobalInfo& property) const;
    virtual bool isFinished(Model3DInstance& target, float position, bool direction);
    virtual ~ColorProperty() {}
    
private:
    void CalcNodeColor(Vector4f& Out, float AnimationTick) const;
    std::vector<KeyFrame>::type d_keyFrames;
};
    
class CEGUIEXPORT MoveProperty : public AnimationProperty, public AllocatedObject<NodeProperty>
{
public:
    struct BezierStatus
    {
        Vector3f position;
        Vector3f direction;
    };
    struct BezierProperty
    {
        Vector3f positionStart;
        Vector3f positionControl0;
        Vector3f positionControl1;
        Vector3f positionEnd;
        Vector3f speedControl0 = Vector3f(0.0, 0.0, 0.0);
        Vector3f speedControl1 = Vector3f(0.0, 0.0, 0.0);
    };
    
    float d_duration;
    float d_ticksPerSecond;
    
    void addKeyFramePositionControl(Vector4f key);
    void addKeyFrameSpeedControl(Vector4f key);
    void addBezierProperty(BezierProperty key);
    
    virtual void apply(Model3DInstance& target, float position, Model3D::GlobalInfo& property) const;
    virtual bool isFinished(Model3DInstance& target, float position, bool direction);
    virtual ~MoveProperty() {}
    
private:
    BezierStatus CalcBezier(Vector3f control0, Vector3f control1, Vector3f control2, Vector3f control3, float AnimationTick) const;
    BezierProperty d_bezierProperty;
    std::vector<Vector4f>::type d_keyFramePositionControls;
    std::vector<Vector4f>::type d_keyFrameSpeedControls;
};

class CEGUIEXPORT ViewAngleProperty : public AnimationProperty, public AllocatedObject<NodeProperty>
{
public:
    struct KeyFrameCamera
    {
        Vector3f position;
        Vector3f target;
        Vector3f up;
        float angle;
    };
    
    float d_duration;
    float d_ticksPerSecond;
    
    void addKeyFrame(KeyFrameCamera key);
    
    virtual void apply(Model3DInstance& target, float position, Model3D::GlobalInfo& property) const;
    virtual bool isFinished(Model3DInstance& target, float position, bool direction);
    virtual ~ViewAngleProperty() {}
    
private:
    KeyFrameCamera d_keyFrameCamera;
};

class CEGUIEXPORT AnimationController : public AllocatedObject<AnimationController>
{
public:
    enum ANIM_PRIORITY
    {
        NORMAL = 0,
        HIGH,
        EXTREME_HIGH,
        NUM_PRIORITY
    };
    
    friend class Model3D_xmlHandler;
    
    String animName;
    ANIM_PRIORITY priority;
    bool multiExistForSamePriority;
    std::vector<AnimationProperty*>::type d_animationProperties;
    Model3DAnimation* d_parent;
    
    AnimationController() {
        priority = NORMAL;
        multiExistForSamePriority = true;
    }
    ~AnimationController();
        
    void initializeTarget(Model3DInstance& target);
    
    void step(Model3DInstance& target, float delta);
    
    void stop(Model3DInstance& target);
    
    void getPropertyByPosition(Model3DInstance& target, String& nodeName, float position, Model3D::GlobalInfo& property) const;
    
    bool checkIsFinished(Model3DInstance& target, float position);
    
private:
    
};

class CEGUIEXPORT Model3DAnimation : public AllocatedObject<Model3DAnimation>
{
public:
    enum ANIM_DIRECTION
    {
        BACKWARD = 0,
        FORWARD
    };
    enum ANIM_STATUS
    {
        STOP = 0,
        PAUSE,
        RUN
    };
    
    struct AnimationData
    {
        String animName = "";
        ANIM_DIRECTION direction = FORWARD;
        ANIM_STATUS status = STOP;
        AnimationController::ANIM_PRIORITY priority = AnimationController::NORMAL;
        float position = 0.0;
        bool updated = false;
    };
    
    std::map<String, AnimationController>::type d_animationControllers;
    
    friend class Model3D;
    friend class Model3D_xmlHandler;
    
    Model3DAnimation();
    
    void initializeTarget(Model3DInstance& target);
    
    void startAnimation(Model3DInstance& target, const String& name, ANIM_DIRECTION direction = FORWARD);
    void pauseAnimation(Model3DInstance& target, const String& name);
    void stopAnimation(Model3DInstance& target, const String& name);
    void killAllRunningAnimation(Model3DInstance& target);
    
    bool step(Model3DInstance& target, float delta);
    void onAnimStausChanged(Model3DInstance& target, AnimationController& anim, ANIM_STATUS status);
    
    void getPropertyByPosition(Model3DInstance& target, const String& animName, String& nodeName, float position, Model3D::GlobalInfo& property) const;
    
private:
    
};



}
#endif  // end of guard _MODEL3DANIMATION_h_

