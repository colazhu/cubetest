#ifndef _Model3DRenderer_h_
#define _Model3DRenderer_h_

#include "CEGUI/String.h"
#include "CEGUI/Vector.h"
#include "CEGUI/Renderer.h"
#include "MEM_map.h"
#include "CEGUI/Model3DInstance.h"
#include "CEGUI/Rect.h"

// Start of CEGUI namespace section
namespace CEGUI
{

class CEGUIEXPORT Model3DRenderer : public AllocatedObject<Model3DRenderer>
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
        
        renderInfo render;
    };
    
    struct PixelInfo {
        float PrimID;
        float ObjectID;
        float DrawID;
//        float Reserve;
        PixelInfo()     {
            ObjectID = 0;
            DrawID = 0;
            PrimID = 0;
//            Reserve = 0;
        }
    };
    Model3DRenderer(Model3DInstance& model3DInstance, Renderer& owner);
    ~Model3DRenderer();

    void initialize();
    bool update(const double& delta);
    void setTextureSize(const int& width, const int& height);
    Texture& getTexture();
    String getTargetFromPosition(UVector2 position);
    void setupRenderInfoes();
    void free();

    void move(const Vector2f& offset);
    void click(const Vector2f& pos);
    PixelInfo ReadPixel(unsigned int x, unsigned int y);
private:
    unsigned int planeTex;
    float intense;
    bool isInputed;
    bool isUpdated;
    Rectf m_area;
    void createFramebuffer();
    void createPlaneObject();
    void generatePickingTexture(const int& width, const int& height);
//    Texture& createTexture(OpenGLRendererBase& owner, const String& name, GLuint tex, const Sizef& sz);
    void generateFramebuffer(const int& width, const int& height);
    void generatePostProcess(const int& width, const int& height);
    void drawingPhase();
    void drawingPlanePhase();
    void drawingMesh(const unsigned int& id);
    void pickingMesh(const unsigned int& id);
    bool initPicking(const int& width, const int& height);
    void EnableWritingPicking();
    void pickingPhase();
    Model3DInstance& m_model3DInstance;
    renderInfo m_planeInfo;
    std::map<unsigned int, renderInfo>::type m_renderInfos;
    std::map<unsigned int, renderInfo>::type m_pickrenderInfos;
    framebufferInfo m_frame;
    framebufferInfo m_pick;
    framebufferInfo m_process;
    Renderer& m_renderOwner;
    Vector2f m_clickedPos;
    Mat4x4f m_projection;
//    OpenGL3Shader m_screenShader;
};

} // End of  CEGUI namespace section

#endif /* _Model3DRenderer_h_ */
