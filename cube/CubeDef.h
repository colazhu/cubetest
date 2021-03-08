#ifndef CUBEDEF
#define CUBEDEF

enum CubeEvent
{
    CUBE_EVENT_3FLICK_LEFT = 0,
    CUBE_EVENT_3FLICK_RIGHT,
};

class CubeCallback
{
public:
    virtual ~CubeCallback() {}
    virtual void onTexturePicked(int txtid) {}
};


#endif // CUBEDEF

