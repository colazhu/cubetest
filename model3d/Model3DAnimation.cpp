#include "CEGUI/Model3DAnimation.h"
#include "CEGUI/Model3DInstance.h"
#include "assimp/postprocess.h"
#include "glm_3d/glm/glm.hpp"
#include "glm_3d/glm/gtc/matrix_transform.hpp"
#include "glm_3d/glm/gtc/type_ptr.hpp"
#include "glm_3d/glm/gtx/matrix_decompose.hpp"
#include <iostream>

//----------------------------------------------------------------------------//
Model3DAnimation::Model3DAnimation()
{

}

//----------------------------------------------------------------------------//
AnimationProperty::~AnimationProperty()
{

}

//----------------------------------------------------------------------------//
void Model3DAnimation::startAnimation(Model3DInstance& target, const String& name, Model3DAnimation::ANIM_DIRECTION direction)
{
    if (target.d_animations.find(name) == target.d_animations.end()) return;
    
    target.d_animations[name].direction = direction;
    bool isNeedStart = true;
    for (auto itr = target.d_animations.begin(); itr != target.d_animations.end(); itr++) {
        // if priority equals incoming priority and cannot multi play, stop the running or paused, the incoming can be started
        if (itr->second.priority == target.d_animations[name].priority
            && !d_animationControllers[name].multiExistForSamePriority) {
            itr->second.status = STOP;
        }
        if (itr->second.status == RUN) {
            // if running priority is lower than incoming priority, pause the running, wait to be started, the incoming can be started
            if (itr->second.priority < target.d_animations[name].priority) {
                itr->second.status = PAUSE;
            }
            // if running priority is higher than incoming priority, pause the incoming, wait to be started
            else if(itr->second.priority > target.d_animations[name].priority) {
                isNeedStart = false;
            }
        }
    }
    if (isNeedStart) target.d_animations[name].status = RUN;
    else target.d_animations[name].status = PAUSE;
}

//----------------------------------------------------------------------------//
void Model3DAnimation::pauseAnimation(Model3DInstance& target, const String& name)
{

}

//----------------------------------------------------------------------------//
void Model3DAnimation::stopAnimation(Model3DInstance& target, const String& name)
{
    if (target.d_animations.find(name) == target.d_animations.end()) return;
    
    d_animationControllers[name].stop(target);
}

//----------------------------------------------------------------------------//
bool Model3DAnimation::step(Model3DInstance& target, float delta)
{
    bool isUpdated = false;
    for (auto itr = target.d_animations.begin(); itr != target.d_animations.end(); itr++) {
        if (itr->second.status == RUN) {
            isUpdated = true;
            d_animationControllers[itr->first].step(target, delta);
        }
        else {
            target.d_animations[itr->first].updated = false;
        }
    }
    return isUpdated;
}

//----------------------------------------------------------------------------//
void Model3DAnimation::initializeTarget(Model3DInstance& target)
{
    for (auto itr = d_animationControllers.begin(); itr != d_animationControllers.end(); itr++) {
        itr->second.initializeTarget(target);
    }
}

//----------------------------------------------------------------------------//
void Model3DAnimation::onAnimStausChanged(Model3DInstance& target, AnimationController& anim, Model3DAnimation::ANIM_STATUS status)
{
    if (status == PAUSE || status == STOP) {
        // try to start the paused animations when an animation stoped
        for (auto itr = target.d_animations.begin(); itr != target.d_animations.end(); itr++) {
            if (itr->second.status == PAUSE && anim.animName != itr->second.animName) {
                startAnimation(target, itr->first, itr->second.direction);
            }
        }
    }
}

//----------------------------------------------------------------------------//
void Model3DAnimation::getPropertyByPosition(Model3DInstance& target, const String& animName, String& nodeName, float position, Model3D::GlobalInfo& property) const
{
    if (d_animationControllers.find(animName) == d_animationControllers.end()) return ;
    d_animationControllers.at(animName).getPropertyByPosition(target, nodeName, position, property);
}

//----------------------------------------------------------------------------//
AnimationController::~AnimationController()
{
    for (int i = 0; i < d_animationProperties.size(); i++) {
        delete d_animationProperties[i];
    }
}

//----------------------------------------------------------------------------//
void AnimationController::initializeTarget(Model3DInstance& target)
{
    Model3DAnimation::AnimationData animation;
    animation.animName = animName;
    animation.priority = priority;
    target.d_animations[animName] = animation;
    target.d_animations[animName].status = Model3DAnimation::STOP;
}

//----------------------------------------------------------------------------//
void AnimationController::step(Model3DInstance& target, float delta)
{
    target.d_animations[animName].updated = true;
    if (target.d_animations[animName].direction == Model3DAnimation::FORWARD) target.d_animations[animName].position += delta;
    else if (target.d_animations[animName].direction == Model3DAnimation::BACKWARD) target.d_animations[animName].position -= delta;

    if(checkIsFinished(target, target.d_animations[animName].position)) {
        target.d_animations[animName].status = Model3DAnimation::STOP;
        d_parent->onAnimStausChanged(target, *this, Model3DAnimation::STOP);
    }
}

//----------------------------------------------------------------------------//
void AnimationController::stop(Model3DInstance& target)
{
    target.d_animations[animName].status = Model3DAnimation::STOP;
    target.d_animations[animName].position = 0;
    d_parent->onAnimStausChanged(target, *this, Model3DAnimation::STOP);
}

//----------------------------------------------------------------------------//
void AnimationController::getPropertyByPosition(Model3DInstance& target, String& nodeName, float position, Model3D::GlobalInfo& property) const
{
    for (int i = 0; i < d_animationProperties.size(); i++) {
        // if node name is empty, update all nodes' property
        if (nodeName == "" || d_animationProperties[i]->d_nodeName == nodeName) {
            d_animationProperties[i]->apply(target, position, property);
        }
    }
}

//----------------------------------------------------------------------------//
bool AnimationController::checkIsFinished(Model3DInstance& target, float position)
{
    for (int i = 0; i < d_animationProperties.size(); i++) {
        if (!d_animationProperties[i]->isFinished(target, position, target.d_animations[animName].direction)) {
            return false;
        }
    }
    return true;
}

//----------------------------------------------------------------------------//
void NodeProperty::addKeyFramePosition(KeyFramePosition key)
{
    d_keyFramePositions.push_back(key);
}

//----------------------------------------------------------------------------//
void NodeProperty::addKeyFrameRotation(KeyFrameRotation key)
{
    d_keyFrameRotations.push_back(key);
}

//----------------------------------------------------------------------------//
void NodeProperty::addKeyFrameScaling(KeyFrameScaling key)
{
    d_keyFrameScalings.push_back(key);
}

//----------------------------------------------------------------------------//
bool NodeProperty::isFinished(Model3DInstance& target, float position, bool direction)
{
    float timeInTicks = position * d_ticksPerSecond;
    if (timeInTicks > d_duration && direction == Model3DAnimation::FORWARD) {
        return true;
    }
    else if (timeInTicks < 0 && direction == Model3DAnimation::BACKWARD) {
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------//
void ColorProperty::addKeyFrame(KeyFrame key)
{
    d_keyFrames.push_back(key);
}

//----------------------------------------------------------------------------//
bool ColorProperty::isFinished(Model3DInstance& target, float position, bool direction)
{
    float timeInTicks = position * d_ticksPerSecond;
    if (timeInTicks > d_duration && direction == Model3DAnimation::FORWARD) {
        return true;
    }
    else if (timeInTicks < 0 && direction == Model3DAnimation::BACKWARD) {
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------//
int NodeProperty::FindPosition(float AnimationTick) const
{
    if (AnimationTick < (float)d_keyFramePositions[0].tick) return FIRSTKEYFRAME;
    for (int i = 0 ; i < d_keyFramePositions.size() - 1 ; i++) {
        if (AnimationTick < (float)d_keyFramePositions[i + 1].tick) {
            return i;
        }
    }
    return LASTKEYFRAME;
}

//----------------------------------------------------------------------------//
int NodeProperty::FindRotation(float AnimationTick) const
{
    if (AnimationTick < (float)d_keyFrameRotations[0].tick) return FIRSTKEYFRAME;
    for (int i = 0 ; i < d_keyFrameRotations.size() - 1 ; i++) {
        if (AnimationTick < (float)d_keyFrameRotations[i + 1].tick) {
            return i;
        }
    }
    return LASTKEYFRAME;
}

//----------------------------------------------------------------------------//
int NodeProperty::FindScaling(float AnimationTick) const
{
    if (AnimationTick < (float)d_keyFrameScalings[0].tick) return FIRSTKEYFRAME;
    for (int i = 0 ; i < d_keyFrameScalings.size() - 1; i++) {
        if (AnimationTick < (float)d_keyFrameScalings[i + 1].tick) {
            return i;
        }
    }
    return LASTKEYFRAME;
}

//----------------------------------------------------------------------------//
void NodeProperty::CalcInterpolatedPosition(Vector3f& Out, float AnimationTick) const
{
    if (d_keyFramePositions.size() == 1) {
        Out = d_keyFramePositions[0].position;
        return;
    }

    int PositionIndex = FindPosition(AnimationTick);
    if (PositionIndex == LASTKEYFRAME) {
        Out = d_keyFramePositions.back().position;
        return ;
    }
    if (PositionIndex == FIRSTKEYFRAME) {
        Out = d_keyFramePositions[0].position;
        return ;
    }
    int NextPositionIndex = PositionIndex + 1;

    float DeltaTime = (float)(d_keyFramePositions[NextPositionIndex].tick - d_keyFramePositions[PositionIndex].tick);
    float Factor = (AnimationTick - (float)d_keyFramePositions[PositionIndex].tick) / DeltaTime;
    const Vector3f& Start = d_keyFramePositions[PositionIndex].position;
    const Vector3f& End = d_keyFramePositions[NextPositionIndex].position;
    Vector3f Delta = End - Start;
    Out = Start + Delta * Factor;
}

//----------------------------------------------------------------------------//
void NodeProperty::CalcInterpolatedRotation(Quaternion& Out, float AnimationTick) const
{
    if (d_keyFrameRotations.size() <= 1) {
        Out = d_keyFrameRotations[0].rotation;
        return ;
    }

    int RotationIndex = FindRotation(AnimationTick);
    if (RotationIndex == LASTKEYFRAME) {
        Out = d_keyFrameRotations.back().rotation;
        return ;
    }
    if (RotationIndex == FIRSTKEYFRAME) {
        Out = d_keyFrameRotations[0].rotation;
        return ;
    }
    int NextRotationIndex = RotationIndex + 1;

    float DeltaTime = (float)(d_keyFrameRotations[NextRotationIndex].tick - d_keyFrameRotations[RotationIndex].tick);
    float Factor = (AnimationTick - (float)d_keyFrameRotations[RotationIndex].tick) / DeltaTime;
    const aiQuaternion& Start = aiQuaternion(d_keyFrameRotations[RotationIndex].rotation.d_w, d_keyFrameRotations[RotationIndex].rotation.d_x, d_keyFrameRotations[RotationIndex].rotation.d_y, d_keyFrameRotations[RotationIndex].rotation.d_z);
    const aiQuaternion& End = aiQuaternion(d_keyFrameRotations[NextRotationIndex].rotation.d_w, d_keyFrameRotations[NextRotationIndex].rotation.d_x, d_keyFrameRotations[NextRotationIndex].rotation.d_y, d_keyFrameRotations[NextRotationIndex].rotation.d_z);
    aiQuaternion aiOut;
    aiQuaternion::Interpolate(aiOut, Start, End, Factor);
    aiOut = aiOut.Normalize();
    Out = Quaternion(aiOut.w, aiOut.x, aiOut.y, aiOut.z);
}

//----------------------------------------------------------------------------//
void NodeProperty::CalcInterpolatedScaling(Vector3f& Out, float AnimationTick) const
{
    if (d_keyFrameScalings.size() == 1) {
        Out = d_keyFrameScalings[0].scaling;
        return;
    }

    int ScalingIndex = FindScaling(AnimationTick);
    if (ScalingIndex == LASTKEYFRAME) {
        Out = d_keyFrameScalings.back().scaling;
        return ;
    }
    if (ScalingIndex == FIRSTKEYFRAME) {
        Out = d_keyFrameScalings[0].scaling;
        return ;
    }
    int NextScalingIndex = ScalingIndex + 1;

    float DeltaTime = (float)(d_keyFrameScalings[NextScalingIndex].tick - d_keyFrameScalings[ScalingIndex].tick);
    float Factor = (AnimationTick - (float)d_keyFrameScalings[ScalingIndex].tick) / DeltaTime;
    const Vector3f& Start = d_keyFrameScalings[ScalingIndex].scaling;
    const Vector3f& End = d_keyFrameScalings[NextScalingIndex].scaling;
    Vector3f Delta = End - Start;
    Out = Start + Delta * Factor;
}

//----------------------------------------------------------------------------//
void NodeProperty::apply(Model3DInstance& target, float position, Model3D::GlobalInfo& property) const
{
    float positionInTicks = position * d_ticksPerSecond;
    if (d_keyFrameScalings.size() == 0 || d_keyFramePositions.size() == 0 || d_keyFrameRotations.size() == 0) {
        return ;
    }

    Vector3f Scaling;
    CalcInterpolatedScaling(Scaling, positionInTicks);
    property.scaling.d_x = property.scaling.d_x * Scaling.d_x;
    property.scaling.d_y = property.scaling.d_y * Scaling.d_y;
    property.scaling.d_z = property.scaling.d_z * Scaling.d_z;

    Quaternion Rotation;
    CalcInterpolatedRotation(Rotation, positionInTicks);
    property.rotation = Rotation * property.rotation;

    Vector3f Translation;
    CalcInterpolatedPosition(Translation, positionInTicks);
    property.position = property.position + Translation;
}

//----------------------------------------------------------------------------//
void ColorProperty::CalcNodeColor(Vector4f& Out, float AnimationTick) const
{
    if (d_keyFrames.size() == 0) {
        Out = Vector4f(0, 0, 0, 0);
        return ;
    }
    if (AnimationTick <= d_keyFrames.front().tick) {
        Out = d_keyFrames.front().color;
        return ;
    }
    else if (AnimationTick >= d_keyFrames.back().tick) {
        Out = d_keyFrames.back().color;
        return ;
    }
    
    auto it = d_keyFrames.begin();
    for (; it != d_keyFrames.end(); ++it) {
        if (AnimationTick == it->tick) {
            Out = it->color;
            return ;
        }
        else if (AnimationTick < it->tick) break;
    }

    Out = (it-1)->color + (it->color - (it-1)->color) * float((AnimationTick - (it-1)->tick) / (it->tick - (it-1)->tick));
}

//----------------------------------------------------------------------------//
void ColorProperty::apply(Model3DInstance& target, float position, Model3D::GlobalInfo& property) const
{
    float positionInTicks = position * d_ticksPerSecond;
    Vector4f Color;
    CalcNodeColor(Color, positionInTicks);
    property.color = property.color + Color;
}

//----------------------------------------------------------------------------//
void MoveProperty::addKeyFramePositionControl(Vector4f key)
{
    d_keyFramePositionControls.push_back(key);
}

//----------------------------------------------------------------------------//
void MoveProperty::addKeyFrameSpeedControl(Vector4f key)
{
    d_keyFrameSpeedControls.push_back(key);
}

//----------------------------------------------------------------------------//
void MoveProperty::addBezierProperty(BezierProperty key)
{
    d_bezierProperty = key;
}

//----------------------------------------------------------------------------//
bool MoveProperty::isFinished(Model3DInstance& target, float position, bool direction)
{
    float timeInTicks = position * d_ticksPerSecond;
    if (timeInTicks > d_duration && direction == Model3DAnimation::FORWARD) {
        return true;
    }
    else if (timeInTicks < 0 && direction == Model3DAnimation::BACKWARD) {
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------//
MoveProperty::BezierStatus MoveProperty::CalcBezier(Vector3f control0, Vector3f control1, Vector3f control2, Vector3f control3,float AnimationTick) const
{
    glm::vec3 controlA = glm::vec3(control0.d_x, control0.d_y, control0.d_z);
    glm::vec3 controlB = glm::vec3(control1.d_x, control1.d_y, control1.d_z);
    glm::vec3 controlC = glm::vec3(control2.d_x, control2.d_y, control2.d_z);
    glm::vec3 controlD = glm::vec3(control3.d_x, control3.d_y, control3.d_z);
    
    float t = AnimationTick / d_duration;
    glm::vec3 P0 = (1-t)*controlA + t*controlB;
    glm::vec3 P1 = (1-t)*controlB + t*controlC;
    glm::vec3 P2 = (1-t)*controlC + t*controlD;
    glm::vec3 B0 = (1-t)*P0 + t*P1;
    glm::vec3 B1 = (1-t)*P1 + t*P2;
    glm::vec3 B = (1-t)*B0 + t*B1;
    glm::vec3 D = B1 - B0;
    D = glm::normalize(D);
    
    MoveProperty::BezierStatus out;
    out.position = Vector3f(B.x, B.y, B.z);
    out.direction = Vector3f(D.x, D.y, D.z);
    return out;
}

//----------------------------------------------------------------------------//
void MoveProperty::apply(Model3DInstance& target, float position, Model3D::GlobalInfo& property) const
{
    float positionInTicks = position * d_ticksPerSecond;
    //calculate the speed curve
    MoveProperty::BezierStatus speedBezier = CalcBezier(Vector3f(0.0, 0.0, 0.0), d_bezierProperty.speedControl0, d_bezierProperty.speedControl1, Vector3f(d_duration, d_duration, 0.0f), positionInTicks);
    float bezierTime = speedBezier.position.d_y;
    //calculate the move curve
    MoveProperty::BezierStatus positionBezier = CalcBezier(d_bezierProperty.positionStart, d_bezierProperty.positionControl0, d_bezierProperty.positionControl1, d_bezierProperty.positionEnd, bezierTime);
    
    property.position = positionBezier.position;
    glm::vec3 dir(positionBezier.direction.d_x, positionBezier.direction.d_y, positionBezier.direction.d_z);
    float angle = glm::acos(glm::dot(dir, glm::vec3(0.0, 0.0, 1.0)));
    Quaternion qua;
    qua = qua.axisAngleRadians(Vector3f(0.0, 1.0, 0.0), -angle);
    property.rotation = qua;
}

//----------------------------------------------------------------------------//
bool ViewAngleProperty::isFinished(Model3DInstance &target, float position, bool direction)
{
    Vector3f oldPos = target.getCamera().position;
    Vector3f oldTarget = target.getCamera().target;
    
    Vector3f moveDis = d_keyFrameCamera.position - oldPos;
    float angleDis = d_keyFrameCamera.angle - target.getCamera().angle;
    if (moveDis == Vector3f(0, 0, 0) && angleDis == 0) return true;
    return false;
}

//----------------------------------------------------------------------------//
void ViewAngleProperty::addKeyFrame(KeyFrameCamera key)
{
    d_keyFrameCamera = key;
}

//----------------------------------------------------------------------------//
void ViewAngleProperty::apply(Model3DInstance& target, float position, Model3D::GlobalInfo& property) const
{
    Vector3f oldPos = target.getCamera().position;
    Vector3f oldTarget = target.getCamera().target;
    Vector3f oldUp = target.getCamera().up;
    
    Vector3f moveDis = d_keyFrameCamera.position - oldPos;
    int angleDis = int(glm::degrees(d_keyFrameCamera.angle - target.getCamera().angle)) % 360;
    if (angleDis > 180) angleDis -= 360;

    float MAX_DISTANCE = 5;
    float dis_rate_move = (MAX_DISTANCE+50) / (abs(moveDis.d_x) + abs(moveDis.d_y) + abs(moveDis.d_z));
    float dis_rate_rotate = MAX_DISTANCE / abs(angleDis);
    float dis_rate = std::min(dis_rate_move, dis_rate_rotate);

    if (dis_rate < 1) {
        property.camera.position = oldPos + moveDis * dis_rate;
        property.camera.target = oldTarget + (d_keyFrameCamera.target - oldTarget) * dis_rate;
        property.camera.up = oldUp + (d_keyFrameCamera.up - oldUp) * dis_rate;
        property.camera.angle = target.getCamera().angle + glm::radians(angleDis * dis_rate);
    }
    else {
        property.camera.position = d_keyFrameCamera.position;
        property.camera.target = d_keyFrameCamera.target;
        property.camera.up = d_keyFrameCamera.up;
        property.camera.angle = d_keyFrameCamera.angle;
    }
}

