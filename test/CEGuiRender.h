#ifndef CEGUIRENDER_H
#define CEGUIRENDER_H
#include "Base.h"

class CEGuiRenderPrivate;
class CEGuiRender
{
public:
    CEGuiRender();
    ~CEGuiRender();

    void initRender(const char* corePath = NULL);
    void deinitRender();

    void createRootWindow();

    bool loop();
protected:
    void initDefaultResourceGroups();
    void initResourceGroupDirectories(const char* corePath);

    CEGuiRenderPrivate* m_data;
};

#endif // CEGUIRENDER_H
