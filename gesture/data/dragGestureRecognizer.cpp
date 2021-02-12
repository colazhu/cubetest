#include "dragGestureRecognizer.h"
#include "dragGesture.h"
#include "gestureCommonFun.h"
#include <stdlib.h>
#include <math.h>

DragGestureRecognizer::DragGestureRecognizer():
GestureRecognizer(GESTURE_TYPE_DRAG)
{
}

DragGestureRecognizer::~DragGestureRecognizer()
{
}

GestureRecognizer::ResultFlag DragGestureRecognizer::recognize(GestureObject *state, int motionEventAction,
        struct MultiTouchPoint * mtPoints, int pointCount, GestureRegion* validRegion,  void *data)
{
    ResultFlag retFlag = ResultFlag_Ignore;
    DragGesture *ges = static_cast<DragGesture *>(state);
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

    if (!mtPoints || 0 == pointCount) {
        if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
            ges->setState(GESTURE_STATE_FINISHED);
            retFlag = ResultFlag_Trigger;
        }
		else if(GESTURE_STATE_MAYBE == oldState) {
            ges->clearData();
            retFlag = ResultFlag_Ignore;
        }

        return retFlag;
    }

    if (!GestureCommonFun::insideRegion(pointCount, mtPoints, validRegion)) {
        if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
            ges->setLastPos(ges->getPoint().x, ges->getPoint().y);
            ges->setPoint(mtPoints[0].coords.x, mtPoints[0].coords.y);
            ges->setState(GESTURE_STATE_FINISHED);
            retFlag = ResultFlag_Trigger;
        }
		else if(GESTURE_STATE_MAYBE == oldState) {
            ges->clearData();
            retFlag = ResultFlag_Ignore;
        }

        return retFlag;
    }

    int touchType = motionEventAction & MOTION_EVENT_ACTION_MASK;
    switch (touchType) {
    case MOTION_EVENT_ACTION_DOWN:
        {
            if(1 == pointCount) {
                ges->setStartPos(mtPoints[0].coords.x, mtPoints[0].coords.y);
                ges->setLastPos(mtPoints[0].coords.x, mtPoints[0].coords.y);
                ges->setPoint(mtPoints[0].coords.x, mtPoints[0].coords.y);
                ges->setState(GESTURE_STATE_MAYBE);
                retFlag = ResultFlag_Trigger;
            }
        }
        break;

	case MOTION_EVENT_ACTION_POINTER_DOWN:
    case MOTION_EVENT_ACTION_UP:
        {
            if(GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
                ges->setLastPos(ges->getPoint().x, ges->getPoint().y);
                ges->setPoint(mtPoints[0].coords.x, mtPoints[0].coords.y);
                ges->setState(GESTURE_STATE_FINISHED);
                retFlag = ResultFlag_Trigger;
            }
            else if(GESTURE_STATE_MAYBE == oldState) {
                ges->clearData();
                retFlag = ResultFlag_Ignore;
            }
        }
        break;
		
    case MOTION_EVENT_ACTION_POINTER_UP:
        {
            int pressNum = 0;
            struct MultiTouchPoint* point;
            for (int i = 0; i < pointCount; i++) {
                if (TouchPointReleased != mtPoints[i].state) {
                    if (1 > pressNum) {
                        point = &(mtPoints[i]);
                    }
                    pressNum++;
                }
            }

            if (1 == pressNum) {
                ges->setStartPos(point->coords.x, point->coords.y);
                ges->setLastPos(point->coords.x, point->coords.y);
                ges->setPoint(point->coords.x, point->coords.y);
                ges->setState(GESTURE_STATE_MAYBE);
                retFlag = ResultFlag_Ignore;
            }
        }
        break;
		
    case MOTION_EVENT_ACTION_MOVE:
        {
            if(1 == pointCount) {
                if (GESTURE_STATE_MAYBE == oldState) {
                    GesturePoint startPos = ges->startPos();
                    int distance = hypot(abs(startPos.x - mtPoints[0].coords.x), abs(startPos.y - mtPoints[0].coords.y));
                    if(DragGestureTriggerOffset >= fabs(float(distance))) {
                        retFlag = ResultFlag_Ignore;
                    }
                    else {
                        ges->setLastPos(ges->getPoint().x, ges->getPoint().y);
                        ges->setPoint(mtPoints[0].coords.x, mtPoints[0].coords.y);
						ges->setState(GESTURE_STATE_STARTED);
                        retFlag = ResultFlag_Trigger;
                    }
                }
                else if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
                    ges->setLastPos(ges->getPoint().x, ges->getPoint().y);
                    ges->setPoint(mtPoints[0].coords.x, mtPoints[0].coords.y);
                    ges->setState(GESTURE_STATE_UPDATED);
                    retFlag = ResultFlag_Trigger;
                }
            }
        }
        break;
    
    default:
        break;
    }
	
    return retFlag;
}

