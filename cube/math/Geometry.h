#ifndef GEOMETRY_H
#define GEOMETRY_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <math.h>

#ifndef FLT_EPSILON
#define FLT_EPSILON     1.192092896e-07F
#endif

/**
 */
class Point
{
public:
    float x;
    float y;
public:
    Point();
    Point(float xx, float yy);
    const Point operator*(float s) const;
    Point& operator*=(float s);
    const Point operator+(const Point& v) const;
    Point& operator+=(const Point& v);
    bool operator==(const Point& v) const;
    bool operator!=(const Point& v) const;
    bool equals(const Point& target) const;
    void set(float xx, float yy);
    void set(Point& p);
    Point lerp(const Point& other, float alpha) const;
};

/**
 */
class Size
{
public:
    float width;
    float height;

public:

    /**
     */
    Size();

    /**
     */
    Size(float width, float height);

    /**
     */
    Size(const Size& other);

    /**
     */
    explicit Size(const Point& point);

    /**
     */
    Size& operator=(const Size& other);

    /**
     */
    Size& operator=(const Point& point);

    /**
     */
    Size operator+(const Size& right) const;

    /**
     */
    Size operator-(const Size& right) const;

    /**
     */
    Size operator*(float a) const;

    /**
     */
    Size operator/(float a) const;

    /**
     */
    void setSize(float width, float height);

    /**
     */
    bool equals(const Size& target) const;

    static const Size ZERO;
};

/**
 */
class Rect
{
public:
    Point position;
    Size size;

public:
    /**
     */
    Rect();

    /**
     */
    Rect(float x, float y, float width, float height);

    /**
     */
    Rect(const Point& origin, const Size& size);

    /**
     */
    Rect(const Rect& other);

    /**
     */
    Rect& operator=(const Rect& other);

    /**
     */
    void setRect(float x, float y, float width, float height);

    /**
     * return the leftmost x-value of current rect
     */
    float getMinX() const;

    /**
     * return the midpoint x-value of current rect
     */
    float getMidX() const;

    /**
     * return the rightmost x-value of current rect
     */
    float getMaxX() const;

    /**
     * return the bottommost y-value of current rect
     */
    float getMinY() const;

    /**
     * return the midpoint y-value of current rect
     */
    float getMidY() const;

    /**
     * return the topmost y-value of current rect
     */
    float getMaxY() const;

    /**
     */
    bool equals(const Rect& rect) const;

    /**
     */
    bool containsPoint(float x, float y) const;

    /**
     * @brief containsRect
     * @param rect
     * @return
     */
    bool containsRect(const Rect& rect) const;

    /**
     */
    bool intersectsRect(const Rect& rect) const;

    /**
     */
    Rect unionWithRect(const Rect & rect) const;

};
#endif /* GEOMETRY_H */
