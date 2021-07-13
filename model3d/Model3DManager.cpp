#include "CEGUI/Model3DManager.h"
#include "CEGUI/Model3D_xmlHandler.h"
#include <pthread.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
template<> Model3DManager* Singleton<Model3DManager>::ms_Singleton  = 0;
String Model3DManager::s_defaultResourceGroup("");

//----------------------------------------------------------------------------//
Model3DManager::Model3DManager()
{

}

//----------------------------------------------------------------------------//
Model3DManager::~Model3DManager()
{
    destroyAllModel();
}

String filename_temp;
//----------------------------------------------------------------------------//
void Model3DManager::createFromFile(const String& xml_filename, const String& resource_group)
{
    String group;
    if (resource_group != "") group = resource_group;
    else group = s_defaultResourceGroup;
    
//    pthread_t tid;
//    filename_temp = xml_filename;
//    pthread_create(&tid, NULL, load, NULL);
    Model3D_xmlHandler xmlHandler;
    xmlHandler.handleFile(xml_filename, group);
    Model3D* model = xmlHandler.d_model;
    d_model3ds[model->getName()] = model;
}

//----------------------------------------------------------------------------//
void* Model3DManager::load(void *arg)
{
    Model3D_xmlHandler xmlHandler;
    Model3DManager& target = Model3DManager::getSingleton();
    xmlHandler.handleFile(filename_temp, target.getDefaultResourceGroup());
    Model3D* model = xmlHandler.d_model;
    target.d_model3ds[model->getName()] = model;
    return NULL;
}
    
//----------------------------------------------------------------------------//
void Model3DManager::destroyAllModel()
{
    for (auto i = d_model3ds.begin(); i != d_model3ds.end(); ++i) {
        CEGUI_DELETE_AO i->second;
    }
    d_model3ds.clear();
}

//----------------------------------------------------------------------------//
bool Model3DManager::isDefined(const String& object_name)
{
    if (d_model3ds.find(object_name) != d_model3ds.end()) return true;
    else return false;
}

//----------------------------------------------------------------------------//
Model3D& Model3DManager::get(const String& object_name)
{
    return *(d_model3ds[object_name]);
}

//----------------------------------------------------------------------------//
void Model3DManager::destroy(const String& name)
{
    if (!isDefined(name)) return;
    CEGUI_DELETE_AO d_model3ds[name];
    d_model3ds.erase(name);
}

//----------------------------------------------------------------------------//
} // End of  CEGUI namespace section
