#ifndef _Model3DRenderer_h_
#define _Model3DRenderer_h_

#include "CEGUI/String.h"
#include "CEGUI/Model3DInstance.h"

// Start of CEGUI namespace section
namespace CEGUI
{

class CEGUIEXPORT Model3DRenderer : public AllocatedObject<Model3DRenderer>
{
public:
    Model3DRenderer() {}
    virtual ~Model3DRenderer() {}
    
    virtual void initialize(Model3D& target) = 0;

    virtual void beginRendering(Model3DInstance& target) = 0;
    virtual void render(Model3DInstance& target, int meshid) = 0;
    virtual void endRendering(Model3DInstance& target) = 0;

    virtual void setTextureSize(int width, int height) = 0;
    virtual Texture& getTexture() = 0;
};

} // End of  CEGUI namespace section

#endif /* _Model3DRenderer_h_ */
