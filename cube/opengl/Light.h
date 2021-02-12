#ifndef LIGHT_H
#define LIGHT_H

#include "NodeDef.h"
// #include "Object.h"


class Node;
enum LightType
{
    DIRECTIONAL = 0,
    POINT,
    SPOT,
};

/**
 * @brief The Light class
 * save the data to create light effect in shader program
 */
class Light {

public:
    static Light* createDirectionalLight(const Vector3& direction, Node* attachedNode = NULL);

    static Light* createPointLight(const Vector3& position, Node* attachedNode = NULL);

    static Light* createSpotLight(  const Vector3& position,
                                    const Vector3& dir,
                                    float cutoff_degree = 30.0f,
                                    float spot_exponent = 0.1f,
                                    Node* attachedNode = NULL);


    LightType type() const;

    void setEnable(bool enable);

    bool isEnable() const;

    /**
     * using attached node model view matrix
     */
    void setAttachedNode(Node* node);

    void setColor(Color4F color);
    void setColor(Vector4 color);
    void setColor(unsigned int color);

    void setIntensity(float intensity);

    void setRange(float range);

    void setAmbientColor(unsigned int color);

    void setAmbientColor(float r, float g, float b, float alpha = 1.0);

    void setDiffuseColor(unsigned int color);

    void setDiffuseColor(float r, float g, float b, float alpha = 1.0);

    void setSpecularColor(unsigned int color);

    void setSpecularColor(float r, float g, float b, float alpha = 1.0);

    void computeDistanceAttenuation(bool enable);

    bool isComputeDistanceAttenuation() const;

    /**
     * f(d) = 1/(a0+a1*d+a2*d*d)
     * f: light intensity
     * d: distance from object to light
     */
    void setAttenuationFactors(const Vector3& attenuation_factors);

    void setAttenuationFactors(float a0, float a1, float a2);

    /**
     */
    void setPosition(const Vector3& position);

    Vector3 position() const;

    /**
     * set light direction for directional light
     */
    void setDirection(const Vector3& direction);

    Vector3 direction() const;

    /**
     * set spot light cutoff angle in degree
     */
    void setCutoffAngle(float angle);

    /**
     */
    void setSpotExponent(float exponent);

    float spotExponent() const;

    /*
     * get light data for computing light in shader
     */
    LightData getLightData() const;

    void notifyLightChanged() {/* NGPOST(lightChanged); */}
    //NGSIGNAL() lightChanged;

    Vector4 doLighting(const MaterialData& material, const Vector4& position, const Vector3& normal, const Vector3& eye);
protected:
    Light(LightType type);

    LightType m_type;
    bool m_enable;
    LightData m_lightData;
    float m_ratio;
    Node* m_nodeAttached;
    DISABLE_COPY(Light)
};



#endif // LIGHT_H
