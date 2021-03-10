#ifndef DATATYPES_H
#define DATATYPES_H

#include "Base.h"
#include "MathInc.h"
#include "Geometry.h"
#include "GLFunction.h"

struct Color4B;
struct Color4F;

/** RGB color composed of bytes 3 bytes
 */
struct Color3B
{
    Color3B();
    Color3B(GLubyte _r, GLubyte _g, GLubyte _b);
    explicit Color3B(const Color4B& color);
    explicit Color3B(const Color4F& color);

    bool operator==(const Color3B& right) const;
    bool operator==(const Color4B& right) const;
    bool operator==(const Color4F& right) const;
    bool operator!=(const Color3B& right) const;
    bool operator!=(const Color4B& right) const;
    bool operator!=(const Color4F& right) const;

    bool equals(const Color3B& other)
    {
        return (*this == other);
    }

    GLubyte r;
    GLubyte g;
    GLubyte b;
    
    static const Color3B WHITE;
    static const Color3B YELLOW;
    static const Color3B BLUE;
    static const Color3B GREEN;
    static const Color3B RED;
    static const Color3B MAGENTA;
    static const Color3B BLACK;
    static const Color3B ORANGE;
    static const Color3B GRAY;
};

/** RGBA color composed of 4 bytes
*/
struct Color4B
{
    Color4B();
    Color4B(GLubyte _r, GLubyte _g, GLubyte _b, GLubyte _a);
    explicit Color4B(const Color3B& color);
    explicit Color4B(const Color4F& color);

    bool operator==(const Color4B& right) const;
    bool operator==(const Color3B& right) const;
    bool operator==(const Color4F& right) const;
    bool operator!=(const Color4B& right) const;
    bool operator!=(const Color3B& right) const;
    bool operator!=(const Color4F& right) const;

    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;

    static const Color4B WHITE;
    static const Color4B YELLOW;
    static const Color4B BLUE;
    static const Color4B GREEN;
    static const Color4B RED;
    static const Color4B MAGENTA;
    static const Color4B BLACK;
    static const Color4B ORANGE;
    static const Color4B GRAY;
};


/** RGBA color composed of 4 floats
*/
struct Color4F
{
    Color4F();
    Color4F(float _r, float _g, float _b, float _a);
    Color4F(const Vector4& vec4);
    explicit Color4F(const Color3B& color);
    explicit Color4F(const Color4B& color);

    bool operator==(const Color4F& right) const;
    bool operator==(const Color3B& right) const;
    bool operator==(const Color4B& right) const;
    bool operator!=(const Color4F& right) const;
    bool operator!=(const Color3B& right) const;
    bool operator!=(const Color4B& right) const;
    const Color4F operator+(const Color4F& v) const;

    static inline Vector4 multiply(const Vector4& aa, const Vector4& bb) {
        return Vector4(aa.x * bb.x,
                       aa.y * bb.y,
                       aa.z * bb.z,
                       aa.w * bb.w);
    }

    static inline Color4F multiply(const Color4F& aa, const Color4F& bb) {
        return Color4F(aa.r * bb.r,
                       aa.g * bb.g,
                       aa.b * bb.b,
                       aa.a * bb.a);
    }

    bool equals(const Color4F &other)
    {
        return (*this == other);
    }
    
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;

    static const Color4F WHITE;
    static const Color4F YELLOW;
    static const Color4F BLUE;
    static const Color4F GREEN;
    static const Color4F RED;
    static const Color4F MAGENTA;
    static const Color4F BLACK;
    static const Color4F ORANGE;
    static const Color4F GRAY;
};
        
/** A texcoord composed of 2 floats: u, y
 */
struct Tex2F {
    Tex2F(float _u, float _v): u(_u), v(_v) {}
    
    Tex2F(): u(0.f), v(0.f) {}
    
    GLfloat u;
    GLfloat v;
};

 
//! Vec2 Sprite component
struct PointSprite
{
    Vector2    pos;        // 8 bytes
    Color4B    color;      // 4 bytes
    GLfloat    size;       // 4 bytes
};

//!    A 2D Quad. 4 * 2 floats
struct Quad2
{
    Vector2     tl;
    Vector2     tr;
    Vector2     bl;
    Vector2     br;
};


//!    A 3D Quad. 4 * 3 floats
struct Quad3 {
    Vector3        bl;
    Vector3        br;
    Vector3        tl;
    Vector3        tr;
};

//! a Vec2 with a vertex point, a tex coord point and a color 4B
struct V2F_C4B_T2F
{
    //! vertices (2F)
    Vector2      vertices;
    //! colors (4B)
    Color4B      colors;
    //! tex coords (2F)
    Tex2F        texCoords;
};

//! a Vec2 with a vertex point, a tex coord point and a color 4F
struct V2F_C4F_T2F
{
    //! vertices (2F)
    Vector2      vertices;
    //! colors (4F)
    Color4F      colors;
    //! tex coords (2F)
    Tex2F        texCoords;
};

//! a Vec3 with a vertex point, a tex coord point and a color 4B
struct V3F_C4B_T2F
{
    //! vertices (3F)
    Vector3     vertices;            // 12 bytes

    //! colors (4B)
    Color4B      colors;              // 4 bytes

    //! tex coords (2F)
    Tex2F        texCoords;           // 8 bytes
};

//! a Vec2 with a vertex point, a tex coord point
struct V3F_T2F
{
    //! vertices (2F)
    Vector3       vertices;
    //! tex coords (2F)
    Tex2F          texCoords;
};

//! A Triangle of V2F_C4B_T2F
struct V2F_C4B_T2F_Triangle
{
	//! Vec2 A
	V2F_C4B_T2F a;
	//! Vec2 B
	V2F_C4B_T2F b;
	//! Vec2 B
	V2F_C4B_T2F c;
};

//! A Quad of V2F_C4B_T2F
struct V2F_C4B_T2F_Quad
{
    //! bottom left
    V2F_C4B_T2F    bl;
    //! bottom right
    V2F_C4B_T2F    br;
    //! top left
    V2F_C4B_T2F    tl;
    //! top right
    V2F_C4B_T2F    tr;
};

//! 4 Vertex3FTex2FColor4B
struct V3F_C4B_T2F_Quad
{
    //! top left
    V3F_C4B_T2F    tl;
    //! bottom left
    V3F_C4B_T2F    bl;
    //! top right
    V3F_C4B_T2F    tr;
    //! bottom right
    V3F_C4B_T2F    br;
};

//! 4 Vertex2FTex2FColor4F Quad
struct V2F_C4F_T2F_Quad
{
    //! bottom left
    V2F_C4F_T2F    bl;
    //! bottom right
    V2F_C4F_T2F    br;
    //! top left
    V2F_C4F_T2F    tl;
    //! top right
    V2F_C4F_T2F    tr;
};

struct V3F_T2F_Quad
{
    //! bottom left
    V3F_T2F    bl;
    //! bottom right
    V3F_T2F    br;
    //! top left
    V3F_T2F    tl;
    //! top right
    V3F_T2F    tr;
};

//! Blend Function used for textures
struct BlendFunc
{
    //! source blend function
    GLenum src;
    //! destination blend function
    GLenum dst;

    //! Blending disabled. Uses {GL_ONE, GL_ZERO}
    static const BlendFunc DISABLE;
    //! Blending enabled for textures with Alpha premultiplied. Uses {GL_ONE, GL_ONE_MINUS_SRC_ALPHA}
    static const BlendFunc ALPHA_PREMULTIPLIED;
    //! Blending enabled for textures with Alpha NON premultiplied. Uses {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA}
    static const BlendFunc ALPHA_NON_PREMULTIPLIED;
    //! Enables Additive blending. Uses {GL_SRC_ALPHA, GL_ONE}
    static const BlendFunc ADDITIVE;

    bool operator==(const BlendFunc &a) const
    {
        return src == a.src && dst == a.dst;
    }

    bool operator<(const BlendFunc &a) const
    {
        return src < a.src || (src == a.src && dst < a.dst);
    }

	void setBlending(bool on = true) 
	{
		if(on) {
			setBlending(src,dst);
		}
		else {
			setBlending(BlendFunc::DISABLE.src,BlendFunc::DISABLE.dst);
		}
	}

	static void setBlending(GLenum sfactor, GLenum dfactor)
	{
		if (sfactor == GL_ONE && dfactor == GL_ZERO)
		{
            GLHook::glDisable(GL_BLEND);
		}
		else
		{
            GLHook::glEnable(GL_BLEND);
            GLHook::glBlendFunc(sfactor, dfactor);
		}
	}
};

// texture coordinates for a quad
struct T2F_Quad
{
    //! bottom left
    Tex2F    bl;
    //! bottom right
    Tex2F    br;
    //! top left
    Tex2F    tl;
    //! top right
    Tex2F    tr;
};

// struct that holds the size in pixels, texture coordinates and delays for animated ParticleSystemQuad
//struct AnimationFrameData
//{
//    T2F_Quad texCoords;
//    float delay;
//    Size size;
//};

#endif /* DATATYPES_H */
