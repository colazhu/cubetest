#ifndef TWOTAPGESTURE_H
#define TWOTAPGESTURE_H

#include "gestureObject.h"

class TwoTapGesture:public GestureObject
{
public:
    TwoTapGesture();
    virtual ~TwoTapGesture();

    void setPoint(int index, int x, int y);
    GesturePoint& getPoint(int index);

    void setCenter(int x, int y);
    GesturePoint& getCenter();

    void setLastCenter(int x, int y);
    GesturePoint& getLastCenter();

    void setStartCenter(int x, int y);
    GesturePoint& getStartCenter();

    void setOffset(int x, int y);
    GesturePoint& getOffset();

    void setLastOffset(int x, int y);
    GesturePoint& getLastOffset();

    void setStartOffset(int x, int y);
    GesturePoint& getStartOffset();
    
    virtual bool copyData(GestureObject* object);
    virtual void clearData();

private:
    GesturePoint mPoint[2];
    GesturePoint mCenter;
    GesturePoint mLastCenter;
    GesturePoint mStartCenter;
    GesturePoint mOffset;
    GesturePoint mLastOffset;
    GesturePoint mStartOffset;
};

#endif /* TWOTAPGESTURE_H */
