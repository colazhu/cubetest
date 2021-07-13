#include "CEGUI/Model3DAnimation.h"
#include "CEGUI/Model3D_xmlHandler.h"
#include "CEGUI/Logger.h"
#include "CEGUI/XMLAttributes.h"
#include "CEGUI/Model3DManager.h"
#include "CEGUI/Vector.h"
#include "CEGUI/PropertyHelper.h"

ColorProperty* colorProperty;
MoveProperty* moveProperty;
ViewAngleProperty* sceneProperty;
NodeProperty* modelProperty;
//----------------------------------------------------------------------------//
const String Model3D_xmlHandler::Model3DElement("Model3D");
const String Model3D_xmlHandler::NameElement("name");
const String Model3D_xmlHandler::FileNameElement("filename");
const String Model3D_xmlHandler::TriggerNodeElement("triggerNode");
const String Model3D_xmlHandler::AnimTargetElement("animTarget");
const String Model3D_xmlHandler::positionElement("position");
const String Model3D_xmlHandler::diffuseElement("diffuse");
const String Model3D_xmlHandler::ambientElement("ambient");
const String Model3D_xmlHandler::specularElement("specular");
const String Model3D_xmlHandler::nameElement("name");
const String Model3D_xmlHandler::emissiveElement("emissive");
const String Model3D_xmlHandler::shininessElement("shininess");
const String Model3D_xmlHandler::strengthElement("strength");
const String Model3D_xmlHandler::transparentElement("transparent");
const String Model3D_xmlHandler::upElement("up");
const String Model3D_xmlHandler::lookatElement("lookat");
const String Model3D_xmlHandler::nearElement("near");
const String Model3D_xmlHandler::farElement("far");
const String Model3D_xmlHandler::LightElement("Light");
const String Model3D_xmlHandler::MaterialElement("Material");
const String Model3D_xmlHandler::CameraElement("Camera");
const String Model3D_xmlHandler::angleElement("angle");
const String Model3D_xmlHandler::typeElement("type");
const String Model3D_xmlHandler::startElement("start");
const String Model3D_xmlHandler::endElement("end");
const String Model3D_xmlHandler::cameraElement("camera");
const String Model3D_xmlHandler::travelElement("travel");
const String Model3D_xmlHandler::travel1Element("travel1");
const String Model3D_xmlHandler::durationElement("duration");
const String Model3D_xmlHandler::ticksPerSecondElement("ticksPerSecond");
const String Model3D_xmlHandler::tickElement("tick");
const String Model3D_xmlHandler::colorElement("color");
const String Model3D_xmlHandler::ChannelElement("Channel");
const String Model3D_xmlHandler::KeyframeElement("Keyframe");
const String Model3D_xmlHandler::SceneElement("Scene");
const String Model3D_xmlHandler::speedControlElement("speedControl");
const String Model3D_xmlHandler::speedControl1Element("speedControl1");
const String Model3D_xmlHandler::ModelAnimationType("ModelAnimation");
const String Model3D_xmlHandler::MoveAnimationType("MoveAnimation");
const String Model3D_xmlHandler::ColorAnimationType("ColorAnimation");
const String Model3D_xmlHandler::SceneAnimationType("SceneAnimation");
const String Model3D_xmlHandler::AnimationElement("Animation");
const String Model3D_xmlHandler::BezierElement("Bezier");
const String Model3D_xmlHandler::priorityElement("priority");
const String Model3D_xmlHandler::multiExistElement("multiExist");
    
Model3D_xmlHandler::Model3D_xmlHandler() :
    d_model(NULL)
{}

//----------------------------------------------------------------------------//
Model3D_xmlHandler::~Model3D_xmlHandler()
{
}

//----------------------------------------------------------------------------//
void Model3D_xmlHandler::elementStart(const String& element,
                                         const XMLAttributes& attributes)
{
    if (element == Model3DElement) {
        elementModel3DStart(attributes);
    }
    else if (element == LightElement) {
        elementLightStart(attributes);
    }
    else if (element == MaterialElement) {
        elementMaterialStart(attributes);
    }
    else if (element == CameraElement) {
        elementCameraStart(attributes);
    }
    else if (element == ChannelElement) {
        elementChannelStart(attributes);
    }
    else if (element == KeyframeElement) {
        elementKeyframeStart(attributes);
    }
    else if (element == SceneElement) {
        elementSceneStart(attributes);
    }
    else if (element == AnimationElement) {
        elementAnimationStart(attributes);
    }
    else if (element == BezierElement) {
        elementBezierStart(attributes);
    }
    else {
        Logger::getSingleton().logEvent("Model3D_xmlHandler::elementStart: "
                                        "Unknown element encountered: <" + element + ">", Errors);
    } 
}
    
//----------------------------------------------------------------------------//
void Model3D_xmlHandler::elementEnd(const String& element)
{
    if (element == Model3DElement) {
        elementModel3DEnd();
    }  
    else if (element == LightElement) {
        elementLightEnd();
    }
    else if (element == MaterialElement) {
        elementMaterialEnd();
    }
    else if (element == CameraElement) {
        elementCameraEnd();
    }
    else if (element == ChannelElement) {
        elementChannelEnd();
    }
    else if (element == KeyframeElement) {
        elementKeyframeEnd();
    }
    else if (element == SceneElement) {
        elementSceneEnd();
    }
    else if (element == AnimationElement) {
        elementAnimationEnd();
    }
    else if (element == BezierElement) {
        elementBezierEnd();
    }
}

//----------------------------------------------------------------------------//
void Model3D_xmlHandler::elementModel3DStart(const XMLAttributes& attributes)
{
    if (d_model) CEGUI_DELETE_AO d_model;

    String name = attributes.getValueAsString(NameElement);
    String fileName = attributes.getValueAsString(FileNameElement);
    
    String resourceGroup = d_resoureGroup == ""? Model3DManager::getSingleton().getDefaultResourceGroup() : d_resoureGroup;
    String finalFileName = static_cast<DefaultResourceProvider*>(System::getSingleton().getResourceProvider())->getFinalFilename(fileName, resourceGroup);
    d_model = CEGUI_NEW_AO Model3D(name, finalFileName);
}

//----------------------------------------------------------------------------//
void Model3D_xmlHandler::elementLightStart(const XMLAttributes& attributes)
{
    Light* light = new Light();
    light->name = attributes.getValueAsString(nameElement);
    light->position = PropertyHelper<Vector3f>::fromString(attributes.getValueAsString(positionElement));
    light->diffuse = PropertyHelper<Vector3f>::fromString(attributes.getValueAsString(diffuseElement));
    light->ambient = PropertyHelper<Vector3f>::fromString(attributes.getValueAsString(ambientElement));
    light->specular = PropertyHelper<Vector3f>::fromString(attributes.getValueAsString(specularElement));

    d_model->m_lights.push_back(light);
}

//----------------------------------------------------------------------------//
void Model3D_xmlHandler::elementMaterialStart(const XMLAttributes& attributes)
{
    Material* material = new Material();
    material->name = attributes.getValueAsString(nameElement);
    material->diffuse.color = PropertyHelper<Vector4f>::fromString(attributes.getValueAsString(diffuseElement));
    material->specular.color = PropertyHelper<Vector4f>::fromString(attributes.getValueAsString(specularElement));
    material->ambient.color = PropertyHelper<Vector4f>::fromString(attributes.getValueAsString(ambientElement));
    material->emissive.color = PropertyHelper<Vector4f>::fromString(attributes.getValueAsString(emissiveElement));
    material->shininess = attributes.getValueAsFloat(shininessElement);
    material->strength = attributes.getValueAsFloat(strengthElement);
    material->transparent = attributes.getValueAsFloat(transparentElement);

    for (int i = 0; i < d_model->m_materials.size(); i++) {
        if (d_model->m_materials[i]->name == material->name) {
            d_model->m_materials[i] = material;
            return ;
        }
    }
    d_model->m_materials.push_back(material);
}

//----------------------------------------------------------------------------//
void Model3D_xmlHandler::elementCameraStart(const XMLAttributes& attributes)
{
    Camera* camera = new Camera();
    camera->position = PropertyHelper<Vector3f>::fromString(attributes.getValueAsString(positionElement));
    camera->up = PropertyHelper<Vector3f>::fromString(attributes.getValueAsString(upElement));
    camera->target = PropertyHelper<Vector3f>::fromString(attributes.getValueAsString(lookatElement));
   
   d_model->m_camera.m_cameras.push_back(camera);
}

//----------------------------------------------------------------------------//
void Model3D_xmlHandler::elementKeyframeStart(const XMLAttributes& attributes)
{
    float tick = attributes.getValueAsFloat(tickElement);
    Vector4f color = PropertyHelper<Vector4f>::fromString(attributes.getValueAsString(colorElement));

    ColorProperty::KeyFrame keyframe;
    keyframe.tick = tick;
    keyframe.color = color;
    colorProperty->d_nodeName = d_animation.channel;
    colorProperty->addKeyFrame(keyframe);
}

//----------------------------------------------------------------------------//
void Model3D_xmlHandler::elementChannelStart(const XMLAttributes& attributes)
{
    String name = attributes.getValueAsString(nameElement);
    d_animation.channel = name;
    if (name != "") d_model->m_nodes.at(name).m_anims.push_back(d_animation.name);

    if (d_animation.type == ModelAnimationType) {
        modelProperty = new NodeProperty();
    }
    else if (d_animation.type == ColorAnimationType) {
        colorProperty = new ColorProperty();
    }
    else if (d_animation.type == MoveAnimationType) {
        moveProperty = new MoveProperty();
    }
    else if (d_animation.type == SceneAnimationType) {
        sceneProperty = new ViewAngleProperty();
    }
}

//----------------------------------------------------------------------------//
void Model3D_xmlHandler::elementSceneStart(const XMLAttributes& attributes)
{
    d_model->m_camera.m_anims.push_back(d_animation.name);
    
    ViewAngleProperty::KeyFrameCamera keyframe;
    keyframe.position = PropertyHelper<Vector3f>::fromString(attributes.getValueAsString(positionElement));
    keyframe.target = PropertyHelper<Vector3f>::fromString(attributes.getValueAsString(lookatElement));
    keyframe.up = PropertyHelper<Vector3f>::fromString(attributes.getValueAsString(upElement));
    keyframe.angle = attributes.getValueAsFloat(angleElement);
    
    sceneProperty->d_nodeName = d_animation.channel;
    sceneProperty->addKeyFrame(keyframe);
}

//----------------------------------------------------------------------------//
void Model3D_xmlHandler::elementAnimationStart(const XMLAttributes& attributes)
{
    d_animation.name = attributes.getValueAsString(nameElement);
    d_animation.type = attributes.getValueAsString(typeElement);
    d_animation.duration = PropertyHelper<float>::fromString(attributes.getValueAsString(durationElement));
    d_animation.ticksPerSecond = PropertyHelper<float>::fromString(attributes.getValueAsString(ticksPerSecondElement));
    d_animation.priority = attributes.getValueAsInteger(priorityElement);
    d_animation.multiExist = attributes.getValueAsBool(multiExistElement);
}

//----------------------------------------------------------------------------//
void Model3D_xmlHandler::elementBezierStart(const XMLAttributes& attributes)
{
    MoveProperty::BezierProperty bezierProperty;
    bezierProperty.positionStart = PropertyHelper<Vector3f>::fromString(attributes.getValueAsString(startElement));
    bezierProperty.positionEnd = PropertyHelper<Vector3f>::fromString(attributes.getValueAsString(endElement));
    bezierProperty.positionControl0 = PropertyHelper<Vector3f>::fromString(attributes.getValueAsString(travelElement));
    bezierProperty.positionControl1 = PropertyHelper<Vector3f>::fromString(attributes.getValueAsString(travel1Element));
    bezierProperty.speedControl0 = PropertyHelper<Vector3f>::fromString(attributes.getValueAsString(speedControlElement));
    bezierProperty.speedControl1 = PropertyHelper<Vector3f>::fromString(attributes.getValueAsString(speedControl1Element));

    moveProperty->d_nodeName = d_animation.channel;
    moveProperty->addBezierProperty(bezierProperty);
}
    
//----------------------------------------------------------------------------//
void Model3D_xmlHandler::elementModel3DEnd()
{

}

//----------------------------------------------------------------------------//
void Model3D_xmlHandler::elementLightEnd()
{

}

//----------------------------------------------------------------------------//
void Model3D_xmlHandler::elementMaterialEnd()
{

}

//----------------------------------------------------------------------------//
void Model3D_xmlHandler::elementCameraEnd()
{

}

//----------------------------------------------------------------------------//
void Model3D_xmlHandler::elementChannelEnd()
{
    AnimationController& control = d_model->d_model3DAnimation->d_animationControllers[d_animation.name];
    control.animName = d_animation.name;
    control.priority = (AnimationController::ANIM_PRIORITY)d_animation.priority;
    control.multiExistForSamePriority = d_animation.multiExist;
    control.d_parent = d_model->d_model3DAnimation;

    if (d_animation.type == ModelAnimationType) {
        // if the type is ModelAnimationType, only do the NodeProperty updating
        for (int i=0; i < control.d_animationProperties.size(); i++) {
            if (d_animation.channel == "" || d_animation.channel == control.d_animationProperties[i]->d_nodeName) {
                static_cast<NodeProperty*>(control.d_animationProperties[i])->d_duration = d_animation.duration;
                static_cast<NodeProperty*>(control.d_animationProperties[i])->d_ticksPerSecond = d_animation.ticksPerSecond;
            }
        }
    }
    else if (d_animation.type == ColorAnimationType) {
        colorProperty->d_duration = d_animation.duration;
        colorProperty->d_ticksPerSecond = d_animation.ticksPerSecond;
        control.d_animationProperties.push_back(colorProperty);
        colorProperty = NULL;
    }
    else if (d_animation.type == MoveAnimationType) {
        moveProperty->d_duration = d_animation.duration;
        moveProperty->d_ticksPerSecond = d_animation.ticksPerSecond;
        control.d_animationProperties.push_back(moveProperty);
        moveProperty = NULL;
    }
    else if (d_animation.type == SceneAnimationType) {
        sceneProperty->d_duration = d_animation.duration;
        sceneProperty->d_ticksPerSecond = d_animation.ticksPerSecond;
        control.d_animationProperties.push_back(sceneProperty);
        sceneProperty = NULL;
    }
    
    d_animation.channel = "";
}

//----------------------------------------------------------------------------//
void Model3D_xmlHandler::elementKeyframeEnd()
{

}

//----------------------------------------------------------------------------//
void Model3D_xmlHandler::elementSceneEnd()
{
    
}
    
//----------------------------------------------------------------------------//
void Model3D_xmlHandler::elementAnimationEnd()
{
    d_animation = Animation();
}

//----------------------------------------------------------------------------//
void Model3D_xmlHandler::elementBezierEnd()
{
        
}

//----------------------------------------------------------------------------//
const String& Model3D_xmlHandler::getDefaultResourceGroup() const
{
    return Model3DManager::getDefaultResourceGroup();
}

//----------------------------------------------------------------------------//
const String& Model3D_xmlHandler::getSchemaName() const
{
    static String sTmpStr("");
    return sTmpStr;
}
