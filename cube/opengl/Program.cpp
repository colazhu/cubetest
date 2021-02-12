#include "Program.h"
// #include "CommonInc.h"
#include "Director.h"
#include "Camera.h"
#include "Log.h"
#include "MatrixStack.h"
#include "LightCache.h"

extern const char* LIGHTING_VERT;

// extern const char* RIPPLEEFFECT_VERT;
// uniform names
const char* Program::UNIFORM_NAME_P_MATRIX = "uc_PMatrix";
const char* Program::UNIFORM_NAME_MV_MATRIX = "uc_MVMatrix";
const char* Program::UNIFORM_NAME_MV_INVERSE_MATRIX = "uc_MVInverseMatrix";
const char* Program::UNIFORM_NAME_MVP_MATRIX = "uc_MVPMatrix";
const char* Program::UNIFORM_NAME_EYE = "uc_Eye";
const char* Program::UNIFORM_NAME_TIME = "uc_Time";
const char* Program::UNIFORM_NAME_SIN_TIME = "uc_SinTime";
const char* Program::UNIFORM_NAME_COS_TIME = "uc_CosTime";
const char* Program::UNIFORM_NAME_RANDOM01 = "uc_Random01";
const char* Program::UNIFORM_NAME_SAMPLER0 = "uc_Texture0";
const char* Program::UNIFORM_NAME_SAMPLER1 = "uc_Texture1";
const char* Program::UNIFORM_NAME_SAMPLER2 = "uc_Texture2";
const char* Program::UNIFORM_NAME_SAMPLER3 = "uc_Texture3";
const char* Program::UNIFORM_NAME_ALPHA_TEST_VALUE = "uc_alpha_value";

// Attribute names
const char* Program::ATTRIBUTE_NAME_COLOR = "a_color";
const char* Program::ATTRIBUTE_NAME_POSITION = "a_position";
const char* Program::ATTRIBUTE_NAME_TEX_COORD = "a_texCoord";
const char* Program::ATTRIBUTE_NAME_NORMAL = "a_normal";
const char* Program::ATTRIBUTE_NAME_SIZE = "a_size";

Program::Program()
        : m_program(NULL), m_vertShader(NULL), m_fragShader(NULL) {
    memset(_builtInUniforms, 0, sizeof(_builtInUniforms));
}

Program::~Program() {
//     // there is no need to delete the shaders. They should have been already deleted.
    ASSERT(m_vertShader == 0, "Vertex Shaders should have been already deleted");
    ASSERT(m_fragShader == 0, "Fragment Shaders should have been already deleted");

    if (m_program) {
        glDeleteProgram(m_program);
    }

//    tHashUniformEntry *current_element, *tmp;
    for (UniformEntryList::iterator it = m_uniformEntryList.begin(); it != m_uniformEntryList.end();
            ++it) {
        if (UniformEntry* entry = (*it)) {
            free(entry->value);
            free(entry);
        }
    }
    m_uniformEntryList.clear();
}

Program* Program::createWithByteArrays(const GLchar* vShaderByteArray,
                                       const GLchar* fShaderByteArray) {
    Program* ret = new Program();
    if (ret && ret->initWithByteArrays(vShaderByteArray, fShaderByteArray)) {
        ret->link();
        ret->updateUniforms();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return NULL;
}

bool Program::initWithByteArrays(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray) {
    m_program = glCreateProgram();
    CHECK_GL_ERROR_DEBUG();

    m_vertShader = m_fragShader = 0;

    // Create and compile vertex shader
    if (vShaderByteArray) {
        if (!compileShader(&m_vertShader, GL_VERTEX_SHADER, vShaderByteArray)) {
            LOG_BASE("ERROR: Failed to compile vertex shader");
            return false;
        }
    }

    // Create and compile fragment shader
    if (fShaderByteArray) {
        if (!compileShader(&m_fragShader, GL_FRAGMENT_SHADER, fShaderByteArray)) {
            LOG_BASE("ERROR: Failed to compile fragment shader");
            return false;
        }
    }

    if (m_vertShader) {
        glAttachShader(m_program, m_vertShader);
    }
    CHECK_GL_ERROR_DEBUG();

    if (m_fragShader) {
        glAttachShader(m_program, m_fragShader);
    }

    CHECK_GL_ERROR_DEBUG();

    return true;
}


bool Program::compileShader(GLuint * shader, GLenum type, const GLchar* source) {
    GLint status;

    if (!source) {
        return false;
    }

    const GLchar *sources[] = {
        "#version 100\n"
//        (type == GL_VERTEX_SHADER ? "precision highp float;\n" : "precision mediump float;\n"),
        "precision mediump float;\n",
        "uniform mat4 uc_PMatrix;\n"
        "uniform mat4 uc_MVMatrix;\n"
        "uniform mat4 uc_MVInverseMatrix;\n"
        "uniform mat4 uc_MVPMatrix;\n"
        "uniform vec4 uc_Eye;\n"
        "uniform vec4 uc_Time;\n"
        "uniform sampler2D uc_Texture0;\n",
      (type == GL_VERTEX_SHADER ? LIGHTING_VERT : ""),
//		(type == GL_VERTEX_SHADER ? RIPPLEEFFECT_VERT : ""),
        source, };

    *shader = GLHook::glCreateShader(type);

    GLHook::glShaderSource(*shader, sizeof(sources) / sizeof(*sources), sources, NULL);
    GLHook::glCompileShader(*shader);

    GLHook::glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);

    if (!status) {
        GLsizei length;
        GLHook::glGetShaderiv(*shader, GL_SHADER_SOURCE_LENGTH, &length);
        GLchar* src = (GLchar *) malloc(sizeof(GLchar) * length);

        GLHook::glGetShaderSource(*shader, length, NULL, src);
        LOG_BASE_E("!!! Failed to compile shader !!!");
        free(src);
    }
    return (status == GL_TRUE);
}

void Program::bindPredefinedVertexAttribs() {
    static const struct {
        const char *attributeName;
        int location;
    } attribute_locations[] = { { ATTRIBUTE_NAME_POSITION, VERTEX_ATTRIB_POSITION },
                                { ATTRIBUTE_NAME_COLOR, VERTEX_ATTRIB_COLOR },
                                { ATTRIBUTE_NAME_TEX_COORD, VERTEX_ATTRIB_TEX_COORD },
                                { ATTRIBUTE_NAME_NORMAL, VERTEX_ATTRIB_NORMAL },
								{ ATTRIBUTE_NAME_SIZE, VERTEX_ATTRIB_SIZE }, };

    const int size = sizeof(attribute_locations) / sizeof(attribute_locations[0]);

    for (int i = 0; i < size; ++i) {
        GLHook::glBindAttribLocation(m_program,
                             attribute_locations[i].location,
                             attribute_locations[i].attributeName);
    }
}

void Program::addAttribute(const char* attributeName, GLuint index) {
    GLHook::glBindAttribLocation(m_program, index, attributeName);
}

void Program::updateUniforms() {
    _builtInUniforms[UNIFORM_P_MATRIX] = GLHook::glGetUniformLocation(m_program, UNIFORM_NAME_P_MATRIX);
    _builtInUniforms[UNIFORM_MV_MATRIX] = GLHook::glGetUniformLocation(m_program, UNIFORM_NAME_MV_MATRIX);
    _builtInUniforms[UNIFORM_MV_INVERSE_MATRIX] = GLHook::glGetUniformLocation(m_program,
                                                                       UNIFORM_NAME_MV_INVERSE_MATRIX);
    _builtInUniforms[UNIFORM_MVP_MATRIX] = GLHook::glGetUniformLocation(m_program, UNIFORM_NAME_MVP_MATRIX);

    _builtInUniforms[UNIFORM_TIME] = GLHook::glGetUniformLocation(m_program, UNIFORM_NAME_TIME);
//    _builtInUniforms[UNIFORM_SIN_TIME] = GLHook::glGetUniformLocation(m_program, UNIFORM_NAME_SIN_TIME);
//    _builtInUniforms[UNIFORM_COS_TIME] = GLHook::glGetUniformLocation(m_program, UNIFORM_NAME_COS_TIME);

    _builtInUniforms[UNIFORM_RANDOM01] = GLHook::glGetUniformLocation(m_program, UNIFORM_NAME_RANDOM01);

    _builtInUniforms[UNIFORM_SAMPLER0] = GLHook::glGetUniformLocation(m_program, UNIFORM_NAME_SAMPLER0);
    _builtInUniforms[UNIFORM_SAMPLER1] = GLHook::glGetUniformLocation(m_program, UNIFORM_NAME_SAMPLER1);
    _builtInUniforms[UNIFORM_SAMPLER2] = GLHook::glGetUniformLocation(m_program, UNIFORM_NAME_SAMPLER2);
    _builtInUniforms[UNIFORM_SAMPLER3] = GLHook::glGetUniformLocation(m_program, UNIFORM_NAME_SAMPLER3);

    _builtInUniforms[UNIFORM_EYE] = GLHook::glGetUniformLocation(m_program, UNIFORM_NAME_EYE);

    _flags.usesP = _builtInUniforms[UNIFORM_P_MATRIX] != -1;
    _flags.usesMV = _builtInUniforms[UNIFORM_MV_MATRIX] != -1;
    _flags.usesMVInverse = _builtInUniforms[UNIFORM_MV_INVERSE_MATRIX] != -1;
    _flags.usesMVP = _builtInUniforms[UNIFORM_MVP_MATRIX] != -1;
    _flags.usesTime = (_builtInUniforms[UNIFORM_TIME] != -1
         /*   || _builtInUniforms[UNIFORM_SIN_TIME] != -1
            || _builtInUniforms[UNIFORM_COS_TIME] != -1*/);
    _flags.usesRandom = _builtInUniforms[UNIFORM_RANDOM01] != -1;
    _flags.usesEye = _builtInUniforms[UNIFORM_EYE] != -1;

    Director* director = Director::instance();
    _flags.usesLighting = director->lightCache().useLighting(m_program);

    // Since sample most probably won't change, set it to 0,1,2,3 now.
    if(_builtInUniforms[UNIFORM_SAMPLER0] != -1)
        setUniformLocationWith1i(_builtInUniforms[UNIFORM_SAMPLER0], 0);
    if(_builtInUniforms[UNIFORM_SAMPLER1] != -1)
        setUniformLocationWith1i(_builtInUniforms[UNIFORM_SAMPLER1], 1);
    if(_builtInUniforms[UNIFORM_SAMPLER2] != -1)
        setUniformLocationWith1i(_builtInUniforms[UNIFORM_SAMPLER2], 2);
    if(_builtInUniforms[UNIFORM_SAMPLER3] != -1)
        setUniformLocationWith1i(_builtInUniforms[UNIFORM_SAMPLER3], 3);
}

bool Program::link() {
    ASSERT(m_program != 0, "Cannot link invalid program");

    GLint status = GL_TRUE;

    bindPredefinedVertexAttribs();

    GLHook::glLinkProgram(m_program);

    parseActiveAttribute();
    parseActiveUniforms();

    if (m_vertShader) {
        GLHook::glDeleteShader(m_vertShader);
    }

    if (m_fragShader) {
        GLHook::glDeleteShader(m_fragShader);
    }

    m_vertShader = m_fragShader = 0;

    GLHook::glGetProgramiv(m_program, GL_LINK_STATUS, &status);

    if (status == GL_FALSE) {
        GLHook::glDeleteProgram(m_program);
        m_program = 0;
    }

    return (status == GL_TRUE);
}

void Program::use() {

    GLHook::glUseProgram(m_program);
    Director::instance()->programCache().setCurrentProgram(this);
}

// Uniform cache
bool Program::updateUniformLocation(GLint location, GLvoid* data, unsigned int bytes) {
    if (location < 0) {
        return false;
    }

    bool updated = true;
    UniformEntry* entry = NULL;
    for (UniformEntryList::iterator it = m_uniformEntryList.begin(); it != m_uniformEntryList.end();
            ++it) {
        UniformEntry* ent = (*it);
        if (ent && ent->location == location) {
            entry = ent;
            break;
        }
    }

    if (!entry) {
        entry = (UniformEntry*) malloc(sizeof(*entry));
        entry->location = location;
        entry->value = malloc(bytes);
        memcpy(entry->value, data, bytes);
        m_uniformEntryList.push_back(entry);
    }
    else {
        if (memcmp(entry->value, data, bytes) == 0) {
            updated = false;
        }
        else {
            memcpy(entry->value, data, bytes);
        }
    }

    return updated;
}

GLint Program::getUniformLocationForName(const char* name) {
    ASSERT(name != NULL, "Invalid uniform name" );
    ASSERT(m_program != 0, "Invalid operation. Cannot get uniform location when program is not initialized");

    return GLHook::glGetUniformLocation(m_program, name);
}

void Program::setUniformLocationWith1i(GLint location, GLint i1) {
    bool updated = updateUniformLocation(location, &i1, sizeof(i1) * 1);

    if (updated) {
        GLHook::glUniform1i((GLint) location, i1);
    }
}

void Program::setUniformLocationWith2i(GLint location, GLint i1, GLint i2) {
    GLint ints[2] = { i1, i2 };
    bool updated = updateUniformLocation(location, ints, sizeof(ints));

    if (updated) {
        GLHook::glUniform2i((GLint) location, i1, i2);
    }
}

void Program::setUniformLocationWith3i(GLint location, GLint i1, GLint i2, GLint i3) {
    GLint ints[3] = { i1, i2, i3 };
    bool updated = updateUniformLocation(location, ints, sizeof(ints));

    if (updated) {
        GLHook::glUniform3i((GLint) location, i1, i2, i3);
    }
}

void Program::setUniformLocationWith4i(GLint location, GLint i1, GLint i2, GLint i3, GLint i4) {
    GLint ints[4] = { i1, i2, i3, i4 };
    bool updated = updateUniformLocation(location, ints, sizeof(ints));

    if (updated) {
        GLHook::glUniform4i((GLint) location, i1, i2, i3, i4);
    }
}

void Program::setUniformLocationWith1iv(GLint location, GLint* ints, unsigned int numberOfArrays) {
    bool updated = updateUniformLocation(location, ints, sizeof(int) * numberOfArrays);

    if (updated) {
        GLHook::glUniform1iv((GLint) location, (GLsizei) numberOfArrays, ints);
    }
}

void Program::setUniformLocationWith2iv(GLint location, GLint* ints, unsigned int numberOfArrays) {
    bool updated = updateUniformLocation(location, ints, sizeof(int) * 2 * numberOfArrays);

    if (updated) {
        GLHook::glUniform2iv((GLint) location, (GLsizei) numberOfArrays, ints);
    }
}

void Program::setUniformLocationWith3iv(GLint location, GLint* ints, unsigned int numberOfArrays) {
    bool updated = updateUniformLocation(location, ints, sizeof(int) * 3 * numberOfArrays);

    if (updated) {
        GLHook::glUniform3iv((GLint) location, (GLsizei) numberOfArrays, ints);
    }
}

void Program::setUniformLocationWith4iv(GLint location, GLint* ints, unsigned int numberOfArrays) {
    bool updated = updateUniformLocation(location, ints, sizeof(int) * 4 * numberOfArrays);

    if (updated) {
        GLHook::glUniform4iv((GLint) location, (GLsizei) numberOfArrays, ints);
    }
}

void Program::setUniformLocationWith1f(GLint location, GLfloat f1) {
    bool updated = updateUniformLocation(location, &f1, sizeof(f1) * 1);

    if (updated) {
        GLHook::glUniform1f((GLint) location, f1);
    }
}

void Program::setUniformLocationWith2f(GLint location, GLfloat f1, GLfloat f2) {
    GLfloat floats[2] = { f1, f2 };
    bool updated = updateUniformLocation(location, floats, sizeof(floats));

    if (updated) {
        GLHook::glUniform2f((GLint) location, f1, f2);
    }
}

void Program::setUniformLocationWith3f(GLint location, GLfloat f1, GLfloat f2, GLfloat f3) {
    GLfloat floats[3] = { f1, f2, f3 };
    bool updated = updateUniformLocation(location, floats, sizeof(floats));

    if (updated) {
        GLHook::glUniform3f((GLint) location, f1, f2, f3);
    }
}

void Program::setUniformLocationWith4f(GLint location,
                                       GLfloat f1,
                                       GLfloat f2,
                                       GLfloat f3,
                                       GLfloat f4) {
    GLfloat floats[4] = { f1, f2, f3, f4 };
    bool updated = updateUniformLocation(location, floats, sizeof(floats));

    if (updated) {
        GLHook::glUniform4f((GLint) location, f1, f2, f3, f4);
    }
}

void Program::setUniformLocationWith1fv(GLint location,
                                        GLfloat* floats,
                                        unsigned int numberOfArrays) {
    bool updated = updateUniformLocation(location, floats, sizeof(float) * numberOfArrays);

    if (updated) {
        GLHook::glUniform1fv((GLint) location, (GLsizei) numberOfArrays, floats);
    }
}

void Program::setUniformLocationWith2fv(GLint location,
                                        GLfloat* floats,
                                        unsigned int numberOfArrays) {
    bool updated = updateUniformLocation(location, floats, sizeof(float) * 2 * numberOfArrays);

    if (updated) {
        GLHook::glUniform2fv((GLint) location, (GLsizei) numberOfArrays, floats);
    }
}

void Program::setUniformLocationWith3fv(GLint location,
                                        GLfloat* floats,
                                        unsigned int numberOfArrays) {
    bool updated = updateUniformLocation(location, floats, sizeof(float) * 3 * numberOfArrays);

    if (updated) {
        GLHook::glUniform3fv((GLint) location, (GLsizei) numberOfArrays, floats);
    }
}

void Program::setUniformLocationWith4fv(GLint location,
                                        GLfloat* floats,
                                        unsigned int numberOfArrays) {
    bool updated = updateUniformLocation(location, floats, sizeof(float) * 4 * numberOfArrays);

    if (updated) {
        GLHook::glUniform4fv((GLint) location, (GLsizei) numberOfArrays, floats);
    }
}

void Program::setUniformLocationWithMatrix4fv(GLint location,
                                              GLfloat* matrixArray,
                                              unsigned int numberOfMatrices) {
    bool updated = updateUniformLocation(location,
                                         matrixArray,
                                         sizeof(float) * 16 * numberOfMatrices);

    if (updated) {
        GLHook::glUniformMatrix4fv((GLint) location, (GLsizei) numberOfMatrices, GL_FALSE, matrixArray);
    }
}

void Program::setUniformsForBuiltins() {
    Director* director = Director::instance();
    ASSERT(NULL != director, "Director is null when seting matrix stack");

    Matrix matrixMV;
    matrixMV = director->matrixStack().getMatrix(MATRIX_STACK_MODELVIEW);

    setUniformsForBuiltins(matrixMV);
}

void Program::setUniformsForBuiltins(Matrix &matrixMV) {
    Director* director = Director::instance();
    Matrix matrixP = director->matrixStack().getMatrix(MATRIX_STACK_PROJECTION);

	if (_flags.usesP) {
        setUniformLocationWithMatrix4fv(_builtInUniforms[UNIFORM_P_MATRIX], matrixP.m, 1);
	}

    if (_flags.usesMV) {
        setUniformLocationWithMatrix4fv(_builtInUniforms[UNIFORM_MV_MATRIX], matrixMV.m, 1);
	}

    if (_flags.usesMVInverse) {
        Matrix matrixMVInverse = matrixMV;
        matrixMVInverse.invert();
        setUniformLocationWithMatrix4fv(_builtInUniforms[UNIFORM_MV_INVERSE_MATRIX],
                                        matrixMVInverse.m,
                                        1);
    }

    if (_flags.usesMVP) {
        Matrix matrixMVP = matrixP * matrixMV;
        setUniformLocationWithMatrix4fv(_builtInUniforms[UNIFORM_MVP_MATRIX], matrixMVP.m, 1);
    }

    if (_flags.usesEye) {
        Vector3 eye3 = director->currentCamera()->eye();
        Vector4 eye(eye3.x, eye3.y, eye3.z, 1.0);
        setUniformLocationWith4fv(_builtInUniforms[UNIFORM_EYE], &eye.x, 1);
    }

    if (_flags.usesLighting) {
         director->lightCache().updateUniform(this);
    }

// 	if(_flags.usesTime) {
// 		float time = director->getTotalFrames() * director->getAnimationInterval();// 
// 		setUniformLocationWith4f(_builtInUniforms[UNIFORM_TIME], time/10.0, time, time*2, time*4);
// 		setUniformLocationWith4f(_builtInUniforms[UNIFORM_SIN_TIME], time/8.0, time/4.0, time/2.0, sinf(time));
// 		setUniformLocationWith4f(_builtInUniforms[UNIFORM_COS_TIME], time/8.0, time/4.0, time/2.0, cosf(time));
// 	}

	if (_flags.usesRandom) {
//        setUniformLocationWith4f(_builtInUniforms[UNIFORM_RANDOM01],
//                                 CCRANDOM_0_1(),
//                                 CCRANDOM_0_1(),
//                                 CCRANDOM_0_1(),
//                                 CCRANDOM_0_1());
	}
}

void Program::setUniformTime(float time)
{
    if(_flags.usesTime) {
        setUniformLocationWith4f(_builtInUniforms[UNIFORM_TIME], time, 1.0 - time, sinf(time), cosf(time));
    }
}

void Program::parseActiveUniforms() {
    m_userUniforms.clear();

    // Query and store uniforms from the program.
    GLint activeUniforms;
    GLHook::glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &activeUniforms);
    if (activeUniforms > 0) {
        GLint length;
        GLHook::glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length);
        if (length > 0) {
            Uniform uniform;

            GLchar* uniformName = (GLchar*) alloca(length + 1);

            for (int i = 0; i < activeUniforms; ++i) {
                // Query uniform info.
                GLHook::glGetActiveUniform(m_program,
                                   i,
                                   length,
                                   NULL,
                                   &uniform.size,
                                   &uniform.type,
                                   uniformName);
                uniformName[length] = '\0';

                // Only add uniforms that are not built-in.
                // The ones that start with 'u_' are built-ins
                if (strncmp("uc_", uniformName, 3) != 0) {

                    // remove possible array '[]' from uniform name
                    if (uniform.size > 1 && length > 3) {
                        char* c = strrchr(uniformName, '[');
                        if (c) {
                            *c = '\0';
                        }
                    }
                    uniform.name = std::string(uniformName);
                    uniform.location = GLHook::glGetUniformLocation(m_program, uniformName);
// 					GLenum __gl_error_code = GLHook::glGetError();
// 					if (__gl_error_code != GL_NO_ERROR) 
// 					{ 
// 						LOG_BASE("error: 0x%x", (int)__gl_error_code);
// 					} 
// 					assert(__gl_error_code == GL_NO_ERROR);

                    m_userUniforms.insert(std::make_pair(uniform.name, uniform));
                }
            }
        }
    }

    LOG_BASE_D("parseActiveUniforms: [");
    for (UniformMap::iterator it = m_userUniforms.begin();
            it != m_userUniforms.end(); ++it) {
        LOG_BASE_D("\t[%s][location:%d,size:%d,type:%d,name:%s]", it->first.c_str(), it->second.location, it->second.size, it->second.type, it->second.name.c_str());
    }
    LOG_BASE_D("]");
}

void Program::parseActiveAttribute() {
    m_vertexAttribs.clear();

    // Query and store vertex attribute meta-data from the program.
    GLint activeAttributes;
    GLint length;
    GLHook::glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &activeAttributes);
    if (activeAttributes > 0) {
        VertexAttrib attribute;

        GLHook::glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &length);
        if (length > 0) {
            GLchar* attribName = (GLchar*) alloca(length + 1);

            for (int i = 0; i < activeAttributes; ++i) {
                // Query attribute info.
                GLHook::glGetActiveAttrib(m_program,
                                  i,
                                  length,
                                  NULL,
                                  &attribute.size,
                                  &attribute.type,
                                  attribName);
                attribName[length] = '\0';
                attribute.name = std::string(attribName);

                // Query the pre-assigned attribute location
                attribute.index = GLHook::glGetAttribLocation(m_program, attribName);
                m_vertexAttribs.insert(std::make_pair(attribute.name, attribute));
            }
        }
    }

    LOG_BASE_D("parseActiveAttribute: [");
    for (VertexAttribMap::iterator it = m_vertexAttribs.begin(); it != m_vertexAttribs.end(); ++it) {
        LOG_BASE_D("\t[%s][idx:%d,size:%d,type:%d,name:%s]", it->first.c_str(), it->second.index, it->second.size, it->second.type, it->second.name.c_str());
    }
    LOG_BASE_D("]");
}

Uniform* Program::getUniform(const std::string &name) {
    UniformMap::iterator it = m_userUniforms.find(name);
    if (it != m_userUniforms.end())
        return &it->second;
    return NULL;
}

VertexAttrib* Program::getVertexAttrib(const std::string &name) {
    VertexAttribMap::iterator it = m_vertexAttribs.find(name);
    if (it != m_vertexAttribs.end()) {
        return &it->second;
	}
    return NULL;

}

void Program::reset() {
    m_vertShader = m_fragShader = 0;
    memset(_builtInUniforms, 0, sizeof(_builtInUniforms));

    if (m_program) {
        // it is already deallocated by android
        GLHook::glDeleteProgram(m_program);
    }
    m_program = 0;
}

