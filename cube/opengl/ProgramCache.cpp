#include "ProgramCache.h"
#include "Program.h"
#include "Shaders.h"
#include "Log.h"

const char* POSITIONSHADER = "EMPTYSHADER";
const char* TESTSHADER = "TESTSHADER";
const char* GRIDSHADER = "ANOTHERSHADER";
const char* PARTICLESHADER = "PARTICLESHADER";
const char* POSITIONTEXTURESHADER = "POSITIONTEXTURESHADER";
const char* POSITIONTEXTURECOLORSHADER = "POSITIONTEXTURECOLORSHADER";
const char* POSITIONCOLORSHADER = "POSITIONCOLORESHADER";

struct defaultShaderData {
    const char* shaderName;
    const char* shaderVert;
    const char* shaderFrag;
};

const static defaultShaderData s_shaderMap[] = {
   { TESTSHADER,TESTSHADER_VERT,TESTSHADER_FRAG	},
   { POSITIONTEXTURESHADER,POSITIONTEXTURE_VERT,POSITIONTEXTURE_FRAG },
   { POSITIONSHADER, POSITION_VERT, POSITION_FRAG },
   { POSITIONCOLORSHADER, POSITIONCOLOR_VERT, POSITIONCOLOR_FRAG },
   { "", "", ""}
//   { PARTICLESHADER, POSITIONCOLORSIZE_VERT, POSITIONCOLORSIZE_FRAG },
//   { GRIDSHADER, GRID_VERT, GRID_FRAG },

//   { POSITIONTEXTURECOLORSHADER,POSITIONTEXTURECOLOR_VERT,POSITIONTEXTURECOLOR_FRAG },
//   { "", "", ""}
};

ProgramCache::ProgramCache()
        : m_programs(),
          m_curProgram(0) {

}

ProgramCache::~ProgramCache() {
    for (ProgramMap::iterator it = m_programs.begin(); it != m_programs.end(); ++it) {
        SAFE_DELETE(it->second);
    }
}

void ProgramCache::loadDefaultPrograms() {
	size_t shaderMapSize = sizeof(s_shaderMap) / sizeof(s_shaderMap[0]);
    for (size_t i = 0; i < shaderMapSize; ++i) {
        if (0 == strlen(s_shaderMap[i].shaderName)) {
            break;
        }
        if (Program *p = Program::createWithByteArrays(s_shaderMap[i].shaderVert,
                                                       s_shaderMap[i].shaderFrag)) {
            m_programs[s_shaderMap[i].shaderName] = p;
        }
        else {
            LOG_BASE("create default shader:%s error", s_shaderMap[i].shaderName);
        }
        CHECK_GL_ERROR_DEBUG();
    }
}

Program* ProgramCache::getProgram(const std::string &keyname) {
    ProgramMap::iterator it = m_programs.find(keyname);
    if (it != m_programs.end()) {
        return it->second;
    }
    return NULL;
}

void ProgramCache::addProgram(const std::string &keyname, Program* program) {
    if(!program || m_programs.count(keyname)) {
		return;
	}
	
     m_programs[keyname] = program;
}

void ProgramCache::setCurrentProgram(Program* program)
{
    m_curProgram = program;
}

Program* ProgramCache::getCurProgram()
{
    return m_curProgram;
}
