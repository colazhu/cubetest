#ifndef PROGRAM_H
#define PROGRAM_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#include "Base.h"
#include "GLFunction.h"
#include "MathInc.h"

enum {
    UNIFORM_P_MATRIX,
    UNIFORM_MV_MATRIX,
    UNIFORM_MV_INVERSE_MATRIX,
    UNIFORM_MVP_MATRIX,
    UNIFORM_TIME,
    UNIFORM_SIN_TIME,
    UNIFORM_COS_TIME,
    UNIFORM_RANDOM01,
    UNIFORM_SAMPLER0,
    UNIFORM_SAMPLER1,
    UNIFORM_SAMPLER2,
    UNIFORM_SAMPLER3,
    UNIFORM_EYE,
    UNIFORM_MAX,
};

enum {
    VERTEX_ATTRIB_POSITION,
    VERTEX_ATTRIB_NORMAL,
    VERTEX_ATTRIB_TEX_COORD,
    VERTEX_ATTRIB_COLOR,
	VERTEX_ATTRIB_SIZE,
    VERTEX_ATTRIB_BLEND_WEIGHT,
    VERTEX_ATTRIB_BLEND_INDEX,

    VERTEX_ATTRIB_MAX,
};

struct Uniform {
    GLint location;
    GLint size;
    GLenum type;
    std::string name;
};

struct VertexAttrib {
    GLuint index;
    GLint size;
    GLenum type;
    std::string name;
};

class Program {
public:
    Program();
    virtual ~Program();


    /** Initializes the Program with a vertex and fragment with bytes array 
     */
    static Program* createWithByteArrays(const GLchar* vShaderByteArray,
                                         const GLchar* fShaderByteArray);


    bool initWithByteArrays(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray);

    /**  It will add a new attribute to the shader 
     */
    void addAttribute(const char* attributeName, GLuint index);

    /** links the glProgram 
     */
    bool link();

    /** it will call glUseProgram() 
     */
    void use();

    /** It will create 4 uniforms:
     - UniformPMatrix
     - UniformMVMatrix
     - UniformMVPMatrix
     - UniformSampler
     */
    void updateUniforms();

    //get user defined uniform and attribute by name
    Uniform* getUniform(const std::string& name);
    VertexAttrib* getVertexAttrib(const std::string& name);

    /** calls retrieves the named uniform location for this shader program. 
     */
    GLint getUniformLocationForName(const char* name);

    /** calls glUniform1i only if the values are different than the previous call for this same shader program. 
     */
    void setUniformLocationWith1i(GLint location, GLint i1);

    /** calls glUniform2i only if the values are different than the previous call for this same shader program. 
     */
    void setUniformLocationWith2i(GLint location, GLint i1, GLint i2);

    /** calls glUniform3i only if the values are different than the previous call for this same shader program. 
     */
    void setUniformLocationWith3i(GLint location, GLint i1, GLint i2, GLint i3);

    /** calls glUniform4i only if the values are different than the previous call for this same shader program.
     */
    void setUniformLocationWith4i(GLint location, GLint i1, GLint i2, GLint i3, GLint i4);

    /** calls glUniform1iv only if the values are different than the previous call for this same shader program.
     */
    void setUniformLocationWith1iv(GLint location, GLint* ints, unsigned int numberOfArrays);

    /** calls glUniform2iv only if the values are different than the previous call for this same shader program. 
     */
    void setUniformLocationWith2iv(GLint location, GLint* ints, unsigned int numberOfArrays);

    /** calls glUniform3iv only if the values are different than the previous call for this same shader program.
     */
    void setUniformLocationWith3iv(GLint location, GLint* ints, unsigned int numberOfArrays);

    /** calls glUniform4iv only if the values are different than the previous call for this same shader program. 
     */

    void setUniformLocationWith4iv(GLint location, GLint* ints, unsigned int numberOfArrays);

    /** calls glUniform1f only if the values are different than the previous call for this same shader program. 
     */
    void setUniformLocationWith1f(GLint location, GLfloat f1);

    /** calls glUniform2f only if the values are different than the previous call for this same shader program. 
     */
    void setUniformLocationWith2f(GLint location, GLfloat f1, GLfloat f2);

    /** calls glUniform3f only if the values are different than the previous call for this same shader program. 
     */
    void setUniformLocationWith3f(GLint location, GLfloat f1, GLfloat f2, GLfloat f3);

    /** calls glUniform4f only if the values are different than the previous call for this same shader program. 
     */
    void setUniformLocationWith4f(GLint location, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4);

    /** calls glUniform1fv only if the values are different than the previous call for this same shader program.
     */
    void setUniformLocationWith1fv(GLint location, GLfloat* floats, unsigned int numberOfArrays);

    /** calls glUniform2fv only if the values are different than the previous call for this same shader program. 
     */
    void setUniformLocationWith2fv(GLint location, GLfloat* floats, unsigned int numberOfArrays);

    /** calls glUniform3fv only if the values are different than the previous call for this same shader program. 
     */
    void setUniformLocationWith3fv(GLint location, GLfloat* floats, unsigned int numberOfArrays);

    /** calls glUniform4fv only if the values are different than the previous call for this same shader program. 
     */
    void setUniformLocationWith4fv(GLint location, GLfloat* floats, unsigned int numberOfArrays);

    /** calls glUniformMatrix4fv only if the values are different than the previous call for this same shader program. 
     */
    void setUniformLocationWithMatrix4fv(GLint location,
                                         GLfloat* matrixArray,
                                         unsigned int numberOfMatrices);

    /** will update the builtin uniforms if they are different than the previous call for this same shader program. 
     */
    void setUniformsForBuiltins();
    void setUniformsForBuiltins(Matrix &modelView);
    void setUniformTime(float time);

    /** reload all shaders, this function is designed for android
     *  when opengl context lost, so don't call it.
     */
    void reset();

    /**
     */
    inline const GLuint getProgram() {
        return m_program;
    }

private:
    bool updateUniformLocation(GLint location, GLvoid* data, unsigned int bytes);
    bool compileShader(GLuint * shader, GLenum type, const GLchar* source);
    void bindPredefinedVertexAttribs();
    void parseActiveUniforms();
    void parseActiveAttribute();

private:
    // uniform names
    static const char* UNIFORM_NAME_P_MATRIX;
    static const char* UNIFORM_NAME_MV_MATRIX;
    static const char* UNIFORM_NAME_MV_INVERSE_MATRIX;
    static const char* UNIFORM_NAME_MVP_MATRIX;
    static const char* UNIFORM_NAME_EYE;
    static const char* UNIFORM_NAME_TIME;
    static const char* UNIFORM_NAME_SIN_TIME;
    static const char* UNIFORM_NAME_COS_TIME;
    static const char* UNIFORM_NAME_RANDOM01;
    static const char* UNIFORM_NAME_SAMPLER0;
    static const char* UNIFORM_NAME_SAMPLER1;
    static const char* UNIFORM_NAME_SAMPLER2;
    static const char* UNIFORM_NAME_SAMPLER3;
    static const char* UNIFORM_NAME_ALPHA_TEST_VALUE;

    // attribute names
    static const char* ATTRIBUTE_NAME_COLOR;
    static const char* ATTRIBUTE_NAME_POSITION;
    static const char* ATTRIBUTE_NAME_TEX_COORD;
    static const char* ATTRIBUTE_NAME_NORMAL;
	static const char* ATTRIBUTE_NAME_SIZE;

    GLuint m_program;
    GLuint m_vertShader;
    GLuint m_fragShader;

    struct flag_struct {
        unsigned int usesTime :1;
        unsigned int usesMVP :1;
        unsigned int usesMV :1;
        unsigned int usesMVInverse :1;
        unsigned int usesP :1;
        unsigned int usesRandom :1;
        unsigned int usesLighting :1;
        unsigned int usesEye :1;
        // handy way to initialize the bitfield
        flag_struct() {
            memset(this, 0, sizeof(*this));
        }
    } _flags;
    GLint _builtInUniforms[UNIFORM_MAX];

	typedef std::map<std::string, Uniform> UniformMap;
    UniformMap m_userUniforms;

	typedef std::map<std::string, VertexAttrib> VertexAttribMap;
    VertexAttribMap m_vertexAttribs;

    typedef struct {
        GLvoid* value;            // value
        unsigned int location;    // Key
    } UniformEntry;

    typedef std::vector<UniformEntry*> UniformEntryList;
    UniformEntryList m_uniformEntryList;

    DISABLE_COPY(Program)
};

#define ENABLE_ATTR_TYPEFLOAT(_Attr_Loc_, _Struct_, _Type_, _Member_) \
    GLHook::glEnableVertexAttribArray(_Attr_Loc_); \
    GLHook::glVertexAttribPointer(_Attr_Loc_, \
        sizeof(_Type_)/sizeof(float), \
        GL_FLOAT, \
        GL_FALSE, \
        sizeof(_Struct_), \
        (GLvoid*) offsetof(_Struct_, _Member_));


#define ENABLE_ATTR_POSITION(_Struct_, _Type_, _Member_) \
    ENABLE_ATTR_TYPEFLOAT(VERTEX_ATTRIB_POSITION, _Struct_, _Type_, _Member_)

#define ENABLE_ATTR_TEX_COORD(_Struct_, _Type_, _Member_) \
    ENABLE_ATTR_TYPEFLOAT(VERTEX_ATTRIB_TEX_COORD, _Struct_, _Type_, _Member_)

#define ENABLE_ATTR_NORMAL(_Struct_, _Type_, _Member_) \
    ENABLE_ATTR_TYPEFLOAT(VERTEX_ATTRIB_NORMAL, _Struct_, _Type_, _Member_)

#define ENABLE_ATTR_COLOR(_Struct_, _Type_, _Member_) \
    ENABLE_ATTR_TYPEFLOAT(VERTEX_ATTRIB_COLOR, _Struct_, _Type_, _Member_)

#endif /* PROGRAM_H */
