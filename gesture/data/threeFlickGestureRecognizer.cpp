#include "threeFlickGestureRecognizer.h"
#include "threeFlickGesture.h"
#   include "gestureCommonFun.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

ThreeFlickGestureRecognizer::ThreeFlickGestureRecognizer():
GestureRecognizer(GESTURE_TYPE_3FLICK)
	,mIsNewSentence(true)
{
}

float ThreeFlickGestureRecognizer::intersect(int x1, int y1, int x2, int y2)
{
	if ((0 == x1 && 0 == y1) || (0 == x2 && 0 == y2)) return INVALID_ANGLE;
	return acos((x1 * x2 + y1 * y2) / (sqrt(x1 * x1 + y1 * y1) * sqrt(x2 * x2 + y2 * y2))) * 180 / PI;
}

ThreeFlickGestureRecognizer::~ThreeFlickGestureRecognizer()
{
}

GestureRecognizer::ResultFlag ThreeFlickGestureRecognizer::recognize(GestureObject *state, int motionEventAction,
        struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion, void *data)
{
	ResultFlag retFlag = ResultFlag_Ignore;
	ThreeFlickGesture *ges = static_cast<ThreeFlickGesture *>(state);
	if (!ges) {
		return retFlag;
	}

	GESTURE_STATE oldState = ges->getState();
	if ((GESTURE_STATE_INVALID == oldState 
		|| GESTURE_STATE_CANCECLED == oldState
		|| GESTURE_STATE_FINISHED == oldState)
		&& !isIgnore()) {
		if (ThreeFlickGestureRecognizeValidTime < (getCurTime() - getPressTime())) {
			setIgnore(true);
		}
	}

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
		if (GESTURE_STATE_MAYBE == oldState) {
			ges->clearData();
		}

		return retFlag;
	}

	int touchType = motionEventAction & MOTION_EVENT_ACTION_MASK;
    if(MOTION_EVENT_ACTION_DOWN != touchType
		&& !mIsNewSentence) {
		return retFlag;
	}

	switch (touchType) {
	case MOTION_EVENT_ACTION_DOWN:
		{
			mStartInfoVector.clear();
			ges->setState(GESTURE_STATE_INVALID);
			mIsNewSentence = true;
			if (THREE_FLICK_GESTURE_MIN_POINTS <= pointCount && THREE_FLICK_GESTURE_MAX_POINTS >= pointCount) {
					ges->initializeGestureData(mtPoints, pointCount);
					ges->setState(GESTURE_STATE_MAYBE);
			}
			else {
			}
		}
		break;

	case MOTION_EVENT_ACTION_POINTER_DOWN:
		{
			if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
				if (THREE_FLICK_GESTURE_MAX_POINTS >= pointCount) {
					ges->setState(GESTURE_STATE_UPDATED);
				}
				else {
					ges->setState(GESTURE_STATE_FINISHED);
					mIsNewSentence = false;
				}

				ges->updateGestureData(mtPoints, pointCount);
				retFlag = ResultFlag_Trigger;
			}
			else if (GESTURE_STATE_MAYBE == oldState) {
				if (THREE_FLICK_GESTURE_MAX_POINTS >= pointCount) {
					ges->initializeGestureData(mtPoints, pointCount);
				}
				else {
					ges->clearData();
				}
			}
			else {
				if (THREE_FLICK_GESTURE_MIN_POINTS <= pointCount && THREE_FLICK_GESTURE_MAX_POINTS >= pointCount) {
					ges->initializeGestureData(mtPoints, pointCount);
					ges->setState(GESTURE_STATE_MAYBE);
				}
			}
		}
		break;
	case MOTION_EVENT_ACTION_POINTER_UP:
		{
			int pressNum = 0;
			for (int i = 0; i < pointCount; i++) {
				if (TouchPointReleased != mtPoints[i].state) {
					pressNum++;
				}
			}

			if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
				if(THREE_FLICK_GESTURE_MIN_POINTS > pressNum) {
					ges->setState(GESTURE_STATE_FINISHED);
					mIsNewSentence = false;
				}
				else {
					ges->setState(GESTURE_STATE_UPDATED);
				}

				ges->updateGestureData(mtPoints, pointCount);
				retFlag = ResultFlag_Trigger;
			}
			else if (GESTURE_STATE_MAYBE == oldState) {
				if (THREE_FLICK_GESTURE_MIN_POINTS > pressNum) {
					ges->clearData();
				}
				else {
					ges->initializeGestureData(mtPoints, pointCount);
					ges->setState(GESTURE_STATE_MAYBE);
				}
			}
			else {
			}
		}
		break;
	case MOTION_EVENT_ACTION_MOVE:
		{
			if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
				ges->updateGestureData(mtPoints, pointCount);
				ges->setState(GESTURE_STATE_UPDATED);
				retFlag = ResultFlag_Trigger;
			}
			else if (GESTURE_STATE_MAYBE == oldState) {
				int num = pointCount > THREE_FLICK_GESTURE_MAX_POINTS ? THREE_FLICK_GESTURE_MAX_POINTS:pointCount;
				struct GesturePoint center;
				memset(&center, 0, sizeof(center));
				for (int i = 0;i < num;++i) {
					center.x += mtPoints[i].coords.x;
					center.y += mtPoints[i].coords.y;
				}

				center.x /= num;
				center.y /= num;
				unsigned int manhattanLength = abs(center.x- ges->getStartCenter().x) + abs(center.y - ges->getStartCenter().y);
				if (ThreeFlickGestureStartRadius < manhattanLength) {
					float angle = intersectBetweenFingers(ges, mtPoints, pointCount);
					if (INVALID_ANGLE == angle) {
						ges->updateGestureData(mtPoints, pointCount);
						return ResultFlag_Ignore;
					}
					if (ThreeFlickGestureAngleThreshold > angle) {
						ges->updateGestureData(mtPoints, pointCount);
						ges->setState(GESTURE_STATE_STARTED);
						retFlag = ResultFlag_Trigger;
					}
					else {
						ges->clearData();
						retFlag = ResultFlag_Ignore;
					}
				}
			}
		}
		break;
	case MOTION_EVENT_ACTION_UP:
		{
			if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
				ges->updateGestureData(mtPoints, pointCount);
				ges->setState(GESTURE_STATE_FINISHED);
				mIsNewSentence = false;
				retFlag = ResultFlag_Trigger;
			}
			else if (GESTURE_STATE_MAYBE == oldState) {
				ges->clearData();
			}
			else {
			}
		}
		break;

	default:
		break;
	}

	return retFlag;
}
float ThreeFlickGestureRecognizer::intersectBetweenFingers(GestureObject* obj, struct MultiTouchPoint *mtPoints, unsigned int pointCount)
{
	uint32_t i = 0;
	float maxAngle = INVALID_ANGLE;
	ThreeFlickGesture* ges = static_cast<ThreeFlickGesture*>(obj);
	for (; i < pointCount - 1; ++i) {
		for (uint32_t j = i + 1;j < pointCount; ++j) {
			if ((0 == mtPoints[i].coords.x - ges->getStartPoint(i).x && 
				0 == mtPoints[i].coords.y - ges->getStartPoint(i).y) || 
				(0 == mtPoints[j].coords.x - ges->getStartPoint(j).x && 
				0 == mtPoints[j].coords.y - ges->getStartPoint(j).y)) {
				return INVALID_ANGLE;
			}
			float angle = intersect(mtPoints[i].coords.x - ges->getStartPoint(i).x,
			mtPoints[i].coords.y - ges->getStartPoint(i).y,
			mtPoints[j].coords.x - ges->getStartPoint(j).x,
			mtPoints[j].coords.y - ges->getStartPoint(j).y);
			if (angle > maxAngle) {
				maxAngle = angle;
			}
		}
	}

	return maxAngle;
}
