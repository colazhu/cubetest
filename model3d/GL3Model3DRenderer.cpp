#include "GL3Model3DRenderer.h"
#include "glm_3d/glm/glm.hpp"
#include "glm_3d/glm/gtc/matrix_transform.hpp"
#include "glm_3d/glm/gtc/type_ptr.hpp"
#include "assimp/material.h"
#include "GL3Renderer.h"
#include "Model3DShaderFrag.h"
#include "Model3DShaderVert.h"
#include "Shader.h"
#include "Texture.h"
#include "CEGUI/System.h"
#include "CEGUI/Model3DManager.h"
#include "CEGUI/Colour.h"
#include "CEGUI/OSTimeUtils.h"
#include <iostream>
#include <sstream>
#include <iostream>
#include <stdio.h>

#if __APPLE__
#if TARGET_OS_MAC
#define GL_VERTEX_ARRAY_BINDING GL_VERTEX_ARRAY_BINDING_APPLE
#define GL_DRAW_FRAMEBUFFER GL_DRAW_FRAMEBUFFER_EXT
#define GL_READ_FRAMEBUFFER GL_READ_FRAMEBUFFER_EXT
#define GL_RGBA8 GL_RGBA
#define GL_RGB16F GL_RGB16F_ARB
#define GL_RGBA16F GL_RGBA16F_ARB
#define FRAMEBUFFERTEXTURE2D(a, b, c, d, e, f) \
            GLHook::FramebufferTexture2D(a, b, c, d, e)
#define RENDERBUFFERSTORAGEMULTISAMPLE(a, b, c, d, e) \
            glRenderbufferStorageMultisampleEXT(a, b, c, d, e)
#define RESOLVEMULTISAMPLEFRAMEBUFFER(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter) \
            glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
#define GL_MAX_SAMPLES GL_MAX_SAMPLES_EXT
#else
#define GL_VERTEX_ARRAY_BINDING GL_VERTEX_ARRAY_BINDING_OES
#define GL_DRAW_FRAMEBUFFER GL_DRAW_FRAMEBUFFER_APPLE
#define GL_READ_FRAMEBUFFER GL_READ_FRAMEBUFFER_APPLE
#define GL_RGBA8 GL_RGBA8_OES
#define GL_RGB16F GL_RGB16F_EXT
#define GL_RGBA16F GL_RGBA16F_EXT
#define FRAMEBUFFERTEXTURE2D(a, b, c, d, e, f) \
            GLHook::FramebufferTexture2D(a, b, c, d, e)
#define RENDERBUFFERSTORAGEMULTISAMPLE(a, b, c, d, e) \
            glRenderbufferStorageMultisampleAPPLE(a, b, c, d, e)
#define RESOLVEMULTISAMPLEFRAMEBUFFER(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter) \
            glResolveMultisampleFramebufferAPPLE();
#define GL_MAX_SAMPLES GL_MAX_SAMPLES_APPLE
#endif
#else
#include <EGL/egl.h>
#define GL_VERTEX_ARRAY_BINDING GL_VERTEX_ARRAY_BINDING_OES
#define GL_RGB16F GL_RGB16F_EXT
#define GL_RGBA16F GL_RGBA16F_EXT
#define FRAMEBUFFERTEXTURE2D(a, b, c, d, e, f) \
            ((PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC)eglGetProcAddress("glFramebufferTexture2DMultisampleEXT"))(a, b, c, d, e, f)
#define RENDERBUFFERSTORAGEMULTISAMPLE(a, b, c, d, e) \
            ((PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC)eglGetProcAddress("glRenderbufferStorageMultisampleEXT"))(a, b, c, d, e)
#define GL_MAX_SAMPLES GL_MAX_SAMPLES_EXT
#endif

namespace CEGUI
{
//----------------------------------------------------------------------------//
void activeMaterial(const MaterialProperty &mat, int& texCount, unsigned int colorLoc, bool isUseTex = false, unsigned int texLoc = 0, unsigned int isUseTexLoc = 0)
{
    GLHook::Uniform1i(isUseTexLoc, isUseTex);
    if (isUseTex) {
        texCount++;
        GLHook::Uniform1i(texLoc, texCount);
        GLHook::ActiveTexture(GL_TEXTURE0 + texCount);
        GLHook::BindTexture(GL_TEXTURE_2D, static_cast<OpenGLTexture*>(mat.tex)->getOpenGLTexture());
    }
    else {
        GLHook::Uniform4f(colorLoc, mat.color.d_x, mat.color.d_y, mat.color.d_z, mat.color.d_w);
    }
}
    
OpenGL3Model3DRenderer::OpenGL3Model3DRenderer(Renderer& owner) : m_renderOwner(owner), m_area(Rectf(0, 0, 200, 200))
{
    OpenGL3ShaderManager* t_shaderManager = static_cast<OpenGL3Renderer&>(m_renderOwner).getOpenGLShaderManager();
    t_shaderManager->loadShader(SHADER_ID_MODEL3DSHADER, Model3DShaderVert_Model3D, Model3DShaderFrag_Model3D);
    t_shaderManager->loadShader(SHADER_ID_FRAMEBUFFERSHADER, Model3DShaderVert_Framebuffer, Model3DShaderFrag_Framebuffer);
    t_shaderManager->loadShader(SHADER_ID_PICKINGSHADER, Model3DShaderVert_Picking, Model3DShaderFrag_Picking);
    t_shaderManager->loadShader(SHADER_ID_GLSLSHADER, Model3DShaderVert_GLSL, Model3DShaderFrag_GLSL);
}

void OpenGL3Model3DRenderer::initialize(Model3D& target)
{
    // bind shader
    OpenGL3ShaderManager* t_shaderManager = static_cast<OpenGL3Renderer&>(m_renderOwner).getOpenGLShaderManager();
    OpenGL3Shader* t_modelShader = t_shaderManager->getShader(SHADER_ID_MODEL3DSHADER);
    OpenGL3Shader* t_pickShader = t_shaderManager->getShader(SHADER_ID_PICKINGSHADER);
    t_modelShader->bind();
    // set light info
    const std::vector<Light*>::type& t_lights = target.getAllLights();
    GLHook::Uniform1i(t_modelShader->getUniformLocation("numPointLights"), t_lights.size());
    for(int i = 0; i < t_lights.size(); i++) {
        char tmp[30];
        
        std::sprintf(tmp, "pointLights[%d].diffuse", i);
        GLHook::Uniform3f(t_modelShader->getUniformLocation(tmp), t_lights[i]->diffuse.d_x, t_lights[i]->diffuse.d_y, t_lights[i]->diffuse.d_z);
        memset(tmp,'\0',sizeof(tmp));
        
        std::sprintf(tmp, "pointLights[%d].ambient", i);
        GLHook::Uniform3f(t_modelShader->getUniformLocation(tmp), t_lights[i]->ambient.d_x, t_lights[i]->ambient.d_y, t_lights[i]->ambient.d_z);
        memset(tmp,'\0',sizeof(tmp));
        
        std::sprintf(tmp, "pointLights[%d].specular", i);
        GLHook::Uniform3f(t_modelShader->getUniformLocation(tmp), t_lights[i]->specular.d_x, t_lights[i]->specular.d_y, t_lights[i]->specular.d_z);
        memset(tmp,'\0',sizeof(tmp));
        
        std::sprintf(tmp, "pointLights[%d].constant", i);
        GLHook::Uniform1f(t_modelShader->getUniformLocation(tmp), t_lights[i]->constant);
        memset(tmp,'\0',sizeof(tmp));
        
        std::sprintf(tmp, "pointLights[%d].linear", i);
        GLHook::Uniform1f(t_modelShader->getUniformLocation(tmp), t_lights[i]->linear);
        memset(tmp,'\0',sizeof(tmp));
        
        std::sprintf(tmp, "pointLights[%d].quadratic", i);
        GLHook::Uniform1f(t_modelShader->getUniformLocation(tmp), t_lights[i]->quadratic);
        memset(tmp,'\0',sizeof(tmp));
    }
    t_modelShader->unbind();
    
    // create vao, vbo, ebo
    int currentVAO = 0, currentVBO = 0, currentEBO = 0;
    GLHook::GetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);
    GLHook::GetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentVBO);
    GLHook::GetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &currentEBO);
    
    const std::vector<Mesh*>::type& meshes = target.getAllMeshes();
    for (int i = 0; i < meshes.size(); i++) {
        renderInfo t_render;
        renderInfo t_pickrender;

        const CEGUI::Mesh& t_mesh = *meshes[i];
        const std::vector<CEGUI::Mesh::Vertex>::type& t_ver = t_mesh.m_vertices;
        const std::vector<unsigned int>::type& t_ind = t_mesh.m_indices;
        unsigned int numVer = t_ver.size();
        unsigned int numInd = t_ind.size();

        t_render.numIndices = t_ind.size();
        t_pickrender.numIndices = t_render.numIndices;
        // create buffers/arrays
        GLHook::GenVertexArrays(1, &t_render.VAO);
        GLHook::GenVertexArrays(1, &t_pickrender.VAO);
        GLHook::GenBuffers(1, &t_render.VBO);
        t_pickrender.VBO = t_render.VBO;
        GLHook::GenBuffers(1, &t_render.EBO);
        t_pickrender.EBO = t_render.EBO;

        GLHook::BindBuffer(GL_ARRAY_BUFFER, t_render.VBO);
        GLHook::BufferData(GL_ARRAY_BUFFER, numVer * sizeof(CEGUI::Mesh::Vertex), &t_ver[0], GL_DYNAMIC_DRAW);
        GLHook::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, t_render.EBO);
        GLHook::BufferData(GL_ELEMENT_ARRAY_BUFFER, numInd * sizeof(unsigned int), &t_ind[0], GL_DYNAMIC_DRAW);

        //  pick vao
        GLHook::BindVertexArray(t_pickrender.VAO);

        GLHook::BindBuffer(GL_ARRAY_BUFFER, t_pickrender.VBO);
        GLHook::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, t_pickrender.EBO);
        // vertex Positions
        unsigned int loc_pos_pick = t_pickShader->getAttribLocation("aPos");
        GLHook::EnableVertexAttribArray(loc_pos_pick);
        GLHook::VertexAttribPointer(loc_pos_pick, 3, GL_FLOAT, GL_FALSE, sizeof(CEGUI::Mesh::Vertex), (void *)0);

        // render vao
        GLHook::BindVertexArray(t_render.VAO);
        GLHook::BindBuffer(GL_ARRAY_BUFFER, t_render.VBO);
        GLHook::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, t_render.EBO);
        // vertex Positions
        unsigned int loc_pos = t_modelShader->getAttribLocation("aPos");
        GLHook::EnableVertexAttribArray(loc_pos);
        GLHook::VertexAttribPointer(loc_pos, 3, GL_FLOAT, GL_FALSE, sizeof(CEGUI::Mesh::Vertex), (void *)0);

        unsigned int loc_normal = t_modelShader->getAttribLocation("aNormal");
        GLHook::EnableVertexAttribArray(loc_normal);
        GLHook::VertexAttribPointer(loc_normal, 3, GL_FLOAT, GL_FALSE, sizeof(CEGUI::Mesh::Vertex), (void *)offsetof(Mesh::Vertex, Normal));

        unsigned int loc_texcoords = t_modelShader->getAttribLocation("aTexCoords");
        GLHook::EnableVertexAttribArray(loc_texcoords);
        GLHook::VertexAttribPointer(loc_texcoords, 2, GL_FLOAT, GL_FALSE, sizeof(CEGUI::Mesh::Vertex), (void *)offsetof(Mesh::Vertex, TexCoords));
        
        unsigned int loc_boneids = t_modelShader->getAttribLocation("aBoneIDs");
        GLHook::EnableVertexAttribArray(loc_boneids);
        GLHook::VertexAttribPointer(loc_boneids, 3, GL_FLOAT, GL_FALSE, sizeof(CEGUI::Mesh::Vertex), (void *)offsetof(Mesh::Vertex, BoneID));
        GLHook::EnableVertexAttribArray(loc_boneids + 1);
        GLHook::VertexAttribPointer(loc_boneids + 1, 3, GL_FLOAT, GL_FALSE, sizeof(CEGUI::Mesh::Vertex), (void *)(offsetof(Mesh::Vertex, BoneID) + 12));
        GLHook::EnableVertexAttribArray(loc_boneids + 2);
        GLHook::VertexAttribPointer(loc_boneids + 2, 3, GL_FLOAT, GL_FALSE, sizeof(CEGUI::Mesh::Vertex), (void *)(offsetof(Mesh::Vertex, BoneID) + 24));
        
        unsigned int loc_boneweights = t_modelShader->getAttribLocation("aBoneWeights");
        GLHook::EnableVertexAttribArray(loc_boneweights);
        GLHook::VertexAttribPointer(loc_boneweights, 3, GL_FLOAT, GL_FALSE, sizeof(CEGUI::Mesh::Vertex), (void *)offsetof(Mesh::Vertex, BoneWeight));
        GLHook::EnableVertexAttribArray(loc_boneweights + 1);
        GLHook::VertexAttribPointer(loc_boneweights + 1, 3, GL_FLOAT, GL_FALSE, sizeof(CEGUI::Mesh::Vertex), (void *)(offsetof(Mesh::Vertex, BoneWeight) + 12));
        GLHook::EnableVertexAttribArray(loc_boneweights + 2);
        GLHook::VertexAttribPointer(loc_boneweights + 2, 3, GL_FLOAT, GL_FALSE, sizeof(CEGUI::Mesh::Vertex), (void *)(offsetof(Mesh::Vertex, BoneWeight) + 24));

        unsigned int loc_tangent = t_modelShader->getAttribLocation("aTangent");
        GLHook::EnableVertexAttribArray(loc_tangent);
        GLHook::VertexAttribPointer(loc_tangent, 3, GL_FLOAT, GL_FALSE, sizeof(CEGUI::Mesh::Vertex), (void *)offsetof(Mesh::Vertex, Tangent));

        unsigned int loc_bitangent = t_modelShader->getAttribLocation("aBitangent");
        GLHook::EnableVertexAttribArray(loc_bitangent);
        GLHook::VertexAttribPointer(loc_bitangent, 3, GL_FLOAT, GL_FALSE, sizeof(CEGUI::Mesh::Vertex), (void *)offsetof(Mesh::Vertex, Bitangent));

        GLHook::BindVertexArray(currentVAO);
        GLHook::BindBuffer(GL_ARRAY_BUFFER, currentVBO);
        GLHook::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentEBO);
        
        m_renderInfos[i] = t_render;
//        m_pickrenderInfos[i] = t_pickrender;
    }
}
    
//----------------------------------------------------------------------------//
Texture& OpenGL3Model3DRenderer::getTexture()
{
    return static_cast<OpenGLRendererBase&>(m_renderOwner).getTexture("Model3D");
}

//----------------------------------------------------------------------------//
int currentFBO = 0, currentDepthTest = 0, currentAlphaConverage = 0, currentMask = 0, currentVAO = 0;
void OpenGL3Model3DRenderer::beginRendering(Model3DInstance& target)
{
    // backup current information
    GLHook::GetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);
    GLHook::GetIntegerv(GL_DEPTH_TEST, &currentDepthTest);
    GLHook::GetIntegerv(GL_SAMPLE_ALPHA_TO_COVERAGE, &currentAlphaConverage);
    GLHook::GetIntegerv(GL_DEPTH_WRITEMASK, &currentMask);
    GLHook::GetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);

    // set information for render 3d model
    GLHook::BindFramebuffer(GL_FRAMEBUFFER, m_frame.FBO);
    GLHook::DepthMask(true);
    GLHook::Viewport(0,0,m_area.getWidth(),m_area.getHeight());
    GLHook::Scissor(0, 0, m_area.getWidth(), m_area.getHeight());
    GLHook::ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    GLHook::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLHook::Enable(GL_DEPTH_TEST);
    GLHook::Enable(GL_SAMPLE_ALPHA_TO_COVERAGE);

    // bind shader
    OpenGL3ShaderManager* t_shaderManager = static_cast<OpenGL3Renderer&>(m_renderOwner).getOpenGLShaderManager();
    OpenGL3Shader* t_modelShader = t_shaderManager->getShader(SHADER_ID_MODEL3DSHADER);
    t_modelShader->bind();

    // setup model
    GLHook::UniformMatrix4fv(t_modelShader->getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
    // setup projection
    glm::mat4 projection = glm::perspective((float)target.getCamera().fovy, (float)m_area.getWidth() / (float)m_area.getHeight(), target.getCamera().near, target.getCamera().far);
    memcpy(m_projection, glm::value_ptr(projection), 16 * sizeof(float));
    // setup camera
    const Vector3f& camPos = target.getCamera().position;
    const Vector3f& camUp = target.getCamera().up;
    const Vector3f& camTarget = target.getCamera().target;
    glm::vec3 vecCamPos(camPos.d_x, camPos.d_y, camPos.d_z);
    vecCamPos = glm::vec4(vecCamPos, 0.0) * glm::rotate(glm::mat4(1.0f), (float)target.getCamera().angle, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 vecCamUp(camUp.d_x, camUp.d_y, camUp.d_z);
    vecCamUp = glm::vec4(vecCamUp, 0.0) * glm::rotate(glm::mat4(1.0f), (float)target.getCamera().angle, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 vecCamTarget(camTarget.d_x, camTarget.d_y, camTarget.d_z);
    vecCamTarget = glm::vec4(vecCamTarget, 0.0) * glm::rotate(glm::mat4(1.0f), (float)target.getCamera().angle, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 view = glm::lookAt(vecCamPos, vecCamTarget, vecCamUp);
    GLHook::Uniform3f(t_modelShader->getUniformLocation("viewPos"), vecCamPos.x, vecCamPos.y, vecCamPos.z);
    // setup world
    glm::mat4 world = glm::make_mat4(m_projection) * view;
    GLHook::UniformMatrix4fv(t_modelShader->getUniformLocation("world"), 1, GL_FALSE, glm::value_ptr(world));
    // setup light
    const std::vector<Light*>::type& t_lights = target.getModel3D().getAllLights();
    for (int i = 0; i < t_lights.size(); i++)
    {
        char tmp[30];
        std::sprintf(tmp, "pointLights[%d].position", i);
        glm::mat4 modelLight = glm::mat4(1.0f);
        glm::vec3 newLightPos = modelLight * glm::vec4(t_lights[i]->position.d_x, t_lights[i]->position.d_y, t_lights[i]->position.d_z, 0.0f);
        GLHook::Uniform3f(t_modelShader->getUniformLocation(tmp), newLightPos.x, newLightPos.y, newLightPos.z);
        memset(tmp,'\0',sizeof(tmp));
    }
    
    // setup bone mat
    for (auto it = target.getModel3D().getAllNodes().begin(); it != target.getModel3D().getAllNodes().end(); ++it) {
        if (!target.getNodeInfo(it->first).isBoneUpdated || it->second.m_boneID == -1) continue;
        
        char tmp[30];
        std::sprintf(tmp, "bone[%d]", it->second.m_boneID);
        GLHook::UniformMatrix4fv(t_modelShader->getUniformLocation(tmp), 1, GL_FALSE, glm::value_ptr(glm::make_mat4(target.getNodeInfo(it->first).boneTransform)));
    }
}

//----------------------------------------------------------------------------//
void OpenGL3Model3DRenderer::render(Model3DInstance& target, int meshid)
{
    OpenGL3ShaderManager* t_shaderManager = static_cast<OpenGL3Renderer&>(m_renderOwner).getOpenGLShaderManager();
    OpenGL3Shader* t_modelShader = t_shaderManager->getShader(SHADER_ID_MODEL3DSHADER);
    renderInfo r = m_renderInfos[meshid];
    
    const Model3DInstance::NodeInfo& nodeInfo = target.getNodeInfo(target.getModel3D().getAllMeshes()[meshid]->m_parent->m_name);
    GLHook::Uniform1i(t_modelShader->getUniformLocation("isActived"), 0);
    
    GLHook::UniformMatrix4fv(t_modelShader->getUniformLocation("global"),
                             1, GL_FALSE, glm::value_ptr(glm::make_mat4(nodeInfo.transform)));
    
    GLHook::Uniform4f(t_modelShader->getUniformLocation("colorAttached"), nodeInfo.color.d_x, nodeInfo.color.d_y, nodeInfo.color.d_z, nodeInfo.color.d_w);
    
    int materialID = target.getModel3D().getAllMeshes()[meshid]->m_materialID;
    const Material& mat = *(target.getModel3D().getAllMaterials()[materialID]);
    int texId = 0;
    activeMaterial(mat.ambient, texId, t_modelShader->getUniformLocation("material.ambient.color"), mat.ambient.tex,
                   t_modelShader->getUniformLocation("material.ambient.texture"),
                   t_modelShader->getUniformLocation("material.ambient.useTexture"));
    activeMaterial(mat.diffuse, texId, t_modelShader->getUniformLocation("material.diffuse.color"), mat.diffuse.tex,
                   t_modelShader->getUniformLocation("material.diffuse.texture"),
                   t_modelShader->getUniformLocation("material.diffuse.useTexture"));
    activeMaterial(mat.specular, texId, t_modelShader->getUniformLocation("material.specular.color"), mat.specular.tex,
                   t_modelShader->getUniformLocation("material.specular.texture"),
                   t_modelShader->getUniformLocation("material.specular.useTexture"));
    activeMaterial(mat.emissive, texId, t_modelShader->getUniformLocation("material.emissive.color"), mat.emissive.tex,
                   t_modelShader->getUniformLocation("material.emissive.texture"),
                   t_modelShader->getUniformLocation("material.emissive.useTexture"));
    activeMaterial(mat.normals, texId, t_modelShader->getUniformLocation("material.normals.color"), mat.normals.tex,
                   t_modelShader->getUniformLocation("material.normals.texture"),
                   t_modelShader->getUniformLocation("material.normals.useTexture"));
    GLHook::Uniform1f(t_modelShader->getUniformLocation("material.shininess"), mat.shininess);
    GLHook::Uniform1f(t_modelShader->getUniformLocation("material.strength"), mat.strength);
    GLHook::Uniform1f(t_modelShader->getUniformLocation("material.transparent"), mat.transparent);
    //    printf("mesh.VAO: id:%d, VAO:%d\n", id, r.VAO);
    GLHook::BindVertexArray(r.VAO);
    
    GLHook::DrawElements(GL_TRIANGLES, r.numIndices, GL_UNSIGNED_INT, 0);
    
}

//----------------------------------------------------------------------------//
void OpenGL3Model3DRenderer::endRendering(Model3DInstance& target)
{
    // unbind shader
    OpenGL3ShaderManager* t_shaderManager = static_cast<OpenGL3Renderer&>(m_renderOwner).getOpenGLShaderManager();
    OpenGL3Shader* t_modelShader = t_shaderManager->getShader(SHADER_ID_MODEL3DSHADER);
    t_modelShader->unbind();
    
#if __APPLE__
    // swap sampled FBO to normal FBO
    GLHook::BindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frame.FBOforRBO);
    GLHook::BindFramebuffer(GL_READ_FRAMEBUFFER, m_frame.FBO);
    RESOLVEMULTISAMPLEFRAMEBUFFER(0, 0, m_area.getWidth(), m_area.getHeight(), 0, 0, m_area.getWidth(), m_area.getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
#endif
    
    // restore setting
    GLHook::BindVertexArray(currentVAO);
    GLHook::BindFramebuffer(GL_FRAMEBUFFER, currentFBO);
    if (currentDepthTest) GLHook::Enable(GL_DEPTH_TEST);
    else GLHook::Disable(GL_DEPTH_TEST);
    if (currentAlphaConverage) GLHook::Enable(GL_SAMPLE_ALPHA_TO_COVERAGE);
    else GLHook::Disable(GL_SAMPLE_ALPHA_TO_COVERAGE);
    GLHook::DepthMask(currentMask);
}

//----------------------------------------------------------------------------//
void OpenGL3Model3DRenderer::setTextureSize(int width, int height)
{
    if (width == m_area.getWidth() && height == m_area.getHeight()) return;
    
    m_area = Rectf(0, 0, width, height);
    
    // update fbo, texture for new texture size
    generateFramebuffer(width, height);
    
    // create texture for new texture size
    OpenGLRendererBase& renderer = static_cast<OpenGLRendererBase&>(m_renderOwner);
    if (renderer.isTextureDefined("Model3D")) {
        renderer.destroyTexture("Model3D");
    }
    renderer.createTexture("Model3D", m_frame.TEX, m_area.getSize());
}

//----------------------------------------------------------------------------//
void OpenGL3Model3DRenderer::generateFramebuffer(int width, int height)
{
    int currentFBO = 0, currentTEX = 0, currentRBO = 0;
    GLHook::GetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);
    GLHook::GetIntegerv(GL_TEXTURE_BINDING_2D, &currentTEX);
    GLHook::GetIntegerv(GL_RENDERBUFFER_BINDING, &currentRBO);
    int samples;
    GLHook::GetIntegerv(GL_MAX_SAMPLES, &samples);
    
    GLenum frameStatus;
    
    GLHook::GenFramebuffers(1, &m_frame.FBO);
    GLHook::BindFramebuffer(GL_FRAMEBUFFER, m_frame.FBO);
    
    GLHook::GenRenderbuffers(1, &m_frame.DBO);
    GLHook::BindRenderbuffer(GL_RENDERBUFFER, m_frame.DBO);
    RENDERBUFFERSTORAGEMULTISAMPLE(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT16, width, height);
    GLHook::FramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_frame.DBO);
    
#if __APPLE__
    GLHook::GenRenderbuffers(1, &m_frame.RBO);
    GLHook::BindRenderbuffer(GL_RENDERBUFFER, m_frame.RBO);
    RENDERBUFFERSTORAGEMULTISAMPLE(GL_RENDERBUFFER, samples, GL_RGBA8, width, height);
    GLHook::FramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_frame.RBO);
    
    frameStatus = GLHook::CheckFramebufferStatus(GL_FRAMEBUFFER);
    if(frameStatus != GL_FRAMEBUFFER_COMPLETE) {
        printf("OpenGL3Model3DRenderer::generateFramebuffer Framebuffer is not complete! 0x%x\n", frameStatus);
    }
    
    GLHook::GenFramebuffers(1, &m_frame.FBOforRBO);
    GLHook::BindFramebuffer(GL_FRAMEBUFFER, m_frame.FBOforRBO);
#endif

    GLHook::GenTextures(1, &m_frame.TEX);
    GLHook::BindTexture(GL_TEXTURE_2D, m_frame.TEX);
    GLHook::TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    GLHook::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLHook::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    GLHook::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    GLHook::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    FRAMEBUFFERTEXTURE2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_frame.TEX, 0, samples);

    frameStatus = GLHook::CheckFramebufferStatus(GL_FRAMEBUFFER);
    if(frameStatus != GL_FRAMEBUFFER_COMPLETE) {
        printf("OpenGL3Model3DRenderer::generateFramebuffer Framebuffer is not complete! 0x%x\n", frameStatus);
    }

    GLHook::BindFramebuffer(GL_FRAMEBUFFER, currentFBO);
    GLHook::BindTexture(GL_TEXTURE_2D, currentTEX);
    GLHook::BindRenderbuffer(GL_RENDERBUFFER, currentRBO);
}

//----------------------------------------------------------------------------//
OpenGL3Model3DRenderer::~OpenGL3Model3DRenderer()
{
    for(int i=0; i<m_renderInfos.size(); i++)
    {
        GLHook::DeleteVertexArrays(1, &m_renderInfos[i].VAO);
        GLHook::DeleteBuffers(1, &m_renderInfos[i].VBO);
        GLHook::DeleteBuffers(1, &m_renderInfos[i].EBO);
    }
    GLHook::DeleteFramebuffers(1, &m_frame.FBO);
#if __APPLE__
    GLHook::DeleteFramebuffers(1, &m_frame.FBOforRBO);
    GLHook::DeleteRenderbuffers(1, &m_frame.RBO);
#endif
    GLHook::DeleteRenderbuffers(1, &m_frame.DBO);
    GLHook::DeleteTextures(1, &m_frame.TEX);
    static_cast<OpenGLRendererBase&>(m_renderOwner).destroyTexture("Model3D");
}
}// End of  CEGUI namespace section
