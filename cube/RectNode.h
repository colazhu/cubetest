#ifndef RECTNODE_H
#define RECTNODE_H

#include "Node.h"

class RectNode : public Node {
public:
    RectNode(const std::string& name, Node*parent);
	virtual ~RectNode();

    virtual void onInitGeometryBuffer();
    virtual void onDraw();
    void setTexture(int txtid);
    void setRatio(float ratio);
    void useMVP(bool use = false);

protected:
	RectNode();

    struct V3F_N3F_T2F_C4F
    {
        Vector3 position;
        Vector3 normal;
        Tex2F   texCoords;
        Color4F colors;
    };
    V3F_N3F_T2F_C4F m_vertices[6];

    int m_txtid;
    float m_ratio;
    bool m_useMvp;
};
#endif
