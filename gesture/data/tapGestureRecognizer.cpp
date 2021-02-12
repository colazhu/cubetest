#include "tapGestureRecognizer.h"
#include "tapGesture.h"
#include "gestureCommonFun.h"
#include <stdlib.h>

TapGestureRecognizer::TapGestureRecognizer():
GestureRecognizer(GESTURE_TYPE_TAP)
{
}

TapGestureRecognizer::~TapGestureRecognizer()
{
}

GestureRecognizer::ResultFlag TapGestureRecognizer::recognize(GestureObject *state, int motionEventAction,
		struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion, void *data)
{
	ResultFlag retFlag = ResultFlag_Ignore;
	TapGesture *ges = static_cast<TapGesture *>(state);
	if (!ges) {
		return retFlag;
	}

	GESTURE_STATE oldState = ges->getState();
	if (isIgnore()) {
		if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
			ges->setState(GESTURE_STATE_CANCECLED);
			return ResultFlag_Ignore;
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
			ges->setState(GESTURE_STATE_CANCECLED);
			retFlag = ResultFlag_Ignore;
		}

		return retFlag;
	}

	if (!GestureCommonFun::insideRegion(pointCount, mtPoints, validRegion)) {
		if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
			ges->setPoint(mtPoints[0].coords.x, mtPoints[0].coords.y);
			ges->setState(GESTURE_STATE_CANCECLED);
			retFlag = ResultFlag_Ignore;
		}

		return retFlag;
	}

	int touchType = motionEventAction & MOTION_EVENT_ACTION_MASK;
	switch (touchType) {
	case MOTION_EVENT_ACTION_DOWN:
		{
			if (1 == pointCount) {
				ges->setPoint(mtPoints[0].coords.x, mtPoints[0].coords.y);
				ges->setStartPoint(mtPoints[0].coords.x, mtPoints[0].coords.y);
				ges->setState(GESTURE_STATE_STARTED);
				ges->updateTime(getCurEventTime());
				retFlag = ResultFlag_Ignore;
			}
		}
		break;

	case MOTION_EVENT_ACTION_UP:
		{
			if (TapGestureInterval < getCurEventTime() - ges->getTime()) {
				setIgnore(true);
				retFlag = ResultFlag_Ignore;
			}
			else if (1 == pointCount 
				&& (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState)) {
				ges->setPoint(mtPoints[0].coords.x, mtPoints[0].coords.y);
				ges->setState(GESTURE_STATE_FINISHED);
				retFlag = ResultFlag_Trigger;
			}
		}
		break;

	case MOTION_EVENT_ACTION_MOVE:
		{
			if (1 == pointCount 
				&& (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState)) {

				GesturePoint startPoint = ges->getStartPoint();
				unsigned int manhattanLength = abs(startPoint.x - mtPoints[0].coords.x) + abs(startPoint.y - mtPoints[0].coords.y);
				unsigned long interval = getCurEventTime() - ges->getTime();

				if (TapGestureRadius <= manhattanLength || TapGestureInterval < interval) {
					// ges->setState(GESTURE_STATE_CANCECLED);
					// retFlag = ResultFlag_Trigger;
					setIgnore(true);
					retFlag = ResultFlag_Ignore;
				}
				else {
				}
			}
			else {
				if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
					ges->setPoint(mtPoints[0].coords.x, mtPoints[0].coords.y);
					ges->setState(GESTURE_STATE_CANCECLED);
					retFlag = ResultFlag_Ignore;
				}
			}
		}
		break;

	case MOTION_EVENT_ACTION_POINTER_DOWN:
	case MOTION_EVENT_ACTION_POINTER_UP:
	default:
		{
			if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
				ges->setPoint(mtPoints[0].coords.x, mtPoints[0].coords.y);
				ges->setState(GESTURE_STATE_CANCECLED);
				retFlag = ResultFlag_Ignore;
			}
		}
		break;
	}

	return retFlag;
}

