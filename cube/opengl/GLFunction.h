#ifndef GLFUNCTION_HOOK_H
#define GLFUNCTION_HOOK_H

#include "Base.h"

#define GL_SILENCE_DEPRECATION

#if defined (__APPLE__)
    #if OS_MAC
        #include <OpenGL/gl.h>
        #include <OpenGL/glext.h>
    #else
        #include <OpenGLES/ES2/gl.h>
        #include <OpenGLES/ES2/glext.h>
    #endif
#elif OS_ANDROID
    #include <GLES/gl.h>
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
#else
    #include <GL/gl.h>
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
#endif

#ifndef GLHook
#define GLHook GLFunctionHook
#endif



#if 1
#define GL_FUNCTION_DUMP
#define GL_FUNCTION_DUMP_GLNUM(T)
#define GL_FUNCTION_DUMP_COUNT_1(FUCNAME)
#define GL_FUNCTION_DUMP_COUNT_2(FUCNAME,T1)
#define GL_FUNCTION_DUMP_TAG
#define GL_FUNCTION_DUMP_START GL_FUNCTION_DUMP_TAG
#define GL_FUNCTION_DUMP_END GL_FUNCTION_DUMP_TAG
#define GL_FUNCTION_DUMP_DRAWCMD GL_FUNCTION_DUMP
#else
#include <unistd.h>
#include <sys/time.h>
unsigned long __GetTickCount()
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return time.tv_sec * 1000 + time.tv_nsec / 1000000;
}

unsigned long __GetThreadTime()
{
    struct timespec time;
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &time);
    return time.tv_sec * 1000 + time.tv_nsec / 1000000;
}

static unsigned long s_tickcount = 0;
static unsigned long s_threadtime = 0;
static unsigned long s_drawcount = 0;
static unsigned long s_drawid = 0;
static unsigned long s_drawcmd = 0;

#define GL_FUNCTION_DUMP_GLNUM(T) GL_FUNCTION_DUMP
#define GL_FUNCTION_DUMP_COUNT_1(FUCNAME) GL_FUNCTION_DUMP
#define GL_FUNCTION_DUMP_COUNT_2(FUCNAME,T1) GL_FUNCTION_DUMP
#define GL_FUNCTION_DUMP  ++s_drawcount; // printf("\n |---[GLHOOK] Func:%s \n", __FUNCTION__);
#define GL_FUNCTION_DUMP_START { s_threadtime = __GetThreadTime(); s_tickcount = __GetTickCount(); s_drawcount = 0; s_drawcmd = 0; s_drawid = (s_drawid + 1)% 10000; printf("\n [GLHOOK] Render Begin [id:%d] +++ \n", s_drawid); }
#define GL_FUNCTION_DUMP_END { printf("\n [GLHOOK] Render End [id:%d][cmd:%d %d][%.3lf/%.3lf] +++ \n", s_drawid, s_drawcount, s_drawcmd, (__GetThreadTime() - s_threadtime)/1000.0f,  (__GetTickCount() - s_tickcount)/1000.0f); }
#define GL_FUNCTION_DUMP_DRAWCMD ++s_drawcmd;
#endif

class GLStateChangeWrapper;
class GLFunctionHook
{
public:
   static void loadGL();
   static void beginRenderFrame();
   static void endRenderFrame();
   static void glBindTexture(GLenum target, GLuint texture);
   static void glBlendFunc(GLenum sfactor, GLenum dfactor);
   static void glClear(GLbitfield mask);
   static void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
   static void glClearStencil(GLint s);
   static void glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
   static void glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
   static void glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
   static void glCullFace(GLenum mode);
   static void glDeleteTextures(GLsizei n, const GLuint* textures);
   static void glDepthFunc(GLenum func);
   static void glDepthMask(GLboolean flag);
   static void glDisable(GLenum cap);
   static void glDrawArrays(GLenum mode, GLint first, GLsizei count);
   static void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices);
   static void glEnable(GLenum cap);
   static void glFinish();
   static void glFlush();
   static void glFrontFace(GLenum mode);
   static void glGenTextures(GLsizei n, GLuint* textures);
   static void glGetBooleanv(GLenum pname, GLboolean* params);
   static GLenum glGetError();
   static void glGetFloatv(GLenum pname, GLfloat* params);
   static void glGetIntegerv(GLenum pname, GLint* params);
   static const GLubyte *glGetString(GLenum name);
   static void glGetTexParameterfv(GLenum target, GLenum pname, GLfloat* params);
   static void glGetTexParameteriv(GLenum target, GLenum pname, GLint* params);
   static void glHint(GLenum target, GLenum mode);
   static GLboolean glIsEnabled(GLenum cap);
   static GLboolean glIsTexture(GLuint texture);
   static void glLineWidth(GLfloat width);
   static void glPixelStorei(GLenum pname, GLint param);
   static void glPolygonOffset(GLfloat factor, GLfloat units);
   static void glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels);
   static void glScissor(GLint x, GLint y, GLsizei width, GLsizei height);
   static void glStencilFunc(GLenum func, GLint ref, GLuint mask);
   static void glStencilMask(GLuint mask);
   static void glStencilOp(GLenum fail, GLenum zfail, GLenum zpass);
   static void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
   static void glTexParameterf(GLenum target, GLenum pname, GLfloat param);
   static void glTexParameterfv(GLenum target, GLenum pname, const GLfloat* params);
   static void glTexParameteri(GLenum target, GLenum pname, GLint param);
   static void glTexParameteriv(GLenum target, GLenum pname, const GLint* params);
   static void glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels);
   static void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);

   static void glActiveTexture(GLenum texture);
   static void glAttachShader(GLuint program, GLuint shader);
   static void glBindAttribLocation(GLuint program, GLuint index, const char* name);
   static void glBindBuffer(GLenum target, GLuint buffer);
   static void glBindFramebuffer(GLenum target, GLuint framebuffer);
   static void glBindRenderbuffer(GLenum target, GLuint renderbuffer);
   static void glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
   static void glBlendEquation(GLenum mode);
   static void glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha);
   static void glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
   static void glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
   static void glBufferSubData(GLenum target, GLsizeiptr offset, GLsizeiptr size, const void* data);
   static GLenum glCheckFramebufferStatus(GLenum target);
   static void glClearDepthf(GLclampf depth);
   static void glCompileShader(GLuint shader);
   static void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
   static void glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
   static GLuint glCreateProgram();
   static GLuint glCreateShader(GLenum type);
   static void glDeleteBuffers(GLsizei n, const GLuint* buffers);
   static void glDeleteFramebuffers(GLsizei n, const GLuint* framebuffers);
   static void glDeleteProgram(GLuint program);
   static void glDeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers);
   static void glDeleteShader(GLuint shader);
   static void glDepthRangef(GLclampf zNear, GLclampf zFar);
   static void glDetachShader(GLuint program, GLuint shader);
   static void glDisableVertexAttribArray(GLuint index);
   static void glEnableVertexAttribArray(GLuint index);
   static void glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
   static void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
   static void glGenBuffers(GLsizei n, GLuint* buffers);
   static void glGenerateMipmap(GLenum target);
   static void glGenFramebuffers(GLsizei n, GLuint* framebuffers);
   static void glGenRenderbuffers(GLsizei n, GLuint* renderbuffers);
   static void glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);
   static void glGetActiveUniform(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);
   static void glGetAttachedShaders(GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders);
   static GLint glGetAttribLocation(GLuint program, const char* name);
   static void glGetBufferParameteriv(GLenum target, GLenum pname, GLint* params);
   static void glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint* params);
   static void glGetProgramiv(GLuint program, GLenum pname, GLint* params);
   static void glGetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei* length, char* infolog);
   static void glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint* params);
   static void glGetShaderiv(GLuint shader, GLenum pname, GLint* params);
   static void glGetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog);
   static void glGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision);
   static void glGetShaderSource(GLuint shader, GLsizei bufsize, GLsizei* length, char* source);
   static void glGetUniformfv(GLuint program, GLint location, GLfloat* params);
   static void glGetUniformiv(GLuint program, GLint location, GLint* params);
   static GLint glGetUniformLocation(GLuint program, const char* name);
   static void glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat* params);
   static void glGetVertexAttribiv(GLuint index, GLenum pname, GLint* params);
   static void glGetVertexAttribPointerv(GLuint index, GLenum pname, void** pointer);
   static GLboolean glIsBuffer(GLuint buffer);
   static GLboolean glIsFramebuffer(GLuint framebuffer);
   static GLboolean glIsProgram(GLuint program);
   static GLboolean glIsRenderbuffer(GLuint renderbuffer);
   static GLboolean glIsShader(GLuint shader);
   static void glLinkProgram(GLuint program);
   static void glReleaseShaderCompiler();
   static void glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
   static void glSampleCoverage(GLclampf value, GLboolean invert);
   static void glShaderBinary(GLint n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLint length);
   static void glShaderSource(GLuint shader, GLsizei count, const char** string, const GLint* length);
   static void glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask);
   static void glStencilMaskSeparate(GLenum face, GLuint mask);
   static void glStencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass);
   static void glUniform1f(GLint location, GLfloat x);
   static void glUniform1fv(GLint location, GLsizei count, const GLfloat* v);
   static void glUniform1i(GLint location, GLint x);
   static void glUniform1iv(GLint location, GLsizei count, const GLint* v);
   static void glUniform2f(GLint location, GLfloat x, GLfloat y);
   static void glUniform2fv(GLint location, GLsizei count, const GLfloat* v);
   static void glUniform2i(GLint location, GLint x, GLint y);
   static void glUniform2iv(GLint location, GLsizei count, const GLint* v);
   static void glUniform3f(GLint location, GLfloat x, GLfloat y, GLfloat z);
   static void glUniform3fv(GLint location, GLsizei count, const GLfloat* v);
   static void glUniform3i(GLint location, GLint x, GLint y, GLint z);
   static void glUniform3iv(GLint location, GLsizei count, const GLint* v);
   static void glUniform4f(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
   static void glUniform4fv(GLint location, GLsizei count, const GLfloat* v);
   static void glUniform4i(GLint location, GLint x, GLint y, GLint z, GLint w);
   static void glUniform4iv(GLint location, GLsizei count, const GLint* v);
   static void glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
   static void glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
   static void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
   static void glUseProgram(GLuint program);
   static void glValidateProgram(GLuint program);
   static void glVertexAttrib1f(GLuint indx, GLfloat x);
   static void glVertexAttrib1fv(GLuint indx, const GLfloat* values);
   static void glVertexAttrib2f(GLuint indx, GLfloat x, GLfloat y);
   static void glVertexAttrib2fv(GLuint indx, const GLfloat* values);
   static void glVertexAttrib3f(GLuint indx, GLfloat x, GLfloat y, GLfloat z);
   static void glVertexAttrib3fv(GLuint indx, const GLfloat* values);
   static void glVertexAttrib4f(GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
   static void glVertexAttrib4fv(GLuint indx, const GLfloat* values);
   static void glVertexAttribPointer(GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr);
   static void glGenVertexArrays(GLsizei n, GLuint *ids);
   static void glBindVertexArray(GLuint id);
   static void glDeleteVertexArrays(GLsizei n, const GLuint *buffers);

    struct GLFunctionCount
    {
        int glDrawArrays;
        int glDrawElements;
        int glBindBuffer;
        int glBlendFunc;
        int glBindTexture;
    };
    static GLStateChangeWrapper* s_statechangewrapper;
};



inline void GLFunctionHook::beginRenderFrame(){
    GL_FUNCTION_DUMP_START
    // memset(&glCount, 0, sizeof(GLFunctionCount));
}

inline void GLFunctionHook::endRenderFrame(){
    GL_FUNCTION_DUMP_END
}

inline void GLFunctionHook::glBindTexture(GLenum target, GLuint texture)
{
    GL_FUNCTION_DUMP_GLNUM(texture)
    GL_FUNCTION_DUMP_COUNT_1(BindTexture)
    ::glBindTexture(target, texture);
}

inline void GLFunctionHook::glBlendFunc(GLenum sfactor, GLenum dfactor)
{
    GL_FUNCTION_DUMP_COUNT_1(BlendFunc)
    ::glBlendFunc(sfactor, dfactor);
}

inline void GLFunctionHook::glClear(GLbitfield mask)
{
    GL_FUNCTION_DUMP
    ::glClear(mask);
}

inline void GLFunctionHook::glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
    GL_FUNCTION_DUMP
    ::glClearColor(red, green, blue, alpha);
}

inline void GLFunctionHook::glClearStencil(GLint s)
{
    GL_FUNCTION_DUMP
    ::glClearStencil(s);

}

inline void GLFunctionHook::glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{

    GL_FUNCTION_DUMP
    ::glColorMask(red, green, blue, alpha);
}

inline void GLFunctionHook::glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
    GL_FUNCTION_DUMP
    ::glCopyTexImage2D(target, level, internalformat, x, y, width, height, border);

}

inline void GLFunctionHook::glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
    GL_FUNCTION_DUMP
    ::glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
}

inline void GLFunctionHook::glCullFace(GLenum mode)
{
    GL_FUNCTION_DUMP
    ::glCullFace(mode);
}

inline void GLFunctionHook::glDeleteTextures(GLsizei n, const GLuint *textures)
{
    GL_FUNCTION_DUMP
    ::glDeleteTextures(n, textures);
}

inline void GLFunctionHook::glDepthFunc(GLenum func)
{
    GL_FUNCTION_DUMP
    ::glDepthFunc(func);
}

inline void GLFunctionHook::glDepthMask(GLboolean flag)
{
    GL_FUNCTION_DUMP
    ::glDepthMask(flag);
}

inline void GLFunctionHook::glDisable(GLenum cap)
{
    GL_FUNCTION_DUMP_GLNUM(cap)
    ::glDisable(cap);

}

inline void GLFunctionHook::glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
    GL_FUNCTION_DUMP_COUNT_2(DrawArrays,count)
    GL_FUNCTION_DUMP_DRAWCMD
    ::glDrawArrays(mode, first, count);
}

inline void GLFunctionHook::glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
    GL_FUNCTION_DUMP_COUNT_2(DrawElements,count)
    GL_FUNCTION_DUMP_DRAWCMD
    ::glDrawElements(mode, count, type, indices);
}

inline void GLFunctionHook::glEnable(GLenum cap)
{
    GL_FUNCTION_DUMP_GLNUM(cap)
    ::glEnable(cap);
}

inline void GLFunctionHook::glFinish()
{
    GL_FUNCTION_DUMP
    ::glFinish();
}

inline void GLFunctionHook::glFlush()
{
    GL_FUNCTION_DUMP
    ::glFlush();
}

inline void GLFunctionHook::glFrontFace(GLenum mode)
{
    GL_FUNCTION_DUMP
    ::glFrontFace(mode);
}

inline void GLFunctionHook::glGenTextures(GLsizei n, GLuint *textures)
{
    GL_FUNCTION_DUMP
    ::glGenTextures(n, textures);
}

inline void GLFunctionHook::glGetBooleanv(GLenum pname, GLboolean *params)
{
    GL_FUNCTION_DUMP
    ::glGetBooleanv(pname, params);
}

inline GLenum GLFunctionHook::glGetError()
{
    GL_FUNCTION_DUMP
    return ::glGetError();
}

inline void GLFunctionHook::glGetFloatv(GLenum pname, GLfloat *params)
{
    GL_FUNCTION_DUMP
    ::glGetFloatv(pname, params);
}

inline void GLFunctionHook::glGetIntegerv(GLenum pname, GLint *params)
{
    GL_FUNCTION_DUMP
    ::glGetIntegerv(pname, params);

}

inline const GLubyte *GLFunctionHook::glGetString(GLenum name)
{
    GL_FUNCTION_DUMP
    return ::glGetString(name);
}

inline void GLFunctionHook::glGetTexParameterfv(GLenum target, GLenum pname, GLfloat *params)
{
    GL_FUNCTION_DUMP
    ::glGetTexParameterfv(target, pname, params);
}

inline void GLFunctionHook::glGetTexParameteriv(GLenum target, GLenum pname, GLint *params)
{
    GL_FUNCTION_DUMP
    ::glGetTexParameteriv(target, pname, params);
}

inline void GLFunctionHook::glHint(GLenum target, GLenum mode)
{
    GL_FUNCTION_DUMP
    ::glHint(target, mode);
}

inline GLboolean GLFunctionHook::glIsEnabled(GLenum cap)
{
    GL_FUNCTION_DUMP
    return ::glIsEnabled(cap);
}

inline GLboolean GLFunctionHook::glIsTexture(GLuint texture)
{
    GL_FUNCTION_DUMP
    return ::glIsTexture(texture);
}

inline void GLFunctionHook::glLineWidth(GLfloat width)
{
    GL_FUNCTION_DUMP
    ::glLineWidth(width);
}

inline void GLFunctionHook::glPixelStorei(GLenum pname, GLint param)
{
    GL_FUNCTION_DUMP
    ::glPixelStorei(pname, param);
}

inline void GLFunctionHook::glPolygonOffset(GLfloat factor, GLfloat units)
{
    GL_FUNCTION_DUMP
    ::glPolygonOffset(factor, units);
}

inline void GLFunctionHook::glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels)
{
    GL_FUNCTION_DUMP
    ::glReadPixels(x, y, width, height, format, type, pixels);
}

inline void GLFunctionHook::glStencilFunc(GLenum func, GLint ref, GLuint mask)
{
    GL_FUNCTION_DUMP
    ::glStencilFunc(func, ref, mask);
}

inline void GLFunctionHook::glStencilMask(GLuint mask)
{
    GL_FUNCTION_DUMP
    ::glStencilMask(mask);
}

inline void GLFunctionHook::glStencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
    GL_FUNCTION_DUMP
    ::glStencilOp(fail, zfail, zpass);
}

inline void GLFunctionHook::glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
    GL_FUNCTION_DUMP
    ::glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
}

inline void GLFunctionHook::glTexParameterf(GLenum target, GLenum pname, GLfloat param)
{
    GL_FUNCTION_DUMP
    ::glTexParameterf(target, pname, param);
}

inline void GLFunctionHook::glTexParameterfv(GLenum target, GLenum pname, const GLfloat *params)
{
    GL_FUNCTION_DUMP
    ::glTexParameterfv(target, pname, params);
}

inline void GLFunctionHook::glTexParameteri(GLenum target, GLenum pname, GLint param)
{
    GL_FUNCTION_DUMP
    ::glTexParameteri(target, pname, param);
}

inline void GLFunctionHook::glTexParameteriv(GLenum target, GLenum pname, const GLint *params)
{
    GL_FUNCTION_DUMP
    ::glTexParameteriv(target, pname, params);
}

inline void GLFunctionHook::glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{
    GL_FUNCTION_DUMP
    ::glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
}

inline void GLFunctionHook::glActiveTexture(GLenum texture)
{
    GL_FUNCTION_DUMP
    ::glActiveTexture(texture);
}

inline void GLFunctionHook::glAttachShader(GLuint program, GLuint shader)
{
    GL_FUNCTION_DUMP
    ::glAttachShader(program, shader);
}

inline void GLFunctionHook::glBindAttribLocation(GLuint program, GLuint index, const char *name)
{
    GL_FUNCTION_DUMP
    ::glBindAttribLocation(program, index, name);
}

inline void GLFunctionHook::glBindFramebuffer(GLenum target, GLuint framebuffer)
{
    GL_FUNCTION_DUMP
    ::glBindFramebuffer(target, framebuffer);
}

inline void GLFunctionHook::glBindRenderbuffer(GLenum target, GLuint renderbuffer)
{
    GL_FUNCTION_DUMP
    ::glBindRenderbuffer(target, renderbuffer);
}

inline void GLFunctionHook::glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
    GL_FUNCTION_DUMP
    ::glBlendColor(red, green, blue, alpha);
}

inline void GLFunctionHook::glBlendEquation(GLenum mode)
{
    GL_FUNCTION_DUMP
    ::glBlendEquation(mode);
}

inline void GLFunctionHook::glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
{
    GL_FUNCTION_DUMP
    ::glBlendEquationSeparate(modeRGB, modeAlpha);
}

inline void GLFunctionHook::glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
{
    GL_FUNCTION_DUMP
    ::glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
}

inline void GLFunctionHook::glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage)
{
    if(size == 0)
        return;
    GL_FUNCTION_DUMP
    ::glBufferData(target, size, data, usage);
}

inline void GLFunctionHook::glBufferSubData(GLenum target, GLsizeiptr offset, GLsizeiptr size, const void *data)
{
    if(size == 0)
        return;
    GL_FUNCTION_DUMP
    ::glBufferSubData(target, offset, size, data);
}

inline GLenum GLFunctionHook::glCheckFramebufferStatus(GLenum target)
{
    GL_FUNCTION_DUMP
    return ::glCheckFramebufferStatus(target);
}

inline void GLFunctionHook::glClearDepthf(GLclampf depth)
{
    GL_FUNCTION_DUMP
#if TARGET_OS_MAC && !_GL_ES2_
    ::glClearDepth(depth);
#else
    ::glClearDepthf(depth);
#endif
}

inline void GLFunctionHook::glCompileShader(GLuint shader)
{
    GL_FUNCTION_DUMP
    ::glCompileShader(shader);
}

inline void GLFunctionHook::glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data)
{
    GL_FUNCTION_DUMP
    ::glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
}

inline void GLFunctionHook::glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data)
{
    GL_FUNCTION_DUMP
    ::glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
}

inline GLuint GLFunctionHook::glCreateProgram()
{
    GL_FUNCTION_DUMP
    return ::glCreateProgram();
}

inline GLuint GLFunctionHook::glCreateShader(GLenum type)
{
    GL_FUNCTION_DUMP
    return ::glCreateShader(type);
}

inline void GLFunctionHook::glDeleteBuffers(GLsizei n, const GLuint *buffers)
{
    GL_FUNCTION_DUMP
    ::glDeleteBuffers(n, buffers);
}

inline void GLFunctionHook::glDeleteFramebuffers(GLsizei n, const GLuint *framebuffers)
{
    GL_FUNCTION_DUMP
    ::glDeleteFramebuffers(n, framebuffers);
}

inline void GLFunctionHook::glDeleteProgram(GLuint program)
{
    GL_FUNCTION_DUMP
    ::glDeleteProgram(program);
}

inline void GLFunctionHook::glDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers)
{
    GL_FUNCTION_DUMP
    ::glDeleteRenderbuffers(n, renderbuffers);
}

inline void GLFunctionHook::glDeleteShader(GLuint shader)
{
    GL_FUNCTION_DUMP
    ::glDeleteShader(shader);
}

inline void GLFunctionHook::glDepthRangef(GLclampf zNear, GLclampf zFar)
{
    GL_FUNCTION_DUMP
#if TARGET_OS_MAC && !_GL_ES2_
    ::glDepthRange(zNear, zFar);
#else
    ::glDepthRangef(zNear, zFar);
#endif
}

inline void GLFunctionHook::glDetachShader(GLuint program, GLuint shader)
{
    GL_FUNCTION_DUMP
    ::glDetachShader(program, shader);
}

inline void GLFunctionHook::glDisableVertexAttribArray(GLuint index)
{
    GL_FUNCTION_DUMP
    ::glDisableVertexAttribArray(index);
}

inline void GLFunctionHook::glEnableVertexAttribArray(GLuint index)
{
    GL_FUNCTION_DUMP
    ::glEnableVertexAttribArray(index);
}

inline void GLFunctionHook::glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
    GL_FUNCTION_DUMP
    ::glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
}

inline void GLFunctionHook::glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
    GL_FUNCTION_DUMP
    ::glFramebufferTexture2D(target, attachment, textarget, texture, level);
}

inline void GLFunctionHook::glGenBuffers(GLsizei n, GLuint *buffers)
{
    GL_FUNCTION_DUMP
    ::glGenBuffers(n, buffers);
}

inline void GLFunctionHook::glGenerateMipmap(GLenum target)
{
    GL_FUNCTION_DUMP
    ::glGenerateMipmap(target);
}

inline void GLFunctionHook::glGenFramebuffers(GLsizei n, GLuint *framebuffers)
{
    GL_FUNCTION_DUMP
    ::glGenFramebuffers(n, framebuffers);
}

inline void GLFunctionHook::glGenRenderbuffers(GLsizei n, GLuint *renderbuffers)
{
    GL_FUNCTION_DUMP
    ::glGenRenderbuffers(n, renderbuffers);
}

inline void GLFunctionHook::glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufsize, GLsizei *length, GLint *size, GLenum *type, char *name)
{
    GL_FUNCTION_DUMP
    ::glGetActiveAttrib(program, index, bufsize, length, size, type, name);
}

inline void GLFunctionHook::glGetActiveUniform(GLuint program, GLuint index, GLsizei bufsize, GLsizei *length, GLint *size, GLenum *type, char *name)
{
    GL_FUNCTION_DUMP
    ::glGetActiveUniform(program, index, bufsize, length, size, type, name);
}

inline void GLFunctionHook::glGetAttachedShaders(GLuint program, GLsizei maxcount, GLsizei *count, GLuint *shaders)
{
    GL_FUNCTION_DUMP
    ::glGetAttachedShaders(program, maxcount, count, shaders);
}

inline GLint GLFunctionHook::glGetAttribLocation(GLuint program, const char *name)
{
    GL_FUNCTION_DUMP
    return ::glGetAttribLocation(program, name);
}

inline void GLFunctionHook::glGetBufferParameteriv(GLenum target, GLenum pname, GLint *params)
{
    GL_FUNCTION_DUMP
    ::glGetBufferParameteriv(target, pname, params);
}

inline void GLFunctionHook::glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params)
{
    GL_FUNCTION_DUMP
    ::glGetFramebufferAttachmentParameteriv(target, attachment, pname, params);
}

inline void GLFunctionHook::glGetProgramiv(GLuint program, GLenum pname, GLint *params)
{
    GL_FUNCTION_DUMP
    ::glGetProgramiv(program, pname, params);
}

inline void GLFunctionHook::glGetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei *length, char *infolog)
{
    GL_FUNCTION_DUMP
    ::glGetProgramInfoLog(program, bufsize, length, infolog);
}

inline void GLFunctionHook::glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params)
{
    GL_FUNCTION_DUMP
    ::glGetRenderbufferParameteriv(target, pname, params);
}

inline void GLFunctionHook::glGetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
    GL_FUNCTION_DUMP
    ::glGetShaderiv(shader, pname, params);
}

inline void GLFunctionHook::glGetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei *length, char *infolog)
{
    GL_FUNCTION_DUMP
    ::glGetShaderInfoLog(shader, bufsize, length, infolog);
}

inline void GLFunctionHook::glGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision)
{
    GL_FUNCTION_DUMP
    // ::glGetShaderPrecisionFormat(shadertype, precisiontype, range, precision);
}

inline void GLFunctionHook::glGetShaderSource(GLuint shader, GLsizei bufsize, GLsizei *length, char *source)
{
    GL_FUNCTION_DUMP
    ::glGetShaderSource(shader, bufsize, length, source);
}

inline void GLFunctionHook::glGetUniformfv(GLuint program, GLint location, GLfloat *params)
{
    GL_FUNCTION_DUMP
    ::glGetUniformfv(program, location, params);
}

inline void GLFunctionHook::glGetUniformiv(GLuint program, GLint location, GLint *params)
{
    GL_FUNCTION_DUMP
    ::glGetUniformiv(program, location, params);
}

inline GLint GLFunctionHook::glGetUniformLocation(GLuint program, const char *name)
{
    GL_FUNCTION_DUMP
    return ::glGetUniformLocation(program, name);
}

inline void GLFunctionHook::glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params)
{
    GL_FUNCTION_DUMP
    ::glGetVertexAttribfv(index, pname, params);
}

inline void GLFunctionHook::glGetVertexAttribiv(GLuint index, GLenum pname, GLint *params)
{
    GL_FUNCTION_DUMP
    ::glGetVertexAttribiv(index, pname, params);
}

inline void GLFunctionHook::glGetVertexAttribPointerv(GLuint index, GLenum pname, void **pointer)
{
    GL_FUNCTION_DUMP
    ::glGetVertexAttribPointerv(index, pname, pointer);
}

inline GLboolean GLFunctionHook::glIsBuffer(GLuint buffer)
{
    GL_FUNCTION_DUMP
    return ::glIsBuffer(buffer);
}

inline GLboolean GLFunctionHook::glIsFramebuffer(GLuint framebuffer)
{
    GL_FUNCTION_DUMP
    return ::glIsFramebuffer(framebuffer);
}

inline GLboolean GLFunctionHook::glIsProgram(GLuint program)
{
    GL_FUNCTION_DUMP
    return ::glIsProgram(program);
}

inline GLboolean GLFunctionHook::glIsRenderbuffer(GLuint renderbuffer)
{
    GL_FUNCTION_DUMP
    return ::glIsRenderbuffer(renderbuffer);
}

inline GLboolean GLFunctionHook::glIsShader(GLuint shader)
{
    GL_FUNCTION_DUMP
    return ::glIsShader(shader);
}

inline void GLFunctionHook::glLinkProgram(GLuint program)
{
    GL_FUNCTION_DUMP
    ::glLinkProgram(program);
}

inline void GLFunctionHook::glReleaseShaderCompiler()
{
    GL_FUNCTION_DUMP
#if TARGET_OS_MAC && !_GL_ES2_
#else
    ::glReleaseShaderCompiler();
#endif
}

inline void GLFunctionHook::glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
    GL_FUNCTION_DUMP
    ::glRenderbufferStorage(target, internalformat, width, height);
}

inline void GLFunctionHook::glSampleCoverage(GLclampf value, GLboolean invert)
{
    GL_FUNCTION_DUMP
    ::glSampleCoverage(value, invert);
}

inline void GLFunctionHook::glShaderBinary(GLint n, const GLuint *shaders, GLenum binaryformat, const void *binary, GLint length)
{
    GL_FUNCTION_DUMP
#if TARGET_OS_MAC && !_GL_ES2_
#else
    ::glShaderBinary(n, shaders, binaryformat, binary, length);
#endif
}

inline void GLFunctionHook::glShaderSource(GLuint shader, GLsizei count, const char **string, const GLint *length)
{
    GL_FUNCTION_DUMP
    ::glShaderSource(shader, count, string, length);
}

inline void GLFunctionHook::glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
{
    GL_FUNCTION_DUMP
    ::glStencilFuncSeparate(face, func, ref, mask);
}

inline void GLFunctionHook::glStencilMaskSeparate(GLenum face, GLuint mask)
{
    GL_FUNCTION_DUMP
    ::glStencilMaskSeparate(face, mask);
}

inline void GLFunctionHook::glStencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass)
{
    GL_FUNCTION_DUMP
    ::glStencilOpSeparate(face, fail, zfail, zpass);
}

inline void GLFunctionHook::glUniform1f(GLint location, GLfloat x)
{
    GL_FUNCTION_DUMP
    ::glUniform1f(location, x);
}

inline void GLFunctionHook::glUniform1fv(GLint location, GLsizei count, const GLfloat *v)
{
    GL_FUNCTION_DUMP
    ::glUniform1fv(location, count, v);
}

inline void GLFunctionHook::glUniform1i(GLint location, GLint x)
{
    GL_FUNCTION_DUMP
    ::glUniform1i(location, x);
}

inline void GLFunctionHook::glUniform1iv(GLint location, GLsizei count, const GLint *v)
{
    GL_FUNCTION_DUMP
    ::glUniform1iv(location, count, v);
}

inline void GLFunctionHook::glUniform2f(GLint location, GLfloat x, GLfloat y)
{
    GL_FUNCTION_DUMP
    ::glUniform2f(location, x, y);
}

inline void GLFunctionHook::glUniform2fv(GLint location, GLsizei count, const GLfloat *v)
{
    GL_FUNCTION_DUMP
    ::glUniform2fv(location, count, v);
}

inline void GLFunctionHook::glUniform2i(GLint location, GLint x, GLint y)
{
    GL_FUNCTION_DUMP
    ::glUniform2i(location, x, y);
}

inline void GLFunctionHook::glUniform2iv(GLint location, GLsizei count, const GLint *v)
{
    GL_FUNCTION_DUMP
    ::glUniform2iv(location, count, v);
}

inline void GLFunctionHook::glUniform3f(GLint location, GLfloat x, GLfloat y, GLfloat z)
{
    GL_FUNCTION_DUMP
    ::glUniform3f(location, x, y, z);
}

inline void GLFunctionHook::glUniform3fv(GLint location, GLsizei count, const GLfloat *v)
{
    GL_FUNCTION_DUMP
    ::glUniform3fv(location, count, v);
}

inline void GLFunctionHook::glUniform3i(GLint location, GLint x, GLint y, GLint z)
{
    GL_FUNCTION_DUMP
    ::glUniform3i(location, x, y, z);
}

inline void GLFunctionHook::glUniform3iv(GLint location, GLsizei count, const GLint *v)
{
    GL_FUNCTION_DUMP
    ::glUniform3iv(location, count, v);
}

inline void GLFunctionHook::glUniform4f(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    GL_FUNCTION_DUMP
    ::glUniform4f(location, x, y, z, w);
}

inline void GLFunctionHook::glUniform4fv(GLint location, GLsizei count, const GLfloat *v)
{
    GL_FUNCTION_DUMP
    ::glUniform4fv(location, count, v);
}

inline void GLFunctionHook::glUniform4i(GLint location, GLint x, GLint y, GLint z, GLint w)
{
    GL_FUNCTION_DUMP
    ::glUniform4i(location, x, y, z, w);
}

inline void GLFunctionHook::glUniform4iv(GLint location, GLsizei count, const GLint *v)
{
    GL_FUNCTION_DUMP
    ::glUniform4iv(location, count, v);
}

inline void GLFunctionHook::glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    GL_FUNCTION_DUMP
    ::glUniformMatrix2fv(location, count, transpose, value);
}

inline void GLFunctionHook::glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    GL_FUNCTION_DUMP
    ::glUniformMatrix3fv(location, count, transpose, value);
}

inline void GLFunctionHook::glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    GL_FUNCTION_DUMP
    ::glUniformMatrix4fv(location, count, transpose, value);
}

inline void GLFunctionHook::glUseProgram(GLuint program)
{
    GL_FUNCTION_DUMP
    ::glUseProgram(program);
}

inline void GLFunctionHook::glValidateProgram(GLuint program)
{
    GL_FUNCTION_DUMP
    ::glValidateProgram(program);
}

inline void GLFunctionHook::glVertexAttrib1f(GLuint indx, GLfloat x)
{
    GL_FUNCTION_DUMP
    ::glVertexAttrib1f(indx, x);
}

inline void GLFunctionHook::glVertexAttrib1fv(GLuint indx, const GLfloat *values)
{
    GL_FUNCTION_DUMP
    ::glVertexAttrib1fv(indx, values);
}

inline void GLFunctionHook::glVertexAttrib2f(GLuint indx, GLfloat x, GLfloat y)
{
    GL_FUNCTION_DUMP
    ::glVertexAttrib2f(indx, x, y);
}

inline void GLFunctionHook::glVertexAttrib2fv(GLuint indx, const GLfloat *values)
{
    GL_FUNCTION_DUMP
    ::glVertexAttrib2fv(indx, values);
}

inline void GLFunctionHook::glVertexAttrib3f(GLuint indx, GLfloat x, GLfloat y, GLfloat z)
{
    GL_FUNCTION_DUMP
    ::glVertexAttrib3f(indx, x, y, z);
}

inline void GLFunctionHook::glVertexAttrib3fv(GLuint indx, const GLfloat *values)
{
    GL_FUNCTION_DUMP
    ::glVertexAttrib3fv(indx, values);
}

inline void GLFunctionHook::glVertexAttrib4f(GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    GL_FUNCTION_DUMP
    ::glVertexAttrib4f(indx, x, y, z, w);
}

inline void GLFunctionHook::glVertexAttrib4fv(GLuint indx, const GLfloat *values)
{
    GL_FUNCTION_DUMP
    ::glVertexAttrib4fv(indx, values);
}

inline void GLFunctionHook::glVertexAttribPointer(GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *ptr)
{
    GL_FUNCTION_DUMP
    ::glVertexAttribPointer(indx, size, type, normalized, stride, ptr);
}

#endif // GLFUNCTION_H
