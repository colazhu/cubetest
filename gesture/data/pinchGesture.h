#ifndef PINCHGESTURE_H
#define PINCHGESTURE_H

#include "gestureObject.h"

class PinchGesture:public GestureObject
{
#ifndef RADIAN_TO_DEGREE
#define RADIAN_TO_DEGREE(x) (x) < 0 ? (360 + 57.2958*(x)) : (57.2958*(x))
#endif

public:
    PinchGesture();
    virtual ~PinchGesture();
    virtual bool copyData(GestureObject* object);
    virtual void clearData();

    //get propety
    GesturePoint* getPoints();
    GesturePoint getStartCenter() const {return mStartCenterPoint;}
    GesturePoint getLastCenter() const {return mLastCenterPoint;}
    GesturePoint getCenter() const {return mCenterPoint;}

    float getTotalScaleFactor() const {return mTotalScaleFactor;}
    float getLastScaleFactor() const {return mLastScaleFactor;}
    float getScaleFactor() const {return mScaleFactor;}

    float getTotalRotationAngle() const {return mTotalRotationAngle;}
    float getLastRotationAngle() const {return mLastRotationAngle;}
    float getRotationAngle() const {return mRotationAngle;}

    //set propety
    void setPoints(int x1, int y1, int x2, int y2);
	void setLastPoints(int x1, int y1, int x2, int y2);
	void setStartPoints(int x1, int y1, int x2, int y2);
	
    GesturePoint* getStartPoints() {return mStartPoints;}
    GesturePoint* getLastPoints() {return mLastPoints;}
	
    void setStartCenterPoint(GesturePoint value) {mStartCenterPoint = value;}
    void setLastCenterPoint(GesturePoint value) {mLastCenterPoint = value;}
    void setCenterPoint(GesturePoint value) {mCenterPoint = value;}
	
    void setTotalScaleFactor(float value) {mTotalScaleFactor = value;}
    void setLastScaleFactor(float value) {mLastScaleFactor = value;}
    void setScaleFactor(float value) {mScaleFactor = value;}
	
    void setTotalRotationAngle(float value) {mTotalRotationAngle = value;}
    void setLastRotationAngle(float value) {mLastRotationAngle = value;}
    void setRotationAngle(float value) {mRotationAngle = value;}

	float calcScaleFactor();
	float calcRotationAngle();
	float calcIntersectAngle(int x, int y, int x1, int y1);

private:
    GesturePoint mPoints[2];
	GesturePoint mLastPoints[2];
	GesturePoint mStartPoints[2];
    GesturePoint mStartCenterPoint;
    GesturePoint mLastCenterPoint;
    GesturePoint mCenterPoint;

    float mTotalScaleFactor;
    float mLastScaleFactor;
    float mScaleFactor;

    float mTotalRotationAngle;
    float mLastRotationAngle;
    float mRotationAngle;
};
#endif // PINCHGESTURE_H
