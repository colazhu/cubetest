#include "CEGUI/Model3DInstance.h"
#include "CEGUI/Quaternion.h"
#include "glm_3d/glm/glm.hpp"
#include "glm_3d/glm/gtc/matrix_transform.hpp"
#include "glm_3d/glm/gtc/type_ptr.hpp"
#include "glm_3d/glm/gtx/matrix_decompose.hpp"
#include "assimp/postprocess.h"
#include "CEGUI/InputEvent.h"
#include "CEGUI/GlobalEventSet.h"
#include <iostream>
// Start of CEGUI namespace section

//----------------------------------------------------------------------------//
Model3DInstance::Model3DInstance(Model3D& model3d, Model3DRenderer& renderer) : m_model3D(model3d), m_model3DRenderer(renderer), m_forceUpdate(false)
{
}

//----------------------------------------------------------------------------//
void Model3DInstance::initialise()
{
    m_model3D.initialiseTarget(*this);
    m_model3DRenderer.initialize(m_model3D);
}

//----------------------------------------------------------------------------//
bool Model3DInstance::isAnimationRunning()
{
    return true;
}

//----------------------------------------------------------------------------//
void Model3DInstance::startAnimation(const String& animName, bool isForward)
{
    if(d_animations.find(animName) == d_animations.end()) return ;
    
    m_model3D.startAnimation(*this, animName, isForward);
}
    
//----------------------------------------------------------------------------//
void Model3DInstance::stopAnimation(const String& animName)
{
    if(d_animations.find(animName) == d_animations.end()) return ;
    
    m_model3D.stopAnimation(*this, animName);
}
    
//----------------------------------------------------------------------------//
void Model3DInstance::pauseAnimation(const String& animName)
{
    if(d_animations.find(animName) == d_animations.end()) return ;
    
    m_model3D.pauseAnimation(*this, animName);
}
    
//----------------------------------------------------------------------------//
void Model3DInstance::resumeAnimation(const String& animName)
{
}
    
//----------------------------------------------------------------------------//
void Model3DInstance::killAllAnimation()
{
    m_model3D.initialiseTarget(*this);
}

//----------------------------------------------------------------------------//
void Model3DInstance::setPosition(String animName, float position)
{
    if(d_animations.find(animName) == d_animations.end()) return ;
    
    d_animations[animName].position = position;
}

//----------------------------------------------------------------------------//
void Model3DInstance::moveModel(const Vector2f& offset)
{
    m_camera.angle += offset.d_x * 0.005;
    m_forceUpdate = true;
}

//----------------------------------------------------------------------------//
void Model3DInstance::clickModel(const Vector2f& pos)
{
}
    
//----------------------------------------------------------------------------//
Model3D& Model3DInstance::getModel3D()
{
    return m_model3D;
}

//----------------------------------------------------------------------------//
bool Model3DInstance::step(float deltatime)
{
    return m_model3D.step(*this, deltatime) || m_forceUpdate;
}

//----------------------------------------------------------------------------//
void Model3DInstance::render()
{
    m_model3D.render(*this);
    
    // clean animation update flag
    for (auto it = d_animations.begin(); it != d_animations.end(); ++it) {
        it->second.updated = false;
    }
    m_forceUpdate = false;
    for (auto it = m_nodeInfo.begin(); it != m_nodeInfo.end(); ++it) {
        it->second.isBoneUpdated = false;
    }
}

