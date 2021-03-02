#include "TextureCache.h"
#include "Log.h"
#include "ProgramCache.h"
#include "Program.h"
#include "Director.h"
// #include "CEGUI/ImageManager.h"
// #include "CEGUI/BasicImage.h"
// #include "Texture.h"

Texture::Texture(GLuint txtid, int w, int h) :
    ogltxtid(txtid),
    width(w),
    height(h),
    ratio(1.0f)
{
    if (width > 0 && height > 0) {
        ratio = (float)width/(float)height;
    }
}

void Texture::bind(bool updateUniform) {
    if (-1 == ogltxtid) {
        return;
    }

    if (updateUniform) {
        if (Program* program = Director::instance()->programCache().getCurProgram()) {
            if (Uniform* uniform = program->getUniform("u_TextureRatio")) {
                // LOG_BASE("Texture update loc:%d ratio:%.3f",uniform->location, ratio);
                program->setUniformLocationWith1f(uniform->location, ratio);
            }
        }
    }

    GLHook::glActiveTexture(GL_TEXTURE0);
    // LOG_BASE("Texture bind:%d", ogltxtid);
    GLHook::glBindTexture(GL_TEXTURE_2D, ogltxtid);
}

TextureCache::TextureCache() {
}

TextureCache::~TextureCache() {
    for (std::map<std::string, Texture*>::iterator it = m_textures.begin(); it != m_textures.end();
            ++it) {
        SAFE_DELETE(it->second);
    }
    m_textures.clear();
}

std::string TextureCache::id2name(int txtid)
{
    char name[32] = {0};
    snprintf(name, 32, "CubeImage%03d", txtid);
    return std::string(name);
}

Texture* TextureCache::addImage(int id, void* buffer, int w, int h)
{
    return addImage(id2name(id), buffer, w, h);
}

Texture* TextureCache::addImage(const std::string &keyname, void* buffer, int w, int h)
{
    if (!buffer || w <=0 || h <=0 || m_textures.count(keyname)) {
        return NULL;
    }

    
    GLuint old_tex;
    GLHook::glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&old_tex));

    GLuint ogltxtid;
    GLHook::glGenTextures(1, &ogltxtid);   
    GLHook::glBindTexture(GL_TEXTURE_2D, ogltxtid);   
    GLHook::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLHook::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    GLHook::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    GLHook::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);   
    
    GLHook::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     static_cast<GLsizei>(w),
                     static_cast<GLsizei>(h),
                     0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    GLint old_pack;
    GLHook::glGetIntegerv(GL_UNPACK_ALIGNMENT, &old_pack);

    GLHook::glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLHook::glTexSubImage2D(GL_TEXTURE_2D, 0,
                    static_cast<GLint>(0),
                    static_cast<GLint>(0),
                    static_cast<GLsizei>(w),
                    static_cast<GLsizei>(h),
                    GL_RGBA, GL_UNSIGNED_BYTE, buffer);

    GLHook::glPixelStorei(GL_UNPACK_ALIGNMENT, old_pack);

    GLHook::glBindTexture(GL_TEXTURE_2D, old_tex);

    LOG_BASE("addImage:%s buff:%p w:%d h:%d txt:%d", keyname.c_str(), buffer, w, h, ogltxtid);
    // GLHook::glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    return addImage(keyname, ogltxtid, w, h);
}

Texture* TextureCache::addImage(const std::string &keyname, const std::string &path) {
    if (keyname.empty() || m_textures.count(keyname)) {
        return NULL;
    }

//    CEGUI::OpenGLTexture& ogltxt = static_cast<CEGUI::OpenGLTexture&>(CEGUI::ImageManager::getSingleton().addFromTextureFile(keyname.c_str(), path.c_str()));
//    Texture* txt = new Texture(ogltxt.getOpenGLTexture(), ogltxt.getSize().d_width, ogltxt.getSize().d_height);
//    m_textures[keyname] = txt;
//    return txt;
    return NULL;
}

Texture* TextureCache::addImage(int id, int gltxtid, int w, int h) {
    return addImage(id2name(id), gltxtid, w, h);
}

Texture* TextureCache::addImage(const std::string &keyname, int gltxtid, int w, int h) {
    if (keyname.empty() || m_textures.count(keyname)) {
        return NULL;
    }
     
    Texture* txt = new Texture(gltxtid, w, h);
    m_textures[keyname] = txt;
    LOG_BASE("addImage:%s %d w:%d h:%d", keyname.c_str(), gltxtid, w, h);
    return txt;
}

void TextureCache::removeAllTextures() {
    for (TextureMap::iterator it = m_textures.begin(); it != m_textures.end(); ++it) {
        SAFE_DELETE(it->second);
    }
    m_textures.clear();
}

void TextureCache::removeTexture(Texture* texture) {
    if (!texture) {
        return;
    }

    for (TextureMap::iterator it = m_textures.begin(); it != m_textures.end(); /* nothing */) {
        if (it->second == texture) {
            SAFE_DELETE(it->second);
            it = m_textures.erase(it);
            break;
        }
        else {
            ++it;
        }
    }
}

void TextureCache::removeTexture(const std::string &textureKeyName) {
    std::string key = textureKeyName;
    TextureMap::iterator it = m_textures.find(key);

    if (it != m_textures.end()) {
        SAFE_DELETE(it->second);
        m_textures.erase(it);
    }
}

Texture* TextureCache::getTexture(const std::string &textureKeyName) {
    std::string key = textureKeyName;
    TextureMap::iterator it = m_textures.find(key);

    if (it != m_textures.end()) {
        // LOG_BASE("getTexture:%s %p %d", key.c_str(), it->second, it->second->ogltxtid);
        return it->second;
    }
    return NULL;
}

Texture* TextureCache::getTexture(int id) {
    return getTexture(id2name(id));
}
