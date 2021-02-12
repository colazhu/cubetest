#include "rotateGestureRecognizer.h"
#include "rotateGesture.h"
#include "gestureCommonFun.h"
#include <stdlib.h>
#include <math.h>

RotateGestureRecognizer::RotateGestureRecognizer():
GestureRecognizer(GESTURE_TYPE_2ROTARY)
{
}

RotateGestureRecognizer::~RotateGestureRecognizer()
{
}

GestureRecognizer::ResultFlag RotateGestureRecognizer::recognize(GestureObject *state, int motionEventAction,
        struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion, void *data)
{
    ResultFlag retFlag = ResultFlag_Ignore;
    RotateGesture *ges = static_cast<RotateGesture *>(state);

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
            ges->setPoints(mtPoints[0].coords.x, mtPoints[0].coords.y, mtPoints[1].coords.x, mtPoints[1].coords.y);

			GesturePoint center;
			center.x = (mtPoints[0].coords.x + mtPoints[1].coords.x)/2;
			center.y = (mtPoints[0].coords.y + mtPoints[1].coords.y)/2;
			ges->setLastCenter(ges->getCenter());
			ges->setCenter(center);

			float rotationAngle = ges->calcRotationAngle();
			ges->setLastRotationAngle(ges->getRotationAngle());
			ges->setRotationAngle(rotationAngle);
			ges->setTotalRotationAngle(ges->getTotalRotationAngle() + rotationAngle - ges->getLastRotationAngle());

			ges->setState(GESTURE_STATE_FINISHED);
            retFlag = ResultFlag_Trigger;
        }
        else if (GESTURE_STATE_MAYBE == oldState) {
            ges->clearData();
        }

        return retFlag;
    }

    int touchType = motionEventAction & MOTION_EVENT_ACTION_MASK;
    switch (touchType) {
	case MOTION_EVENT_ACTION_DOWN:
		{
			if (2 == pointCount) {
                ges->setPoints(mtPoints[0].coords.x, mtPoints[0].coords.y, mtPoints[1].coords.x, mtPoints[1].coords.y);
                ges->setLastPoints(mtPoints[0].coords.x, mtPoints[0].coords.y, mtPoints[1].coords.x, mtPoints[1].coords.y);
                ges->setStartPoints(mtPoints[0].coords.x, mtPoints[0].coords.y, mtPoints[1].coords.x, mtPoints[1].coords.y);

                GesturePoint center;
				center.x = (mtPoints[0].coords.x + mtPoints[1].coords.x)/2;
				center.y = (mtPoints[0].coords.y + mtPoints[1].coords.y)/2;
				ges->setLastCenter(center);
				ges->setCenter(center);
				ges->setStartCenter(center);	

                ges->setLastRotationAngle(0);
                ges->setRotationAngle(0);
                ges->setTotalRotationAngle(0);
                ges->setStartRotationAngle(0);
				
                ges->setState(GESTURE_STATE_MAYBE);
			}
		}
		break;

	case MOTION_EVENT_ACTION_POINTER_DOWN:
		{
			if (2 == pointCount) {
				ges->setPoints(mtPoints[0].coords.x, mtPoints[0].coords.y, mtPoints[1].coords.x, mtPoints[1].coords.y);
                ges->setLastPoints(mtPoints[0].coords.x, mtPoints[0].coords.y, mtPoints[1].coords.x, mtPoints[1].coords.y);
                ges->setStartPoints(mtPoints[0].coords.x, mtPoints[0].coords.y, mtPoints[1].coords.x, mtPoints[1].coords.y);

				GesturePoint center;
				center.x = (mtPoints[0].coords.x + mtPoints[1].coords.x)/2;
				center.y = (mtPoints[0].coords.y + mtPoints[1].coords.y)/2;
				ges->setLastCenter(center);
				ges->setCenter(center);
				ges->setStartCenter(center);	

                ges->setLastRotationAngle(0);
                ges->setRotationAngle(0);
                ges->setTotalRotationAngle(0);
                ges->setStartRotationAngle(0);
				
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
					ges->setPoints(ponits[0]->coords.x, ponits[0]->coords.y, ponits[1]->coords.x, ponits[1]->coords.y);
                    ges->setLastPoints(mtPoints[0].coords.x, mtPoints[0].coords.y, mtPoints[1].coords.x, mtPoints[1].coords.y);
                    ges->setStartPoints(mtPoints[0].coords.x, mtPoints[0].coords.y, mtPoints[1].coords.x, mtPoints[1].coords.y);

					GesturePoint center;
					center.x = (ponits[0]->coords.x + ponits[1]->coords.x)/2;
					center.y = (ponits[0]->coords.y + ponits[1]->coords.y)/2;
					ges->setLastCenter(center);
					ges->setCenter(center);
					ges->setStartCenter(center);	

                    ges->setLastRotationAngle(0);
                    ges->setRotationAngle(0);
                    ges->setTotalRotationAngle(0);
                    ges->setStartRotationAngle(0);
					
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
	        if (2 == pointCount && (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState || GESTURE_STATE_MAYBE == oldState)) {
				GesturePoint center;
				center.x = (mtPoints[0].coords.x + mtPoints[1].coords.x)/2;
                center.y = (mtPoints[0].coords.y + mtPoints[1].coords.y)/2;
                GesturePoint* lastPoints = ges->getPoints();
                ges->setLastPoints(lastPoints[0].x, lastPoints[0].y, lastPoints[1].x, lastPoints[1].y);
                ges->setPoints(mtPoints[0].coords.x, mtPoints[0].coords.y, mtPoints[1].coords.x, mtPoints[1].coords.y);
                ges->setLastCenter(ges->getCenter());
                ges->setCenter(center);

                float rotationAngle = ges->calcRotationAngle();
                ges->setLastRotationAngle(ges->getRotationAngle());
                ges->setRotationAngle(rotationAngle);
                ges->setTotalRotationAngle(ges->getTotalRotationAngle() + rotationAngle);

				if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
					ges->setState(GESTURE_STATE_UPDATED);
	                retFlag = ResultFlag_Trigger;
				}
				else if (GESTURE_STATE_MAYBE == oldState) {
                    unsigned int angle = abs(ges->getTotalRotationAngle() + ges->calcStartRotationAngle() * (180/PI));
					if (RotateGestureTriggerAngleThreshold <= angle) {
                        ges->setStartRotationAngle(angle);
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
