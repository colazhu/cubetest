#ifndef LIGHTCACHE_H
#define LIGHTCACHE_H

#include "Base.h"
#include "Light.h"
#include "DataTypes.h"

class Program;
class Light;

enum LIGHT_ID{
    LIGHT_ID_00 = 0,
    LIGHT_ID_01,
    LIGHT_ID_02,
	LIGHT_ID_TOTAL,
    //
};

/*
 * lighting is a global resource
 */
class LightCache {
	friend class Program;
public:
    LightCache();
    virtual ~LightCache();

    /*
     */
    bool addLight(int tag, Light* light,bool force = false);

	/*
     */
	void removeLight(int tag);
    
	/*
     */
    Light* getLight(int tag);

    /*
     */
    bool enableLight(int tag, bool enable);

    //
    void setMaterial(const MaterialData& material) {
        m_material = material;
    }

    //set a global ambient color
    void setGlobalAmbientColor(const Vector4& scenceColor) {
        m_globalAmbientColor = scenceColor;
    }

	void setGlobalAmbientColor(const Color4F& scenceColor) {
        m_globalAmbientColor.set(&scenceColor.r);
	}

	void setGlobalAmbientColor(const Color4B& scenceColor) {
		setGlobalAmbientColor(Color4F(scenceColor));
	}

    //
    Vector4 doLighting(const MaterialData& material, const Vector4& position, const Vector3& normal, const Vector3& eye);

    //
    bool useLighting(GLuint program);

    //
    void setFog(const Vector3& fogColor, float distMax, float distMin = 0.0f);

    //
    void useFog(bool use);

private:
    bool init();

    MaterialData m_material;
    Vector4 m_globalAmbientColor;

    typedef std::map<int, Light*> LightList;

    LightList m_lights;

    Vector3 m_flogDist;
    Vector4 m_flogColor;
    
    //
    void updateUniform(Program* p);

    DISABLE_COPY(LightCache)
};
#endif /* LIGHTCACHE_H */
