#include "CEGuiRender.h"
#include "GL3Renderer.h"
#include "CEGUI/System.h"
#include "CEGUI/DefaultResourceProvider.h"
#include "CEGUI/GUIContext.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/Font.h"
#include "CEGUI/FontManager.h"
#include "CEGUI/Scheme.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/AnimationManager.h"
#include "CEGUI/XMLParser.h"
#include "CEGUI/InputEvent.h"
#include "CEGUI/ScriptModule.h"
#include "CEGUI/falagard/WidgetLookManager.h"

#define CEGUI_DATAPATH_ENV "CEGUI_DATAPATH"
#define CEGUI_DATAPATH "./res"

void printWindowTree(CEGUI::Window* win, int depth, bool recursive)
{
    if (!win || depth > 6) {
        return;
    }
    const int log_size = 255;
    char log[log_size]  = {0};
    memset(log, '\t', log_size-2);
    log[log_size-1] = '\0';

    snprintf(log+depth, log_size - depth - 1, "Window[%p %s] [visible:%d alpha:%.1lf] [pos:%.2lf+%.1lf, %.2lf+%.1lf] [size:%.2lf+%.1lf, %.2lf+%.1lf] +++ \n",
             win, win->getName().c_str(),
             win->isVisible(),
             win->getAlpha(),
             win->getXPosition().d_scale, win->getXPosition().d_offset, win->getYPosition().d_scale, win->getYPosition().d_offset,
             win->getWidth().d_scale, win->getWidth().d_offset, win->getHeight().d_scale, win->getHeight().d_offset);
    printf(log);

    if (recursive) {
        size_t count = win->getChildCount();
        for (size_t idx = 0; idx<count; ++idx) {
            CEGUI::Window* pChildWin =  static_cast<CEGUI::Window*>(win->getChildElementAtIdx(idx));
            printWindowTree(pChildWin, depth+1, true);
        }
    }
    snprintf(log+depth, log_size - depth - 1, "Window --- \n");
    printf(log);
}

class CEGuiRenderPrivate
{
public:
    CEGuiRenderPrivate()
    {

    }

    ~CEGuiRenderPrivate()
    {

    }

};

CEGuiRender::CEGuiRender():
    m_data(new CEGuiRenderPrivate)
{

}

CEGuiRender::~CEGuiRender()
{
    if (m_data) {
        delete m_data;
        m_data = NULL;
    }
}

void CEGuiRender::initRender(const char* corePath)
{
    CEGUI::OpenGL3Renderer::bootstrapSystem();
    initResourceGroupDirectories(corePath);
    initDefaultResourceGroups();

    loadResource();
    createRootWindow();
}

void CEGuiRender::deinitRender()
{

}

bool CEGuiRender::loop()
{
    bool needswap = false;

    do {
        CEGUI::System& system = CEGUI::System::getSingleton();
        if (!system.getDefaultGUIContext().isDirty()) {
            break;
        }

        system.getDefaultGUIContext().injectTimePulse(33);
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);
        CEGUI_TRY{
            system.renderAllGUIContexts();
        }
        CEGUI_CATCH(CEGUI::Exception& e){
           CEGUI::Logger::getSingletonPtr()->logEvent(CEGUI::String("Exception")+CEGUI::String(e.what()));
        }
        needswap = true;
    } while(0);

    return needswap;
}

//----------------------------------------------------------------------------//
void CEGuiRender::initResourceGroupDirectories(const char* corePath)
{
    // initialise the required dirs for the DefaultResourceProvider
    CEGUI::DefaultResourceProvider* rp =
        static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());

    // get data path from environment var
    char* envDataPath = getenv(CEGUI_DATAPATH_ENV);

    // set data path prefix / base directory.  This will
    // be either from an environment variable, or from
    // a compiled in default based on original configure
    // options
    const char* path = envDataPath ? envDataPath : CEGUI_DATAPATH;
    if (corePath) {
        path = corePath;
    }

    CEGUI::String dataPathPrefix(path);
    setenv(CEGUI_DATAPATH_ENV, path, 1);

    /* for each resource type, set a resource group directory. We cast strings
       to "const CEGUI::utf8*" in order to support general Unicode strings,
       rather than only ASCII strings (even though currently they're all ASCII).
    */
    rp->setResourceGroupDirectory("schemes", dataPathPrefix +reinterpret_cast<const CEGUI::utf8*>("/schemes/"));
    rp->setResourceGroupDirectory("imagesets", dataPathPrefix +reinterpret_cast<const CEGUI::utf8*>("/imagesets/"));
    rp->setResourceGroupDirectory("fonts", dataPathPrefix +reinterpret_cast<const CEGUI::utf8*>("/fonts/"));
    rp->setResourceGroupDirectory("layouts", dataPathPrefix +reinterpret_cast<const CEGUI::utf8*>("/layouts/"));
    rp->setResourceGroupDirectory("looknfeels", dataPathPrefix +reinterpret_cast<const CEGUI::utf8*>("/looknfeel/"));
    rp->setResourceGroupDirectory("lua_scripts", dataPathPrefix +reinterpret_cast<const CEGUI::utf8*>("/lua_scripts/"));
    rp->setResourceGroupDirectory("schemas", dataPathPrefix +reinterpret_cast<const CEGUI::utf8*>("/xml_schemas/"));
    rp->setResourceGroupDirectory("animations", dataPathPrefix +reinterpret_cast<const CEGUI::utf8*>("/animations/"));
    rp->setResourceGroupDirectory("stringsets", dataPathPrefix +reinterpret_cast<const CEGUI::utf8*>("/strings/"));
}

//----------------------------------------------------------------------------//
void CEGuiRender::initDefaultResourceGroups()
{
    // set the default resource groups to be used
    CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
    CEGUI::Font::setDefaultResourceGroup("fonts");
    CEGUI::Scheme::setDefaultResourceGroup("schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
    CEGUI::WindowManager::setDefaultResourceGroup("layouts");
    //CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
    CEGUI::AnimationManager::setDefaultResourceGroup("animations");
    // setup default group for validation schemas
    CEGUI::XMLParser* parser = CEGUI::System::getSingleton().getXMLParser();
    if (parser->isPropertyPresent("SchemaDefaultResourceGroup")) {
        parser->setProperty("SchemaDefaultResourceGroup", "schemas");
    }
    CEGUI::SchemeManager::getSingleton().createFromFile("Generic.scheme");
}

void CEGuiRender::loadResource()
{
//    CEGUI::ImageManager::getSingleton().loadImageset("background.imageset");

//    CEGUI::ImageManager::getSingleton().loadImageset("home.imageset");
//    CEGUI::ImageManager::getSingleton().loadImageset("home1600.imageset");
//    CEGUI::ImageManager::getSingleton().loadImageset("home2560.imageset");

//    CEGUI::ImageManager::getSingleton().loadImageset("ac.imageset");
//    CEGUI::ImageManager::getSingleton().loadImageset("ac1600.imageset");
//    CEGUI::ImageManager::getSingleton().loadImageset("ac2560.imageset");

//    CEGUI::ImageManager::getSingleton().loadImageset("navi.imageset");
//    CEGUI::ImageManager::getSingleton().loadImageset("navi1600.imageset");
//    CEGUI::ImageManager::getSingleton().loadImageset("navi2560.imageset");

//    CEGUI::ImageManager::getSingleton().loadImageset("mediaplayer.imageset");
//    CEGUI::ImageManager::getSingleton().loadImageset("mediaplayer1600.imageset");
//    CEGUI::ImageManager::getSingleton().loadImageset("mediaplayer2560.imageset");

//    CEGUI::ImageManager::getSingleton().loadImageset("phone.imageset");
//    CEGUI::ImageManager::getSingleton().loadImageset("phone1600.imageset");
//    CEGUI::ImageManager::getSingleton().loadImageset("phone2560.imageset");

//    CEGUI::ImageManager::getSingleton().loadImageset("vehicle.imageset");
//    CEGUI::ImageManager::getSingleton().loadImageset("vehicle1600.imageset");
//    CEGUI::ImageManager::getSingleton().loadImageset("vehicle2560.imageset");
}

void CEGuiRender::createRootWindow()
{
    CEGUI::System& system = CEGUI::System::getSingleton();
    CEGUI::Window* root = CEGUI::WindowManager::getSingleton().createWindow("Generic/Image", "RootView");
    // root->setProperty("Image", "SpaceBackground/SpaceBackground");
    root->setSize(CEGUI::USize(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
    system.getDefaultGUIContext().setRootWindow(root);
}
