#ifndef TEXTURECACHE_H
#define TEXTURECACHE_H

#include "Base.h"
#include "DataTypes.h"

struct Texture
{
    Texture(GLuint txtid = -1, int w = 0, int h = 0);

    void bind(bool updateUniform = false);

    GLuint ogltxtid;
    int width;
    int height;
    float ratio;
};

class TextureCache {
public:
    TextureCache();
    virtual ~TextureCache();

    Texture* addImage(const std::string &keyname, const std::string &path);
    Texture* addImage(int id, void* buffer, int w, int h);
    Texture* addImage(const std::string &keyname, void* buffer, int w, int h);
    Texture* addImage(const std::string &keyname, int txtid, int w, int h);
    Texture* addImage(int id, int gltxtid, int w, int h);
    std::string id2name(int id);
    Texture* getTexture(const std::string& keyname);
    Texture* getTexture(int id);
    void removeAllTextures();
    void removeTexture(Texture* texture);
    void removeTexture(const std::string &keyname);

    GLuint createTexture(int w, int h);
protected:
    typedef std::map<std::string, Texture*> TextureMap;
    TextureMap m_textures;
};
#endif
