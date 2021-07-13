#ifndef _Model3DManager_h_
#define _Model3DManager_h_

#include "CEGUI/Singleton.h"
#include "CEGUI/String.h"
#include "CEGUI/Model3D.h"

// Start of CEGUI namespace section
namespace CEGUI
{

class CEGUIEXPORT Model3DManager :
    public Singleton<Model3DManager>,
    public AllocatedObject<Model3DManager>
{
public:
    Model3DManager();
    ~Model3DManager();

    void createFromFile(const String& xml_filename, const String& resource_group = "");
    void destroyAllModel();
    void destroy(const String& name);
    
    static void setDefaultResourceGroup(const String& resourceGroup) { s_defaultResourceGroup = resourceGroup; }
    static const String& getDefaultResourceGroup() { return s_defaultResourceGroup; }

    bool isDefined(const String& object_name);
    Model3D& get(const String& object_name);

private:
    static void* load(void *arg);
    
    typedef std::map<String, Model3D*>::type Model3DMap;
    Model3DMap d_model3ds;
    
    static String s_defaultResourceGroup;
};

} // End of  CEGUI namespace section

#endif /* _Model3DManager_h_ */
