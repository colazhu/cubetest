#include "Light.h"
#include "Node.h"

Light* Light::createDirectionalLight(const Vector3& direction, Node* attachedNode)
{
    Light * light = new Light(DIRECTIONAL);
    if (light) {
        light->setDirection(direction);
        light->computeDistanceAttenuation(false);
        light->setAttachedNode(attachedNode);
        return light;
    }
    return NULL;
}

Light* Light::createPointLight(const Vector3& position, Node* attachedNode)
{
    Light * light = new Light(POINT);
    if (light) {
        light->setPosition(position);
        light->setCutoffAngle(360.0f); // > 180.0
        light->setAttachedNode(attachedNode);
        return light;
    }
    return NULL;
}

Light* Light::createSpotLight(const Vector3& position,
                            const Vector3& dir,
                            float cutoff_degree,
                            float spot_exponent,
                            Node* attachedNode)
{
    Light * light = new Light(SPOT);
    if (light) {
        light->setPosition(position);
        light->setDirection(dir);
        light->setCutoffAngle(cutoff_degree);
        light->setSpotExponent(spot_exponent);
        light->setAttachedNode(attachedNode);
        return light;
    }
    return NULL;
}

Light::Light(LightType type)
        :m_type(type), m_enable(true), m_ratio(1.0f), m_nodeAttached(NULL)
{
    if (type == DIRECTIONAL) {
        m_lightData.compute_distance_attenuation = false;
        m_lightData.position = Vector4::ZERO;
    }
}

LightType Light::type() const
{
    return m_type;
}

void Light::setEnable(bool enable)
{
    if (m_enable == enable) {
        return;
    }
    m_enable = enable;
    notifyLightChanged();
}

bool Light::isEnable() const
{
    return m_enable;
}

/**
 * using attached node model view matrix
 */
void Light::setAttachedNode(Node* node)
{
    if (node == m_nodeAttached) {
        return;
    }
    m_nodeAttached = node;
    notifyLightChanged();
}

void Light::setColor(Color4F color)
{
    Vector4 vColor = Vector4(color.r, color.g, color.b, color.a);
    setColor(vColor);
}

void Light::setColor(unsigned int color)
{
    setColor(Vector4::fromColor(color));
}

void Light::setColor(Vector4 vColor)
{
    if (m_lightData.specular_color == vColor && m_lightData.diffuse_color == vColor) {
        return;
    }

    m_lightData.specular_color = vColor;
    m_lightData.diffuse_color = vColor;
    notifyLightChanged();
}

void Light::setIntensity(float intensity)
{
    // ("[intensity:%lf]",intensity);
    float ratio = 1.0f;
    if (m_type == POINT || m_type == SPOT) {
        ratio = MATH_Bound<float>(0.0f, intensity, 10.0f) / 10.0f;
    }
    if (ratio == m_ratio) {
        return;
    }
    m_ratio = ratio;
    notifyLightChanged();
}

void Light::setRange(float range)
{
    // UNUSED (range)
}

void Light::setAmbientColor(unsigned int color)
{
    m_lightData.ambient_color = Vector4::fromColor(color);
}

void Light::setAmbientColor(float r, float g, float b, float alpha)
{
    m_lightData.ambient_color = Vector4(r, g, b, alpha);
}

void Light::setDiffuseColor(unsigned int color)
{
    m_lightData.diffuse_color = Vector4::fromColor(color);
}

void Light::setDiffuseColor(float r, float g, float b, float alpha)
{
    m_lightData.diffuse_color = Vector4(r, g, b, alpha);
}

void Light::setSpecularColor(unsigned int color)
{
    m_lightData.specular_color = Vector4::fromColor(color);
}

void Light::setSpecularColor(float r, float g, float b, float alpha)
{
    m_lightData.specular_color = Vector4(r, g, b, alpha);
}

/**
 */
void Light::computeDistanceAttenuation(bool enable)
{
    m_lightData.compute_distance_attenuation = enable;
}

bool Light::isComputeDistanceAttenuation() const
{
    return m_lightData.compute_distance_attenuation;
}

/**
 * f(d) = 1/(a0+a1*d+a2*d*d)
 * f: light intensity
 * d: distance from object to light
 */
void Light::setAttenuationFactors(const Vector3& attenuation_factors)
{
    m_lightData.attenuation_factors = attenuation_factors;
}

void Light::setAttenuationFactors(float a0, float a1, float a2)
{
    m_lightData.attenuation_factors = Vector3(a0, a1, a2);
}

/**
 */
void Light::setPosition(const Vector3& position)
{
    m_lightData.position = Vector4(position);
}

Vector3 Light::position() const
{
    return m_lightData.position;
}

/**
 * set light direction for directional light
 */
void Light::setDirection(const Vector3& direction)
{
    m_lightData.direction = direction;
    m_lightData.direction.normalize();
}

Vector3 Light::direction() const
{
    return m_lightData.direction;
}

/**
 * set spot light cutoff angle in degree
 */
void Light::setCutoffAngle(float angle)
{
    m_lightData.spot_cutoff_angle = angle;
}

/**
 */
void Light::setSpotExponent(float exponent)
{
    m_lightData.spot_exponent = exponent;
}

float Light::spotExponent() const
{
    return m_lightData.spot_exponent;
}

/*
 * get light data for computing light in shader
 */
LightData Light::getLightData() const
{
    LightData data = m_lightData;
    data.ambient_color *= m_ratio;
    data.diffuse_color *= m_ratio;
    data.specular_color *= m_ratio;
    if (m_nodeAttached) {
        Matrix matrix = m_nodeAttached->getNodeToWorldTransform();
        Vector3 newPos;
        matrix.transformPoint(Vector3(data.position), &newPos);
        data.position = newPos;
        // LOG_BASE("get Light Position:%.1f, %.1f, %.1f", data.position.x, data.position.y, data.position.z);
        matrix.transformVector(&data.direction);
        data.direction.normalize();
    }
    return data;
}

Vector4 Light::doLighting(const MaterialData& material, const Vector4& position, const Vector3& normal, const Vector3& eye)
{
    LightData lightData = getLightData();
    Vector4 computed_color = Vector4::ZERO;

    Vector3 h_vec;
    float ndotl, ndoth;
    float att_factor;
    att_factor = 1.0;

    Vector3 VPpli;
    if (lightData.position.w > 0.1) {  // not DIRECTIONAL light 0.0
       float spot_factor;
       Vector3 att_dist;
       // this is a point or spot light
       // we assume "w" values for PPli and V are the same
       Vector4 tmp = lightData.position - position;
       VPpli = Vector3(tmp);
       if (lightData.compute_distance_attenuation) {
           // compute distance attenuation
           att_dist.x = 1.0;
           att_dist.z = Vector3::dot(VPpli, VPpli);
           att_dist.y = sqrt(att_dist.z);
           att_factor = 1.0 / Vector3::dot(att_dist, lightData.attenuation_factors);
       }
       VPpli.normalize();

       if (lightData.spot_cutoff_angle < 180.0) {
           // compute spot factor
           spot_factor = Vector3::dot(-VPpli, lightData.direction);
           if (spot_factor >= cosf(MATH_DEG_TO_RAD(lightData.spot_cutoff_angle)))
               att_factor *= powf(spot_factor, lightData.spot_exponent);
           else
               att_factor = 0.0;		//spot_factor = 0;

           att_factor *= spot_factor;
       }
    }
    else {
       // directional light
       VPpli = -lightData.direction;
    }

    if (att_factor > 0.0) {
       // process lighting equation --> compute the light color
       Vector4 ambient_color = Color4F::multiply(lightData.ambient_color, material.ambient_color);
       computed_color += ambient_color;

       ndotl = MATH_MAX(0.0f, Vector3::dot(normal, VPpli));

       Vector4 diffuse_color = Color4F::multiply(lightData.diffuse_color, material.diffuse_color);
       computed_color += (ndotl * diffuse_color);
       h_vec = VPpli + eye;
       h_vec.normalize();
       ndoth = Vector3::dot(normal, h_vec);
       if (ndoth > 0) {
           Vector4 specular_color = Color4F::multiply(lightData.specular_color, material.specular_color);
           computed_color += (powf(ndoth, material.specular_exponent) * specular_color);
       }
       computed_color *= att_factor;  // multiply color with
       // computed attenuation factor
       // * computed spot factor
    }

     return computed_color;
}
