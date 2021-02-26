#ifndef DIRECTOR_H
#define DIRECTOR_H

#include "MathInc.h"
#include "Geometry.h"
#include "NodeDef.h"

class Camera;
class Scene;
class Node;
class DirectorPrivate;
class MatrixStack;
class LightCache;
class TextureCache;
class ProgramCache;
class ActionCache;

enum LAYOUT_TYPE {
    LAYOUT_FULL_PORTRAIT = 0,
    LAYOUT_FULL_LANDSCAPE = 1,
    LAYOUT_HALF_LEFT = 2,
    LAYOUT_HALF_RIGHT = 3,
};

class Director
{
public:
    static Director* instance();
    static void destroy();

    virtual ~Director();

    void init();
    void deinit();

    void pause();
    void resume();
    bool loop();

    void setWindowSize(int width, int height);
    Rect getWindowSize();

    Camera* getCamera(const std::string& name);
    Camera* currentCamera();

    Scene* getScene(const std::string& name);
    Scene* currentScene();

    MatrixStack& matrixStack();
    LightCache& lightCache();
    TextureCache& textureCache();
    ProgramCache& programCache();
    ActionCache& actionCache();

    void injectTouch(const TouchEvent& event);
    void injectGesture(const GestureEvent& ev);
    float getFps();
    void setPlaneTexture(int planeid, int txtid);
    void addTexture(int txtid, int gltxtid, int w, int h);
    void addTexture(int txtid, void* buff, int w, int h);
    void rotateCube(float degrees, float ms);
    void setMode(int mode);
    void setLayout(int layout);
    int getLayout();

private:
    Director();
    void setDepthTest(bool on);
    void setAlphaBlending(bool on);
    void update(float dt);
    bool render();
    void cleanDeadPool();

    void drawScene();

    static Director* s_instance;
    DirectorPrivate* m_data;
    DISABLE_COPY(Director)
};

#endif // DIRECTOR_H
