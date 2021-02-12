#include "Geometry.h"

#include <algorithm>

Point::Point()
: x(0.0f),
y(0.0f)
{
}

Point::Point(float xx, float yy)
: x(xx),
y(yy)
{
}

const Point Point::operator*(float s) const
{
    Point result(x, y);
    result.x = result.x * s;
    result.y = result.y * s;
    return result;
}

Point& Point::operator*=(float s)
{
    x *= s;
    y *= s;
    return *this;
}

const Point Point::operator+(const Point& v) const
{
    Point result(x, y);
    result.x += v.x;
    result.y += v.y;
    return result;
}

Point& Point::operator+=(const Point& v)
{
    x += v.x;
    y += v.y;
    return *this;
}

bool Point::operator==(const Point& v) const
{
    return x == v.x && y == v.y;
}

bool Point::operator!=(const Point& v) const
{
    return x != v.x || y != v.y;
}

bool Point::equals(const Point& target) const
{
    return (fabs(this->x - target.x) < FLT_EPSILON) && (fabs(this->y - target.y) < FLT_EPSILON);
}

void Point::set(float xx, float yy)
{
    x = xx;
    y = yy;
}

void Point::set(Point& p)
{
    x = p.x;
    y = p.y;
}

Point Point::lerp(const Point& other, float alpha) const
{
    return *this * (1.f - alpha) + other * alpha;
}

Size::Size(void)
        : width(0),
          height(0)
{
}

Size::Size(float w, float h)
        : width(w),
          height(h)
{
}

Size::Size(const Size& other)
        : width(other.width),
          height(other.height)
{
}


Size& Size::operator=(const Size& other)
{
    setSize(other.width, other.height);
    return *this;
}

Size Size::operator+(const Size& right) const
{
    return Size(this->width + right.width, this->height + right.height);
}

Size Size::operator-(const Size& right) const
{
    return Size(this->width - right.width, this->height - right.height);
}

Size Size::operator*(float a) const
{
    return Size(this->width * a, this->height * a);
}

Size Size::operator/(float a) const
{
    return Size(this->width / a, this->height / a);
}

void Size::setSize(float w, float h)
{
    this->width = w;
    this->height = h;
}

bool Size::equals(const Size& target) const
{
    return (fabs(this->width - target.width) < FLT_EPSILON)
            && (fabs(this->height - target.height) < FLT_EPSILON);
}

// implementation of Rect

Rect::Rect()
{
    setRect(0.0f, 0.0f, 0.0f, 0.0f);
}

Rect::Rect(float x, float y, float width, float height)
{
    setRect(x, y, width, height);
}

Rect::Rect(const Point& origin, const Size& size) {
    setRect(origin.x, origin.y, size.width, size.height);
}

Rect::Rect(const Rect& other)
{
    setRect(other.position.x, other.position.y, other.size.width, other.size.height);
}

Rect& Rect::operator=(const Rect& other)
{
    setRect(other.position.x, other.position.y, other.size.width, other.size.height);
    return *this;
}

void Rect::setRect(float x, float y, float width, float height)
{
    // CGRect can support width<0 or height<0
    // ASSERT(width >= 0.0f && height >= 0.0f, "width and height of Rect must not less than 0.");

    position.x = x;
    position.y = y;

    size.width = width;
    size.height = height;
}

bool Rect::equals(const Rect& rect) const
{
    return (position.equals(rect.position) && size.equals(rect.size));
}

float Rect::getMaxX() const
{
    return position.x + size.width;
}

float Rect::getMidX() const
{
    return position.x + size.width / 2.0f;
}

float Rect::getMinX() const
{
    return position.x;
}

float Rect::getMaxY() const
{
    return position.y + size.height;
}

float Rect::getMidY() const
{
    return position.y + size.height / 2.0f;
}

float Rect::getMinY() const
{
    return position.y;
}

bool Rect::containsPoint(float x, float y) const
{
    bool bRet = false;
    if (x >= getMinX() && x <= getMaxX() && y >= getMinY() && y <= getMaxY()) {
        bRet = true;
    }
    
    return bRet;
}

bool Rect::containsRect(const Rect& rect) const
{
    return (rect.getMinX() >= getMinX()
            && rect.getMaxX() <= getMaxX()
            && rect.getMinY() >= getMinY()
            && rect.getMaxY() <= getMaxY());
}

bool Rect::intersectsRect(const Rect& rect) const
{
    return !(getMaxX() < rect.getMinX()
            || rect.getMaxX() < getMinX()
            || getMaxY() < rect.getMinY()
            || rect.getMaxY() < getMinY());
}

Rect Rect::unionWithRect(const Rect & rect) const
{
    float thisLeftX = position.x;
    float thisRightX = position.x + size.width;
    float thisTopY = position.y + size.height;
    float thisBottomY = position.y;

    if (thisRightX < thisLeftX) {
        std::swap(thisRightX, thisLeftX);   // This rect has negative width
    }

    if (thisTopY < thisBottomY) {
        std::swap(thisTopY, thisBottomY);   // This rect has negative height
    }

    float otherLeftX = rect.position.x;
    float otherRightX = rect.position.x + rect.size.width;
    float otherTopY = rect.position.y + rect.size.height;
    float otherBottomY = rect.position.y;

    if (otherRightX < otherLeftX) {
        std::swap(otherRightX, otherLeftX);   // Other rect has negative width
    }

    if (otherTopY < otherBottomY) {
        std::swap(otherTopY, otherBottomY);   // Other rect has negative height
    }

    float combinedLeftX = std::min(thisLeftX, otherLeftX);
    float combinedRightX = std::max(thisRightX, otherRightX);
    float combinedTopY = std::max(thisTopY, otherTopY);
    float combinedBottomY = std::min(thisBottomY, otherBottomY);

    return Rect(combinedLeftX,
                combinedBottomY,
                combinedRightX - combinedLeftX,
                combinedTopY - combinedBottomY);
}

