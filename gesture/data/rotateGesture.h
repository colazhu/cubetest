#ifndef ROTATEGESTURE_H
#define ROTATEGESTURE_H

#include "gestureObject.h"

class RotateGesture:public GestureObject
{
public:
    RotateGesture();
    virtual ~RotateGesture();
    virtual bool copyData(GestureObject* object);
    virtual void clearData();

    //get propety
    GesturePoint* getPoints();
    GesturePoint* getStartPoints();
    GesturePoint* getLastPoints();
    GesturePoint getStartCenter() const {return mStartCenterPoint;}
    GesturePoint getLastCenter() const {return mLastCenterPoint;}
    GesturePoint getCenter() const {return mCenterPoint;}

    float getTotalRotationAngle() const {return mTotalRotationAngle;}
    float getLastRotationAngle() const {return mLastRotationAngle;}
    float getRotationAngle() const {return mRotationAngle;}
	float getStartRotationAngle() const {return mStartRotationAngle;}

    //set propety
    void setPoints(int x1, int y1, int x2, int y2);
    void setStartPoints(int x1, int y1, int x2, int y2);
    void setLastPoints(int x1, int y1, int x2, int y2);
    void setStartCenter(GesturePoint value) {mStartCenterPoint = value;}
    void setLastCenter(GesturePoint value) {mLastCenterPoint = value;}
    void setCenter(GesturePoint value) {mCenterPoint = value;}
	
	void setTotalRotationAngle(float value) {mTotalRotationAngle = value;}
    void setLastRotationAngle(float value) {mLastRotationAngle = value;}
    void setRotationAngle(float value) {mRotationAngle = value;}
	void setStartRotationAngle(float value) {mStartRotationAngle = value;}
	float calcRotationAngle();
    float calcStartRotationAngle();

private:
    GesturePoint mPoints[2];
    GesturePoint mStartPoints[2];
    GesturePoint mLastPoints[2];
    GesturePoint mStartCenterPoint;
    GesturePoint mLastCenterPoint;
    GesturePoint mCenterPoint;

    float mTotalRotationAngle;
    float mLastRotationAngle;
    float mRotationAngle;

	float mStartRotationAngle;
};
#endif // ROTATEGESTURE_H
