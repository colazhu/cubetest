#include "twoFlickGestureRecognizer.h"
#include "twoFlickGesture.h"
#include "gestureCommonFun.h"
#include <stdlib.h>
#include <math.h>

TwoFlickGestureRecognizer::TwoFlickGestureRecognizer():
GestureRecognizer(GESTURE_TYPE_2FLICK)
{
}

TwoFlickGestureRecognizer::~TwoFlickGestureRecognizer()
{
}

GestureRecognizer::ResultFlag TwoFlickGestureRecognizer::recognize(GestureObject *state, int motionEventAction,
		struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion, void *data)
{	
	ResultFlag retFlag = ResultFlag_Ignore;
	TwoFlickGesture *ges = static_cast<TwoFlickGesture *>(state);
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
		if (GESTURE_STATE_MAYBE ==oldState) {
			ges->clearData();
		}
		
		return retFlag;
	}
	if (!GestureCommonFun::insideRegion(pointCount, mtPoints, validRegion)) {
		if (GESTURE_STATE_MAYBE == oldState) {
			ges->clearData();
		}
		
		return retFlag;
	}

	int touchType = motionEventAction & MOTION_EVENT_ACTION_MASK;

	switch (touchType) {
	case MOTION_EVENT_ACTION_DOWN:
	case MOTION_EVENT_ACTION_POINTER_DOWN:
	    {
	        if (2 == pointCount) {
	            ges->resetTraceArray(0, 0, getCurEventTime());
	            ges->setState(GESTURE_STATE_MAYBE);
	        }
	        else {
	            if (GESTURE_STATE_MAYBE == ges->getState()) {
	                ges->clearData();
	            }
	        }
	    }
		break;
	
	case MOTION_EVENT_ACTION_MOVE:
		{
			if (2 == pointCount) {
				int fingersDis = hypot(abs(mtPoints[0].coords.x - mtPoints[1].coords.x),  abs(mtPoints[0].coords.y - mtPoints[1].coords.y));
				if (TwoFingersDistance < fingersDis) {
					if (GESTURE_STATE_MAYBE == oldState) {
						ges->clearData();
					}
				}
				else {
					GesturePoint center;
					center.x = (mtPoints[0].coords.x + mtPoints[1].coords.x) / 2;
					center.y = (mtPoints[0].coords.y + mtPoints[1].coords.y) / 2;

					if (GESTURE_STATE_MAYBE == oldState) {
						ges->updateTraceArray(center.x, center.y, getCurEventTime());
					}
				}
			}
		}
		break;

    case MOTION_EVENT_ACTION_POINTER_UP:
        {
            int pressNum = 0;
            for (int i = 0;i < pointCount; ++i) {
                if (TouchPointReleased != mtPoints[i].state) {
                    ++pressNum;
                }
            }

            if (2 == pressNum) {
                ges->resetTraceArray(0, 0, getCurEventTime());
                ges->setState(GESTURE_STATE_MAYBE);
            }
            else if (2 > pressNum) {
                if (GESTURE_STATE_MAYBE == oldState) {
                    unsigned long pointerUpInterval = ges->pointerUpInterval(getCurTime());

					if (TwoFlickGesturePointerUpInterval > pointerUpInterval) {
                        ges->clearData();
                    }
                    else {
                        if (TwoFlickGestureActionSpeed <= ges->getSpeed()) {
							ges->setPoint(0, mtPoints[0].coords.x, mtPoints[0].coords.y);
	                   	 	ges->setPoint(1, mtPoints[1].coords.x, mtPoints[1].coords.y);
	                    	ges->setCenter((mtPoints[0].coords.x + mtPoints[1].coords.x)/2, (mtPoints[0].coords.y + mtPoints[1].coords.y)/2);
							ges->setState(GESTURE_STATE_FINISHED);
                            retFlag = ResultFlag_Trigger;
                        }
                        else {
                            ges->clearData();
                        }
                    }
                }
            }
            else {

            }
        }
        break;

	case MOTION_EVENT_ACTION_UP:
		{
			if (GESTURE_STATE_MAYBE == oldState) {
                unsigned  long pointerUpInterval = ges->pointerUpInterval(getCurEventTime());
                if (TwoFlickGesturePointerUpInterval > pointerUpInterval) {
					ges->clearData();
				}
				else {		
                    if (TwoFlickGestureActionSpeed <= ges->getSpeed()) {
						ges->setPoint(0, mtPoints[0].coords.x, mtPoints[0].coords.y);
						ges->setPoint(1, mtPoints[1].coords.x, mtPoints[1].coords.y);
						ges->setCenter((mtPoints[0].coords.x + mtPoints[1].coords.x)/2, (mtPoints[0].coords.y + mtPoints[1].coords.y)/2);

						ges->setState(GESTURE_STATE_FINISHED);
						retFlag = ResultFlag_Trigger;
					}
					else {
						ges->clearData();
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

