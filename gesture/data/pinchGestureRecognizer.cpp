#include "pinchGestureRecognizer.h"
#include "pinchGesture.h"
#include "gestureCommonFun.h"
#include <stdlib.h>

PinchGestureRecognizer::PinchGestureRecognizer():
GestureRecognizer(GESTURE_TYPE_PINCH)
{
}

PinchGestureRecognizer::~PinchGestureRecognizer()
{
}

GestureRecognizer::ResultFlag PinchGestureRecognizer::recognize(GestureObject *state, int motionEventAction,
        struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion,  void *data)
{
    ResultFlag retFlag = ResultFlag_Ignore;
    PinchGesture *ges = static_cast<PinchGesture *>(state);

    if (!ges) {
        return retFlag;
    }

    GESTURE_STATE oldState = ges->getState();
	if (isIgnore()) {
		if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
			ges->setState(GESTURE_STATE_FINISHED);
			return ResultFlag_Trigger;
		}
		else if (GESTURE_STATE_MAYBE == oldState) {
			ges->clearData();
			return retFlag;
		}
		else {
			return retFlag;
		}
	}

    if (!mtPoints) {
        if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
            ges->setState(GESTURE_STATE_FINISHED);
            retFlag = ResultFlag_Trigger;
        }
        else if (GESTURE_STATE_MAYBE == oldState) {
            ges->clearData();
        }

        return retFlag;
    }

    if (!GestureCommonFun::insideRegion(pointCount, mtPoints, validRegion)) {
        if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
			GesturePoint* lastPoints = ges->getPoints();
			ges->setLastPoints(lastPoints[0].x, lastPoints[0].y, lastPoints[1].x, lastPoints[1].y);
            ges->setPoints(mtPoints[0].coords.x, mtPoints[0].coords.y, mtPoints[1].coords.x, mtPoints[1].coords.y);

			GesturePoint center;
			center.x = (mtPoints[0].coords.x + mtPoints[1].coords.x)/2;
			center.y = (mtPoints[0].coords.y + mtPoints[1].coords.y)/2;
			ges->setLastCenterPoint(ges->getCenter());
			ges->setCenterPoint(center);
			
			float scaleFactor = ges->calcScaleFactor();
			ges->setLastScaleFactor(ges->getScaleFactor());
			ges->setScaleFactor(scaleFactor);
			ges->setTotalScaleFactor(ges->getTotalScaleFactor() * scaleFactor);

			float rotationAngle = ges->calcRotationAngle();
			ges->setLastRotationAngle(ges->getRotationAngle());
			ges->setRotationAngle(rotationAngle);
            ges->setTotalRotationAngle(ges->getTotalRotationAngle() + rotationAngle /*- ges->getLastRotationAngle()*/);

			ges->setState(GESTURE_STATE_FINISHED);
            retFlag = ResultFlag_Trigger;
        }
        else if (GESTURE_STATE_MAYBE == oldState) {
            ges->clearData();
        }

        return retFlag;
    }

    int touchType = motionEventAction & MOTION_EVENT_ACTION_MASK;
	static unsigned int s_stayCounter = 0;
    switch (touchType) {
	case MOTION_EVENT_ACTION_DOWN:
		{
			if (2 == pointCount) {
				ges->setLastPoints(mtPoints[0].coords.x, mtPoints[0].coords.y, mtPoints[1].coords.x, mtPoints[1].coords.y);
				ges->setPoints(mtPoints[0].coords.x, mtPoints[0].coords.y, mtPoints[1].coords.x, mtPoints[1].coords.y);
				ges->setStartPoints(mtPoints[0].coords.x, mtPoints[0].coords.y, mtPoints[1].coords.x, mtPoints[1].coords.y);
				
				GesturePoint center;
				center.x = (mtPoints[0].coords.x + mtPoints[1].coords.x)/2;
				center.y = (mtPoints[0].coords.y + mtPoints[1].coords.y)/2;
				ges->setLastCenterPoint(center);
				ges->setCenterPoint(center);
				ges->setStartCenterPoint(center);	

				ges->setLastScaleFactor(1.0);
				ges->setScaleFactor(1.0);
				ges->setTotalScaleFactor(1.0);

                ges->setLastRotationAngle(0);
                ges->setRotationAngle(0);
                ges->setTotalRotationAngle(0);
				s_stayCounter = 0;

                ges->setState(GESTURE_STATE_MAYBE);
			}
		}
		break;

	case MOTION_EVENT_ACTION_POINTER_DOWN:
		{
			if (2 == pointCount) {

				ges->setLastPoints(mtPoints[0].coords.x, mtPoints[0].coords.y, mtPoints[1].coords.x, mtPoints[1].coords.y);
				ges->setPoints(mtPoints[0].coords.x, mtPoints[0].coords.y, mtPoints[1].coords.x, mtPoints[1].coords.y);
				ges->setStartPoints(mtPoints[0].coords.x, mtPoints[0].coords.y, mtPoints[1].coords.x, mtPoints[1].coords.y);

				GesturePoint center;
				center.x = (mtPoints[0].coords.x + mtPoints[1].coords.x)/2;
				center.y = (mtPoints[0].coords.y + mtPoints[1].coords.y)/2;
				ges->setLastCenterPoint(center);
				ges->setCenterPoint(center);
				ges->setStartCenterPoint(center);	

				ges->setLastScaleFactor(1.0);
				ges->setScaleFactor(1.0);
				ges->setTotalScaleFactor(1.0);

                ges->setLastRotationAngle(0);
                ges->setRotationAngle(0);
                ges->setTotalRotationAngle(0);
				s_stayCounter = 0;
				
                ges->setState(GESTURE_STATE_MAYBE);
			}
			else {
				if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
					ges->setState(GESTURE_STATE_FINISHED);
                    retFlag = ResultFlag_Trigger;
				}
				else if (GESTURE_STATE_MAYBE == oldState) {
					ges->clearData();
				}
			}
		}
		break;

	case MOTION_EVENT_ACTION_POINTER_UP:
		{
			if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
				ges->setState(GESTURE_STATE_FINISHED);
                retFlag = ResultFlag_Trigger;
			}
			else if (GESTURE_STATE_MAYBE == oldState) {
				ges->clearData();
			}
			else {
				int pressNum = 0;
				struct MultiTouchPoint* ponits[2];
				for (int i = 0; i < pointCount; i++) {
					if (TouchPointReleased != mtPoints[i].state) {
						if (2 > pressNum) {
							ponits[pressNum] = &(mtPoints[i]);
						}
						
						pressNum++;
					}
				}

				if (2 == pressNum) {
					ges->setLastPoints(ponits[0]->coords.x, ponits[0]->coords.y, ponits[1]->coords.x, ponits[1]->coords.y);
					ges->setPoints(ponits[0]->coords.x, ponits[0]->coords.y, ponits[1]->coords.x, ponits[1]->coords.y);
					ges->setStartPoints(mtPoints[0].coords.x, mtPoints[0].coords.y, mtPoints[1].coords.x, mtPoints[1].coords.y);
				
					GesturePoint center;
					center.x = (ponits[0]->coords.x + ponits[1]->coords.x)/2;
					center.y = (ponits[0]->coords.y + ponits[1]->coords.y)/2;
					ges->setLastCenterPoint(center);
					ges->setCenterPoint(center);
					ges->setStartCenterPoint(center);	

					ges->setLastScaleFactor(1.0);
					ges->setScaleFactor(1.0);
					ges->setTotalScaleFactor(1.0);

                    ges->setLastRotationAngle(0);
                    ges->setRotationAngle(0);
                    ges->setTotalRotationAngle(0);
					s_stayCounter = 0;
					
	                ges->setState(GESTURE_STATE_MAYBE);
				}
			}				
		}
		break;

	case MOTION_EVENT_ACTION_UP:
		{
			if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
				ges->setState(GESTURE_STATE_FINISHED);
                retFlag = ResultFlag_Trigger;
			}
			else if (GESTURE_STATE_MAYBE == oldState) {
				ges->clearData();
			}
		}
		break;

	case MOTION_EVENT_ACTION_MOVE:
		{
			if(2 == pointCount) {
				if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
					GesturePoint* lastPoints = ges->getPoints();
					ges->setLastPoints(lastPoints[0].x, lastPoints[0].y, lastPoints[1].x, lastPoints[1].y);
					ges->setPoints(mtPoints[0].coords.x, mtPoints[0].coords.y, mtPoints[1].coords.x, mtPoints[1].coords.y);

					GesturePoint center;
					center.x = (mtPoints[0].coords.x + mtPoints[1].coords.x)/2;
					center.y = (mtPoints[0].coords.y + mtPoints[1].coords.y)/2;
					ges->setLastCenterPoint(ges->getCenter());
					ges->setCenterPoint(center);

					float scaleFactor = ges->calcScaleFactor();
					ges->setLastScaleFactor(ges->getScaleFactor());
					ges->setScaleFactor(scaleFactor);
					ges->setTotalScaleFactor(ges->getTotalScaleFactor() * scaleFactor);

					float rotationAngle = ges->calcRotationAngle();
					ges->setLastRotationAngle(ges->getRotationAngle());
					ges->setRotationAngle(rotationAngle);
					ges->setTotalRotationAngle(ges->getTotalRotationAngle() + rotationAngle );
				
					ges->setState(GESTURE_STATE_UPDATED);
	                retFlag = ResultFlag_Trigger;
				}
				else if (GESTURE_STATE_MAYBE == oldState) {
					GesturePoint* startPoints = ges->getStartPoints();
					unsigned int manhattanLength0 = abs(startPoints[0].x - mtPoints[0].coords.x) + abs(startPoints[0].y - mtPoints[0].coords.y);
					unsigned int manhattanLength1 = abs(startPoints[1].x - mtPoints[1].coords.x) + abs(startPoints[1].y - mtPoints[1].coords.y);
					if ((PinchGestureFingerOffset < manhattanLength0 || PinchGestureFingerOffset < manhattanLength1)) {
						if (0 != manhattanLength0 && 0 != manhattanLength1) {
							if (PinchGestureTriggerAngleThreshold >= ges->calcIntersectAngle(int(mtPoints[0].coords.x) - startPoints[0].x,
								int(mtPoints[0].coords.y) - startPoints[0].y,
								int(mtPoints[1].coords.x) - startPoints[1].x,
								int(mtPoints[1].coords.y) - startPoints[1].y)) {
								ges->clearData();
								return retFlag;
							}
						}
						else {
							if (3 >= ++s_stayCounter) {
								return retFlag;
							}
						}

						GesturePoint* lastPoints = ges->getPoints();
						ges->setLastPoints(lastPoints[0].x, lastPoints[0].y, lastPoints[1].x, lastPoints[1].y);
						ges->setPoints(mtPoints[0].coords.x, mtPoints[0].coords.y, mtPoints[1].coords.x, mtPoints[1].coords.y);

						GesturePoint center;
						center.x = (mtPoints[0].coords.x + mtPoints[1].coords.x)/2;
						center.y = (mtPoints[0].coords.y + mtPoints[1].coords.y)/2;
						ges->setLastCenterPoint(ges->getCenter());
						ges->setCenterPoint(center);

						float scaleFactor = ges->calcScaleFactor();
						ges->setLastScaleFactor(ges->getScaleFactor());
						ges->setScaleFactor(scaleFactor);
						ges->setTotalScaleFactor(ges->getTotalScaleFactor() * scaleFactor);

						float rotationAngle = ges->calcRotationAngle();
						ges->setLastRotationAngle(ges->getRotationAngle());
						ges->setRotationAngle(rotationAngle);
                    	ges->setTotalRotationAngle(ges->getTotalRotationAngle() + rotationAngle);

						ges->setState(GESTURE_STATE_STARTED);
						retFlag = ResultFlag_Trigger;
					}
				}
			}
		}
		break;

    default:
        break;
    }

    return retFlag;
}

