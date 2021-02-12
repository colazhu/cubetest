#ifndef DRAGGESTURE_H
#define DRAGGESTURE_H

#include "gestureObject.h"

class DragGesture : public GestureObject
{
public:
    DragGesture();
    virtual ~DragGesture();

    virtual bool copyData(GestureObject* object);
    virtual void clearData();

    inline GesturePoint getPoint() const { return mPoint; }
    inline GesturePoint lastPos() const { return mLastPoint; }
    inline GesturePoint startPos() const { return mStartPoint; }
    inline void setPoint(GesturePoint& point) { mPoint = point; }
    inline void setLastPos(int x, int y) { mLastPoint.x = x; mLastPoint.y = y; }
    inline void setStartPos(int x, int y) { mStartPoint.x = x; mStartPoint.y = y; }
    inline void setPoint(int x, int y) { mPoint.x = x; mPoint.y = y; }

private:
    GesturePoint mPoint;
    GesturePoint mLastPoint;
    GesturePoint mStartPoint;
};


#endif // DRAGGESTURE_H
