#include "LightCache.h"
#include "Program.h"
#include "GLFunction.h"
// #include "ColorPalette.h"

#define STRINGIFY(A)  #A
#include "Lighting.vert"

const Vector4 GLOBAL_AMBIENT_COLOR_DEFAULT = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
//const Vector4 GLOBAL_AMBIENT_COLOR_DEFAULT = Vector4(.5f, .5f, .5f, 1.0f);
//const Vector4 GLOBAL_AMBIENT_COLOR_DEFAULT = Vector4(180.0f/255.0f, 187.0f/255.0f, 1.0f, 1.0f);
//const Vector4 GLOBAL_AMBIENT_COLOR_DEFAULT = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

const Vector4 GLOBAL_FOG_COLOR_DEFAULT = Vector4(0.2f, 0.2f, 0.2f, 1.0f);
const Vector3 GLOBAL_FOG_DIST_DEFAULT = Vector3(50.0f, 1.0f, 1.0f);

const char* UNIFORM_NAME_GLOBALAMBIENT = "u_globalAmbient";
//
const char* UNIFORM_NAME_MATERIAL_EMSSION = "u_material_emssion";
const char* UNIFORM_NAME_MATERIAL_AMBIEN = "u_material_ambient";
const char* UNIFORM_NAME_MATERIAL_DIFFUSE = "u_material_diffuse";
const char* UNIFORM_NAME_MATERIAL_SPECULAR = "u_material_specular";
const char* UNIFORM_NAME_MATERIAL_SPECULAR_EXPONENT = "u_material_specular_exponent";
//
const char* UNIFORM_NAME_LIGHT_ENABLE = "u_light_enable";
const char* UNIFORM_NAME_LIGHT_POSITION = "u_light_position";
const char* UNIFORM_NAME_LIGHT_AMBIENT = "u_light_ambient";
const char* UNIFORM_NAME_LIGHT_DIFFUSE = "u_light_diffuse";
const char* UNIFORM_NAME_LIGHT_SPECULAR = "u_light_specular";
const char* UNIFORM_NAME_LIGHT_DIRECTION = "u_light_direction";
const char* UNIFORM_NAME_LIGHT_ATTENUATION_FACTOR = "u_light_attenuation_factors";
const char* UNIFORM_NAME_LIGHT_SPOT_EXPONENT = "u_light_spot_exponent";
const char* UNIFORM_NAME_LIGHT_SPOT_CUTOFF_ANGLE = "u_light_spot_cutoff_angle";
const char* UNIFORM_NAME_LIGHT_COMPUTE_DISTANCE_ATTENUATION = "u_light_compute_distance_attenuation";

//
const char* UNIFORM_NAME_FOG_COLOR = "u_FogColor";
const char* UNIFORM_NAME_FOG_DIST = "u_FogDist";

LightCache::LightCache() {
    init();
}

LightCache::~LightCache() {
    for (LightList::iterator it = m_lights.begin(); it != m_lights.end(); ++it) {
        if (Light* light = it->second) {
            SAFE_DELETE(light);
        }
    }
    m_lights.clear();
}

bool LightCache::init() {
    m_globalAmbientColor = GLOBAL_AMBIENT_COLOR_DEFAULT;
    m_flogColor = GLOBAL_FOG_COLOR_DEFAULT;
    m_flogDist = GLOBAL_FOG_DIST_DEFAULT;
    useFog(false);

    return true;
}

Light* LightCache::getLight(int tag) {
    LightList::iterator it = m_lights.find(tag);
    if (it != m_lights.end()) {
        return it->second;
    }
    return NULL;
}

bool LightCache::addLight(int tag, Light* light, bool force) {
	if(force) {
		removeLight(tag);
	}
    
    if (m_lights.size() >= LIGHT_ID_TOTAL) {
        return false;
    }

    m_lights.insert(std::make_pair(tag, light));
    return true;
}

void LightCache::removeLight(int tag) {
    LightList::iterator it = m_lights.find(tag);
    if (it != m_lights.end()) {
		if (Light* light = it->second) {
            SAFE_DELETE(light);
		}
        m_lights.erase(it);
	}
}

bool LightCache::enableLight(int tag, bool enable) {
    LightList::iterator it = m_lights.find(tag);
    if (it != m_lights.end()) {
        if (Light* light = it->second) {
            light->setEnable(enable);
            return true;
        }
    }
    return false;
}

Vector4 LightCache::doLighting(const MaterialData& material,
                               const Vector4& position,
                               const Vector3& normal,
                               const Vector3& eye) {


    Vector4 vtx_color = Vector4::ZERO;

    //add self emssion color
    vtx_color += material.emssion_color;

    //add global ambient color
    Vector4 globalAmbientColor = Color4F::multiply(m_globalAmbientColor, material.ambient_color);
    vtx_color += globalAmbientColor;

    for (LightList::iterator it = m_lights.begin(); it != m_lights.end(); ++it) {
        if (Light* light = it->second) {
            if (light->isEnable()) {
                vtx_color += light->doLighting(material, position, normal, eye);
            }
        }
    }

    vtx_color.w = material.diffuse_color.w;  //alpha
    return vtx_color;
}

bool LightCache::useLighting(GLuint program) {
    if ((-1 != GLHook::glGetUniformLocation(program, UNIFORM_NAME_GLOBALAMBIENT))
            && (-1 != GLHook::glGetUniformLocation(program, UNIFORM_NAME_MATERIAL_EMSSION))
            && (-1 != GLHook::glGetUniformLocation(program, UNIFORM_NAME_MATERIAL_AMBIEN))
            && (-1 != GLHook::glGetUniformLocation(program, UNIFORM_NAME_MATERIAL_DIFFUSE))
            && (-1 != GLHook::glGetUniformLocation(program, UNIFORM_NAME_MATERIAL_SPECULAR))
            && (-1 != GLHook::glGetUniformLocation(program, UNIFORM_NAME_MATERIAL_SPECULAR_EXPONENT))) {
        return true;
    }
    else {
        return false;
    }
    return false;
}

void LightCache::setFog(const Vector3& fogColor, float distMax, float distMin) {
    m_flogColor = fogColor;
    m_flogDist = Vector3(MATH_MAX(abs(distMax), abs(distMin)), MATH_MIN(abs(distMax), abs(distMin)), 1.0f);
}

void LightCache::useFog(bool use) {
    if (use) {
        m_flogDist.z = 1.0;
    }
    else {
        m_flogDist.z = 0.0;
    }
}

void LightCache::updateUniform(Program* p) {
    //global ambient
    if (Uniform* uniform = p->getUniform(UNIFORM_NAME_GLOBALAMBIENT)) {
        p->setUniformLocationWith4fv(uniform->location, &(m_globalAmbientColor.x), 1);
    }

    //material
    if (Uniform* uniform = p->getUniform(UNIFORM_NAME_MATERIAL_EMSSION)) {
        p->setUniformLocationWith4fv(uniform->location, &(m_material.emssion_color.x), 1);
    }

    if (Uniform* uniform = p->getUniform(UNIFORM_NAME_MATERIAL_AMBIEN)) {
        p->setUniformLocationWith4fv(uniform->location, &(m_material.ambient_color.x), 1);
    }

    if (Uniform* uniform = p->getUniform(UNIFORM_NAME_MATERIAL_DIFFUSE)) {
        p->setUniformLocationWith4fv(uniform->location, &(m_material.diffuse_color.x), 1);
    }

    if (Uniform* uniform = p->getUniform(UNIFORM_NAME_MATERIAL_SPECULAR)) {
        p->setUniformLocationWith4fv(uniform->location, &(m_material.specular_color.x), 1);
    }

    if (Uniform* uniform = p->getUniform(UNIFORM_NAME_MATERIAL_SPECULAR_EXPONENT)) {
        p->setUniformLocationWith1f(uniform->location, m_material.specular_exponent);
    }

    //light
    struct {
        int enable[LIGHT_ID_TOTAL];
        Vector4 position[LIGHT_ID_TOTAL];
        Vector4 ambient_color[LIGHT_ID_TOTAL];
        Vector4 diffuse_color[LIGHT_ID_TOTAL];
        Vector4 specular_color[LIGHT_ID_TOTAL];
        Vector3 direction[LIGHT_ID_TOTAL];
        Vector3 attenuation_factors[LIGHT_ID_TOTAL];
        float spot_exponent[LIGHT_ID_TOTAL];
        float spot_cutoff_angle[LIGHT_ID_TOTAL];
        int compute_distance_attenuation[LIGHT_ID_TOTAL];
    } lightData;
    memset(&lightData, 0x00, sizeof(lightData));

    int count = 0;
    for (LightList::iterator it = m_lights.begin();
            count < LIGHT_ID_TOTAL && it != m_lights.end(); ++it) {
        if (Light* light = it->second) {
            LightData lightInfo = light->getLightData();
            lightData.enable[count] = light->isEnable() ? 1 : 0;
            lightData.position[count] = lightInfo.position;
            lightData.ambient_color[count] = lightInfo.ambient_color;
            lightData.diffuse_color[count] = lightInfo.diffuse_color;
            lightData.specular_color[count] = lightInfo.specular_color;
            lightData.direction[count] = lightInfo.direction;
            lightData.attenuation_factors[count] = lightInfo.attenuation_factors;
            lightData.spot_exponent[count] = lightInfo.spot_exponent;
            lightData.spot_cutoff_angle[count] = lightInfo.spot_cutoff_angle;
            lightData.compute_distance_attenuation[count] = lightInfo.compute_distance_attenuation ? 1 : 0;
            ++count;
        }
    }

    if (Uniform* uniform = p->getUniform(UNIFORM_NAME_LIGHT_ENABLE)) {
        p->setUniformLocationWith1iv(uniform->location, lightData.enable, LIGHT_ID_TOTAL);
    }

    if (Uniform* uniform = p->getUniform(UNIFORM_NAME_LIGHT_POSITION)) {
        p->setUniformLocationWith4fv(uniform->location,
                                     &lightData.position[0].x,
                                     LIGHT_ID_TOTAL);
    }

    if (Uniform* uniform = p->getUniform(UNIFORM_NAME_LIGHT_AMBIENT)) {
        p->setUniformLocationWith4fv(uniform->location,
                                     &lightData.ambient_color[0].x,
                                     LIGHT_ID_TOTAL);
    }

    if (Uniform* uniform = p->getUniform(UNIFORM_NAME_LIGHT_DIFFUSE)) {
        p->setUniformLocationWith4fv(uniform->location,
                                     &lightData.diffuse_color[0].x,
                                     LIGHT_ID_TOTAL);
    }

    if (Uniform* uniform = p->getUniform(UNIFORM_NAME_LIGHT_SPECULAR)) {
        p->setUniformLocationWith4fv(uniform->location,
                                     &lightData.specular_color[0].x,
                                     LIGHT_ID_TOTAL);
    }

    if (Uniform* uniform = p->getUniform(UNIFORM_NAME_LIGHT_DIRECTION)) {
        p->setUniformLocationWith3fv(uniform->location,
                                     &lightData.direction[0].x,
                                     LIGHT_ID_TOTAL);
    }

    if (Uniform* uniform = p->getUniform(UNIFORM_NAME_LIGHT_ATTENUATION_FACTOR)) {
        p->setUniformLocationWith3fv(uniform->location,
                                     &lightData.attenuation_factors[0].x,
                                     LIGHT_ID_TOTAL);
    }

    if (Uniform* uniform = p->getUniform(UNIFORM_NAME_LIGHT_SPOT_EXPONENT)) {
        p->setUniformLocationWith1fv(uniform->location,
                                     lightData.spot_exponent,
                                     LIGHT_ID_TOTAL);
    }

    if (Uniform* uniform = p->getUniform(UNIFORM_NAME_LIGHT_SPOT_CUTOFF_ANGLE)) {
        p->setUniformLocationWith1fv(uniform->location,
                                     lightData.spot_cutoff_angle,
                                     LIGHT_ID_TOTAL);
    }

    if (Uniform* uniform = p->getUniform(UNIFORM_NAME_LIGHT_COMPUTE_DISTANCE_ATTENUATION)) {
        p->setUniformLocationWith1iv(uniform->location,
                                     lightData.compute_distance_attenuation,
                                     LIGHT_ID_TOTAL);
    }

    if (Uniform* uniform = p->getUniform(UNIFORM_NAME_FOG_COLOR)) {
        p->setUniformLocationWith4fv(uniform->location,
                                     &m_flogColor.x,
                                     1);
    }

    if (Uniform* uniform = p->getUniform(UNIFORM_NAME_FOG_DIST)) {
        p->setUniformLocationWith3fv(uniform->location,
                                     &m_flogDist.x,
                                     1);
    }
}
