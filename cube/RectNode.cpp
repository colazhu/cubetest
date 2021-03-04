#include "RectNode.h"


RectNode::RectNode(const std::string& name, Node* parent):
Node(name, parent),
m_txtid(0),
m_ratio(1.0)
{
}

RectNode::~RectNode()
{	

}

void RectNode::onInitGeometryBuffer()
{
    float ratio = 1.0/m_ratio;
    float x = 2.0*ratio;
    float y = 2.0;
    float z = 0.0;
    m_vertices[0].position.set(0, 0, z); // left-bottom
    m_vertices[0].texCoords = Tex2F(0, 0);
    m_vertices[1].position.set(0, y, z); // left-top
    m_vertices[1].texCoords = Tex2F(0, 1);
    m_vertices[2].position.set(x, y, z); // right-top
    m_vertices[2].texCoords = Tex2F(1, 1);
    m_vertices[3].position.set(m_vertices[0].position); // left-bottom
    m_vertices[3].texCoords = m_vertices[0].texCoords;
    m_vertices[4].position.set(m_vertices[2].position); // right-top
    m_vertices[4].texCoords = m_vertices[2].texCoords;
    m_vertices[5].position.set(x, 0, z); // right-bottom
    m_vertices[5].texCoords = Tex2F(1, 0);

    for (int i = 0; i < 6; ++i) {
        m_vertices[i].normal.set(0, 0, 1);
        m_vertices[i].colors = Color4F::BLUE;
        m_vertices[i].position -= Vector3(ratio, 1, 0);
    }

    GLHook::glBindBuffer(GL_ARRAY_BUFFER, m_buffersVBO[0]);
    GLHook::glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices[0]) * 6, m_vertices, GL_STATIC_DRAW);
    GLHook::glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RectNode::onDraw()
{
    GLHook::glDisable(GL_DEPTH_TEST);
    Program* p = Director::instance()->programCache().getProgram(POSITIONTEXTURENOMVPSHADER);
    if (!p) {
        return;
    }
    p->use();
    p->setUniformsForBuiltins();

    if (Texture* txt = Director::instance()->textureCache().getTexture(m_txtid)) {
        txt->bind();
    }

    GLHook::glBindBuffer(GL_ARRAY_BUFFER, m_buffersVBO[0]);
    // GLHook::glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices[0]) * 6, m_vertices, GL_STATIC_DRAW);
    ENABLE_ATTR_POSITION(V3F_N3F_T2F_C4F, Vector3, position);
//    ENABLE_ATTR_NORMAL(V3F_N3F_T2F_C4F, Vector3, normal);
    ENABLE_ATTR_TEX_COORD(V3F_N3F_T2F_C4F, Tex2F, texCoords);
//    ENABLE_ATTR_COLOR(V3F_N3F_T2F_C4F, Color4F, colors);
    GLHook::glDrawArrays(GL_TRIANGLES, 0, 6);

    GLHook::glBindBuffer(GL_ARRAY_BUFFER, 0);
    GLHook::glEnable(GL_DEPTH_TEST);
    CHECK_GL_ERROR_DEBUG();

}

void RectNode::setTexture(int txtid)
{
    m_txtid = txtid;
}

void RectNode::setRatio(float ratio)
{
    m_ratio = ratio;
}
