#ifndef PROGRAMCACHE_H
#define PROGRAMCACHE_H

#include "Program.h"
class Program;

extern const char* POSITIONSHADER;
extern const char* TESTSHADER;
extern const char* GRIDSHADER;
extern const char* PARTICLESHADER;
extern const char* POSITIONCOLORSHADER;
extern const char* POSITIONTEXTURESHADER;
extern const char* POSITIONTEXTURECOLORSHADER;

class ProgramCache {
    friend class Program;
public:
    ProgramCache();
    ~ProgramCache();

    void loadDefaultPrograms();
    Program * getProgram(const std::string &keyname);
    void addProgram(const std::string &keyname, Program* program);

    Program* getCurProgram();

protected:
    void setCurrentProgram(Program* program);
    typedef std::map<std::string, Program*> ProgramMap;
    ProgramMap m_programs;
    Program *m_curProgram;
};
#endif /* PROGRAMCACHE_H */
