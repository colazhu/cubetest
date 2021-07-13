#ifndef _CEGUIModel3D_xmlHandler_h_
#define _CEGUIModel3D_xmlHandler_h_

#include "CEGUI/XMLHandler.h"
#include "CEGUI/String.h"
#include "CEGUI/Model3D.h"

// Start of CEGUI namespace section
namespace CEGUI
{
class Model3D;
class Node;
    
class CEGUIEXPORT Model3D_xmlHandler : public XMLHandler
{
public:
    //! Element specifying a language.
    static const String Model3DElement;
    //! Element specifying a Font.
    static const String NameElement;
    //! Element specifying a languagelist.
    static const String FileNameElement;
    // Element specifying a offsetProperty.
    static const String AnimationElement;
    // Element specifying a fontSizeRange.
    static const String TriggerNodeElement;
    // Element specifying a fontSize.
    static const String AnimTargetElement;
    // Element specifying a Light.
    static const String LightElement;
    // Element specifying a Material.
    static const String MaterialElement;
    // Element specifying a Camera.
    static const String CameraElement;
    // Element specifying a position.
    static const String positionElement;
    // Element specifying a diffuse.
    static const String diffuseElement;
    // Element specifying a ambient.
    static const String ambientElement;
    // Element specifying a specular.
    static const String specularElement;
    // Element specifying a name.
    static const String nameElement;
    // Element specifying a emissive.
    static const String emissiveElement;
    // Element specifying a shininess.
    static const String shininessElement;
    // Element specifying a strength.
    static const String strengthElement;
    // Element specifying a transparent.
    static const String transparentElement;
    // Element specifying a up.
    static const String upElement;
    // Element specifying a lookat.
    static const String lookatElement;
    // Element specifying a near.
    static const String nearElement;
    // Element specifying a far.
    static const String farElement;
    // Element specifying a angle.
    static const String angleElement;
    // Element specifying a type.
    static const String typeElement;
    // Element specifying a start.
    static const String startElement;
    // Element specifying a end.
    static const String endElement;
    // Element specifying a camera.
    static const String cameraElement;
    // Element specifying a travel.
    static const String travelElement;
    // Element specifying a travel1.
    static const String travel1Element;
    // Element specifying a duration.
    static const String durationElement;
    // Element specifying a ticksPerSecond.
    static const String ticksPerSecondElement;
    // ELement specifying a tick.
    static const String tickElement;
    // ELement specifying a color.
    static const String colorElement;
    // ELement specifying a Channel.
    static const String ChannelElement;
    // ELement specifying a Keyframe.
    static const String KeyframeElement;
    // ELement specifying a Scene.
    static const String SceneElement;
    // Element specifying a speedControl.
    static const String speedControlElement;
    // Element specifying a speedControl1.
    static const String speedControl1Element;
    // Element specifying a Bezier.
    static const String BezierElement;
    
    static const String priorityElement;
    
    static const String multiExistElement;
    
    static const String ModelAnimationType;
    static const String MoveAnimationType;
    static const String ColorAnimationType;
    static const String SceneAnimationType;

    
    //! Constructor.
    Model3D_xmlHandler();
    
    //! Destructor.
    ~Model3D_xmlHandler();
    
    virtual void handleFile(const String& fileName, const String& resourceGroup) {
        d_resoureGroup = resourceGroup;
        XMLHandler::handleFile(fileName, resourceGroup);
    }
    
    void setModel(Model3D* model) { d_model = model; }

    void elementStart(const String& element, const XMLAttributes& attributes);
    void elementEnd(const String& element);

    //XMLHandler overrides
    const String& getSchemaName() const;
    const String& getDefaultResourceGroup() const;

    Model3D* d_model;

private:
    void elementModel3DStart(const XMLAttributes& attributes);
    void elementLightStart(const XMLAttributes& attributes);
    void elementMaterialStart(const XMLAttributes& attributes);
    void elementCameraStart(const XMLAttributes& attributes);
    void elementKeyframeStart(const XMLAttributes& attributes);
    void elementChannelStart(const XMLAttributes& attributes);
    void elementSceneStart(const XMLAttributes& attributes);
    void elementAnimationStart(const XMLAttributes& attributes);
    void elementBezierStart(const XMLAttributes& attributes);
    void elementModel3DEnd();
    void elementLightEnd();
    void elementMaterialEnd();    
    void elementCameraEnd();
    void elementKeyframeEnd();
    void elementChannelEnd();
    void elementSceneEnd();
    void elementAnimationEnd();
    void elementBezierEnd();

    struct Animation
    {
        String name;
        String type;
        float duration = -1.0;
        float ticksPerSecond = -1.0;
        String channel = "";
        int priority = 0;
        bool multiExist = true;
    };
    
    Animation d_animation;
    
    String d_resoureGroup;
};
    
} // End of  CEGUI namespace section


#endif
