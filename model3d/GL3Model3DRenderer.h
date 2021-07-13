#ifndef _GL3Model3DRenderer_h_
#define _GL3Model3DRenderer_h_

#include "CEGUI/String.h"
#include "CEGUI/Vector.h"
#include "CEGUI/Renderer.h"
#include "MEM_map.h"
#include "CEGUI/Model3DInstance.h"
#include "CEGUI/Model3DRenderer.h"
#include "CEGUI/Rect.h"

// Start of CEGUI namespace section
namespace CEGUI
{

class CEGUIEXPORT OpenGL3Model3DRenderer : public Model3DRenderer
{
public:
    struct renderInfo
    {
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        unsigned int numIndices;
    };
    struct framebufferInfo
    {
        unsigned int FBO;
        unsigned int RBO;
        unsigned int TEX;
        unsigned int DBO;
        unsigned int FBOforRBO;
        
        renderInfo render;
    };
    struct PixelInfo {
        float PrimID;
        float ObjectID;
        float DrawID;
        PixelInfo() {
            ObjectID = 0;
            DrawID = 0;
            PrimID = 0;
        }
    };
    
    OpenGL3Model3DRenderer(Renderer& owner);
    virtual ~OpenGL3Model3DRenderer();

    virtual void initialize(Model3D& target);
    
    virtual void setTextureSize(int width, int height);
    virtual Texture& getTexture();
    
    virtual void beginRendering(Model3DInstance& target);
    virtual void render(Model3DInstance& target, int meshid);
    virtual void endRendering(Model3DInstance& target);

private:
    void generateFramebuffer(int width, int height);
    
    
    // render info for every mesh
    std::map<unsigned int, renderInfo>::type m_renderInfos;
    framebufferInfo m_frame;
    // drawer's owner;
    Renderer& m_renderOwner;
    // projection
    Mat4x4f m_projection;
    // show area
    Rectf m_area;
    
};

} // End of  CEGUI namespace section

#endif /* _GL3Model3DRenderer_h_ */
