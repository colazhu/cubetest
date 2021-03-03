#ifndef NODEDEF_H
#define NODEDEF_H

#include "Base.h"
#include "MathInc.h"
#include "DataTypes.h"


typedef struct _material_properties
{
    Vector4 ambient_color;
    Vector4 emssion_color;
    Vector4 diffuse_color;
    Vector4 specular_color;
    float specular_exponent;

    _material_properties()
            : ambient_color(1.0, 1.0, 1.0, 1.0),
              emssion_color(),
              diffuse_color(),
              specular_color(),
              specular_exponent(0.5)
    {
    }

    inline void setColor(Color4F color) {
        Vector4 vColor = Vector4(color.r, color.g, color.b, color.a);
        specular_color = vColor;
        diffuse_color = vColor;
        ambient_color = vColor;
        specular_exponent = .5f;
    }

    // 0xRRGGBBAA
    inline void setColor(unsigned int color)
    {
        Vector4 vColor = Vector4::fromColor(color);
        specular_color = vColor;
        diffuse_color = vColor;
        ambient_color = vColor;
        specular_exponent = .5f;
    }

    inline void setAmbient(Color4F color) {
        Vector4 vColor = Vector4(color.r, color.g, color.b, color.a);
        ambient_color = vColor;
    }

    inline void setAmbient(unsigned int color)
    {
        ambient_color = Vector4::fromColor(color);
    }

    inline void setEmssion(Color4F color) {
        Vector4 vColor = Vector4(color.r, color.g, color.b, color.a);
        emssion_color = vColor;
    }

    inline void setEmssion(unsigned int color)
    {
        emssion_color = Vector4::fromColor(color);
    }

    inline void setDiffuse(Color4F color) {
        Vector4 vColor = Vector4(color.r, color.g, color.b, color.a);
        diffuse_color = vColor;
    }

    inline void setDiffuse(unsigned int color)
    {
        diffuse_color = Vector4::fromColor(color);
    }

    inline void setSpecular(Color4F color, float exponent = 0.5) {
        Vector4 vColor = Vector4(color.r, color.g, color.b, color.a);
        specular_color = vColor;
        specular_exponent = exponent;
    }

    inline void setSpecular(unsigned int color, float exponent = 0.5)
    {
        specular_color = Vector4::fromColor(color);
        specular_exponent = exponent;
    }

} MaterialData;

typedef struct _light_properties
{
    Vector4 position;  // light position for a point/spot light or normalized dir. for a directional light
    Vector4 ambient_color;
    Vector4 diffuse_color;
    Vector4 specular_color;
    Vector3 direction;
    Vector3 attenuation_factors;
    float spot_exponent;
    float spot_cutoff_angle;
    bool compute_distance_attenuation;
    _light_properties()
            : position(),
              ambient_color(),
              diffuse_color(),
              specular_color(),
              direction(),
              attenuation_factors(0.1, 0.01, 0.01),
              spot_exponent(0.1),
              spot_cutoff_angle(180.0),
              compute_distance_attenuation(true) //DIRECTIONAL LIGHT : false
    {
    }

} LightData;

struct TransformState
{
    Vector4 vEye;
    Matrix matrixP;
    Matrix matrixMV;
    Matrix matrixMVInverse;
    Matrix matrixMVP;
};

struct LightingState
{
    enum {
        MAX_LIGHT_COUNT = 3
    };
    // material
    Vector4 materialEmssionColor;
    Vector4 materialAmbientColor;
    Vector4 materialDiffuseColor;
    Vector4 materialSpecularColor;
    float materialSpecularExponent;
    // global light
    Vector4 globalAmbientColor;

    // single light
    int enable[MAX_LIGHT_COUNT];
    Vector4 position[MAX_LIGHT_COUNT];
    Vector4 ambient_color[MAX_LIGHT_COUNT];
    Vector4 diffuse_color[MAX_LIGHT_COUNT];
    Vector4 specular_color[MAX_LIGHT_COUNT];
    Vector3 direction[MAX_LIGHT_COUNT];
    Vector3 attenuation_factors[MAX_LIGHT_COUNT];
    float spot_exponent[MAX_LIGHT_COUNT];
    float spot_cutoff_angle[MAX_LIGHT_COUNT];
    int compute_distance_attenuation[MAX_LIGHT_COUNT];

};

struct Event
{
    int eventtype;
};

enum TouchAction {
    TouchAction_Down,
    TouchAction_Up,
    TouchAction_Move,
    TouchAction_Cancel
};

struct TouchEvent : public Event
{
    int id;
    int action;
    int x;
    int y;
};

struct GestureEvent : public Event
{
    int gtype;
    int gclass;
    int state;
};


#endif // NODEDEF_H
