#include "GLFunction.h"

#ifdef __APPLE__
#else
#include <EGL/egl.h>
PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOES;
PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOES;
PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOES;
#endif

class GLStateChangeWrapper
{
protected:
    struct BlendFuncParams
    {
        BlendFuncParams();
        void reset();
        bool equal(GLenum sFactor, GLenum dFactor);
        GLenum d_sFactor, d_dFactor;
    };
    struct BlendFuncSeperateParams
    {
        BlendFuncSeperateParams();
        void reset();
        bool equal(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
        GLenum d_sfactorRGB, d_dfactorRGB, d_sfactorAlpha, d_dfactorAlpha;
    };
    struct PortParams
    {
        PortParams();
        void reset();
        bool equal(GLint x, GLint y, GLsizei width, GLsizei height);
        GLint d_x, d_y;
        GLsizei d_width, d_height;
    };
    struct BindBufferParams
    {
        BindBufferParams();
        void reset();
        bool equal(GLenum target, GLuint buffer);
        GLenum d_target;
        GLuint d_buffer;
    };

public:
    GLStateChangeWrapper();
    virtual ~GLStateChangeWrapper();

    void reset();
    void bindVertexArray(GLuint vertexArray);
    void blendFunc(GLenum sfactor, GLenum dfactor);
    void blendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
    void viewport(GLint x, GLint y, GLsizei width, GLsizei height);
    void scissor(GLint x, GLint y, GLsizei width, GLsizei height);
    void bindBuffer(GLenum target, GLuint buffer);

    GLuint                      d_vertexArrayObject;
    BlendFuncParams             d_blendFuncParams;
    BlendFuncSeperateParams     d_blendFuncSeperateParams;
    PortParams                  d_viewPortParams;
    PortParams                  d_scissorParams;
    BindBufferParams            d_bindBufferParams;
};

GLStateChangeWrapper::BlendFuncParams::BlendFuncParams()
{
    reset();
}

void GLStateChangeWrapper::BlendFuncParams::reset()
{
    d_dFactor = -1;
    d_sFactor = -1;
}

bool GLStateChangeWrapper::BlendFuncParams::equal(GLenum sFactor, GLenum dFactor)
{
    bool equal = (d_sFactor == sFactor) && (d_dFactor == dFactor);
    if(!equal)
    {
        d_sFactor = sFactor;
        d_dFactor = dFactor;
    }
    return equal;
}

GLStateChangeWrapper::BlendFuncSeperateParams::BlendFuncSeperateParams()
{
    reset();
}

void GLStateChangeWrapper::BlendFuncSeperateParams::reset()
{
    d_sfactorRGB = -1;
    d_dfactorRGB = -1;
    d_sfactorAlpha = -1;
    d_dfactorAlpha = -1;
}

bool GLStateChangeWrapper::BlendFuncSeperateParams::equal(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
    bool equal = (d_sfactorRGB == sfactorRGB) && (d_dfactorRGB == dfactorRGB) && (d_sfactorAlpha == sfactorAlpha) && (d_dfactorAlpha == dfactorAlpha);
    if(!equal)
    {
        d_sfactorRGB = sfactorRGB;
        d_dfactorRGB = dfactorRGB;
        d_sfactorAlpha = sfactorAlpha;
        d_dfactorAlpha = dfactorAlpha;
    }
    return equal;
}

GLStateChangeWrapper::PortParams::PortParams()
{
    reset();
}
void GLStateChangeWrapper::PortParams::reset()
{
    d_x = -1;
    d_y = -1;
    d_width = -1;
    d_height = -1;
}

bool GLStateChangeWrapper::PortParams::equal(GLint x, GLint y, GLsizei width, GLsizei height)
{
    bool equal = (d_x == x) && (d_y == y) && (d_width == width) && (d_height == height);
    if(!equal)
    {
        d_x = x;
        d_y = y;
        d_width = width;
        d_height = height;
    }
    return equal;
}

GLStateChangeWrapper::BindBufferParams::BindBufferParams()
{
    reset();
}
void GLStateChangeWrapper::BindBufferParams::reset()
{
    d_target = -1;
    d_buffer = -1;
}

bool GLStateChangeWrapper::BindBufferParams::equal(GLenum target, GLuint buffer)
{
    bool equal = (d_target == target) && (d_buffer == buffer);
    if(!equal)
    {
        d_target = target;
        d_buffer = buffer;
    }
    return equal;
}

GLStateChangeWrapper::GLStateChangeWrapper()
{
    reset();
}

GLStateChangeWrapper::~GLStateChangeWrapper()
{
}

void GLStateChangeWrapper::reset()
{
d_vertexArrayObject = -1;
d_blendFuncParams.reset();
d_blendFuncSeperateParams.reset();
d_viewPortParams.reset();
d_scissorParams.reset();
d_bindBufferParams.reset();
}

void GLStateChangeWrapper::bindVertexArray(GLuint vertexArray)
{
    if(vertexArray != d_vertexArrayObject)
    {
    //        glBindVertexArray(vertexArray);
        d_vertexArrayObject = vertexArray;
    }
}

void GLStateChangeWrapper::blendFunc(GLenum sfactor, GLenum dfactor)
{
    bool callIsRedundant = d_blendFuncParams.equal(sfactor, dfactor);
    if(!callIsRedundant)
    {
        glBlendFunc(sfactor, dfactor);
    }
}

void GLStateChangeWrapper::blendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
    bool callIsRedundant = d_blendFuncSeperateParams.equal(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
    if(!callIsRedundant)
        glBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
}

void GLStateChangeWrapper::viewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    bool callIsRedundant = d_viewPortParams.equal(x, y, width, height);
    if(!callIsRedundant)
        glViewport(x, y, width, height);
}

void GLStateChangeWrapper::scissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
    bool callIsRedundant = d_scissorParams.equal(x, y, width, height);
    if(!callIsRedundant)
        glScissor(x, y, width, height);
}
void GLStateChangeWrapper::bindBuffer(GLenum target, GLuint buffer)
{
    bool callIsRedundant = d_bindBufferParams.equal(target, buffer);
    if(!callIsRedundant)
        glBindBuffer(target, buffer);
}

//-------------------------------------------------------------//
GLStateChangeWrapper* GLFunctionHook::s_statechangewrapper = 0;

void GLFunctionHook::loadGL() {
    s_statechangewrapper = new GLStateChangeWrapper;
#ifdef __APPLE__
#else
     glBindVertexArrayOES = (PFNGLBINDVERTEXARRAYOESPROC) eglGetProcAddress("glBindVertexArrayOES");
     glDeleteVertexArraysOES = (PFNGLDELETEVERTEXARRAYSOESPROC) eglGetProcAddress("glDeleteVertexArraysOES");
     glGenVertexArraysOES = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress("glGenVertexArraysOES");
#endif
}

inline void GLFunctionHook::glScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
    GL_FUNCTION_DUMP
    s_statechangewrapper->scissor(x, y, width, height);
    // ::glScissor(x, y, width, height);
}

void GLFunctionHook::glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    GL_FUNCTION_DUMP
    s_statechangewrapper->viewport(x, y, width, height);
    // ::glViewport(x, y, width, height);
}

void GLFunctionHook::glBindBuffer(GLenum target, GLuint buffer)
{
    GL_FUNCTION_DUMP
    s_statechangewrapper->bindBuffer(target, buffer);
    // ::glBindBuffer(target, buffer);
}

void GLFunctionHook::glGenVertexArrays(GLsizei n, GLuint *ids)
{
    GL_FUNCTION_DUMP
#if TARGET_OS_MAC && !_GL_ES2_
    ::glGenVertexArraysAPPLE(n, ids);
#else
    ::glGenVertexArraysOES(n, ids);
#endif
}

void GLFunctionHook::glBindVertexArray(GLuint id)
{
    GL_FUNCTION_DUMP
#if TARGET_OS_MAC && !_GL_ES2_
    ::glBindVertexArrayAPPLE(id);
#else
    ::glBindVertexArrayOES(id);
#endif
}

void GLFunctionHook::glDeleteVertexArrays(GLsizei n, const GLuint *buffers)
{
    GL_FUNCTION_DUMP
#if TARGET_OS_MAC && !_GL_ES2_
    ::glDeleteVertexArraysAPPLE(n, buffers);
#else
    ::glDeleteVertexArraysOES(n, buffers);
#endif
}

/* EOF */

