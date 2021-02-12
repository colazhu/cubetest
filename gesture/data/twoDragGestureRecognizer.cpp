#include "twoDragGestureRecognizer.h"
#include "twoDragGesture.h"
#include "gestureCommonFun.h"
#include <stdlib.h>
#include <math.h>

TwoDragGestureRecognizer::TwoDragGestureRecognizer():
GestureRecognizer(GESTURE_TYPE_2DRAG)
{
}

TwoDragGestureRecognizer::~TwoDragGestureRecognizer()
{
}

GestureRecognizer::ResultFlag TwoDragGestureRecognizer::recognize(GestureObject *state, int motionEventAction,
		struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion, void *data)
{
	ResultFlag retFlag = ResultFlag_Ignore;
	TwoDragGesture *ges = static_cast<TwoDragGesture *>(state);
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

		return retFlag;
	}

	if (!GestureCommonFun::insideRegion(pointCount, mtPoints, validRegion)) {
		if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
			if (2 == pointCount) {
				ges->setPoint(0, mtPoints[0].coords.x, mtPoints[0].coords.y);
				ges->setPoint(1, mtPoints[1].coords.x, mtPoints[1].coords.y);
			}

			ges->setLastCenter(ges->getCenter().x, ges->getCenter().y);
			ges->setLastOffset(ges->getOffset().x, ges->getOffset().y);

			int centerX = (ges->getPoint(0).x + ges->getPoint(1).x)/2;
			int centerY = (ges->getPoint(0).y + ges->getPoint(1).y)/2;
			ges->setCenter(centerX, centerY);
			ges->setOffset(centerX - ges->getLastCenter().x, centerY - ges->getLastCenter().y);
						
			GesturePoint startOffset;
			startOffset.x = centerX - ges->getStartCenter().x;
			startOffset.y = centerY - ges->getStartCenter().y;
			ges->setStartOffset(startOffset.x, startOffset.y);
			
			ges->setState(GESTURE_STATE_FINISHED);
			retFlag = ResultFlag_Trigger;
		}

		return retFlag;
	}

	int touchType = motionEventAction & MOTION_EVENT_ACTION_MASK;
	switch (touchType) {
	case MOTION_EVENT_ACTION_DOWN:
	case MOTION_EVENT_ACTION_POINTER_DOWN:
		{
			if (2 == pointCount){
				int fingersDis = hypot(abs(mtPoints[0].coords.x - mtPoints[1].coords.x),  abs(mtPoints[0].coords.y - mtPoints[1].coords.y));
				if (fingersDis <= TwoFingersDistance) {
					ges->setPoint(0, mtPoints[0].coords.x, mtPoints[0].coords.y);
					ges->setPoint(1, mtPoints[1].coords.x, mtPoints[1].coords.y);
						
					int centerX = (mtPoints[0].coords.x + mtPoints[1].coords.x)/2;
					int centerY = (mtPoints[0].coords.y + mtPoints[1].coords.y)/2;
					ges->setCenter(centerX, centerY);
					ges->setLastCenter(centerX, centerY);
					ges->setStartCenter(centerX, centerY);

					ges->setOffset(0, 0);
					ges->setLastOffset(0, 0);
					ges->setStartOffset(0, 0);

					ges->setState(GESTURE_STATE_MAYBE);
					retFlag = ResultFlag_Ignore;
				}					
			}
			else {
				if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
					ges->setPoint(0, mtPoints[0].coords.x, mtPoints[0].coords.y);
					ges->setPoint(1, mtPoints[1].coords.x, mtPoints[1].coords.y);

					ges->setLastCenter(ges->getCenter().x, ges->getCenter().y);
					ges->setLastOffset(ges->getOffset().x, ges->getOffset().y);

					int centerX = (mtPoints[0].coords.x + mtPoints[1].coords.x)/2;
					int centerY = (mtPoints[0].coords.y + mtPoints[1].coords.y)/2;
					ges->setCenter(centerX, centerY);
					ges->setOffset(centerX - ges->getLastCenter().x, centerY - ges->getLastCenter().y);
						
					GesturePoint startOffset;
					startOffset.x = centerX - ges->getStartCenter().x;
					startOffset.y = centerY - ges->getStartCenter().y;
					ges->setStartOffset(startOffset.x, startOffset.y);

					ges->setState(GESTURE_STATE_FINISHED);
					retFlag = ResultFlag_Trigger;
				}
				else if (GESTURE_STATE_MAYBE == oldState) {
					ges->clearData();
					retFlag = ResultFlag_Ignore;
				}
			}
		}
		break;
	
	case MOTION_EVENT_ACTION_MOVE:
		{
			if (2 == pointCount) {
				if (GESTURE_STATE_MAYBE == oldState) {
					int fingersDis = hypot(abs(mtPoints[0].coords.x - mtPoints[1].coords.x),  abs(mtPoints[0].coords.y - mtPoints[1].coords.y));
					if (fingersDis > TwoFingersDistance) {
						ges->clearData();
						retFlag = ResultFlag_Ignore;
					}
					else {
						TwoDragGesture tmp;
						tmp.copyData(ges);
						
						tmp.setPoint(0, mtPoints[0].coords.x, mtPoints[0].coords.y);
						tmp.setPoint(1, mtPoints[1].coords.x, mtPoints[1].coords.y);

						tmp.setLastCenter(tmp.getCenter().x, tmp.getCenter().y);
						tmp.setLastOffset(tmp.getOffset().x, tmp.getOffset().y);

						int centerX = (mtPoints[0].coords.x + mtPoints[1].coords.x)/2;
						int centerY = (mtPoints[0].coords.y + mtPoints[1].coords.y)/2;
						tmp.setCenter(centerX, centerY);
						tmp.setOffset(centerX - tmp.getLastCenter().x, centerY - tmp.getLastCenter().y);
						
						GesturePoint startOffset;
						startOffset.x = centerX - tmp.getStartCenter().x;
						startOffset.y = centerY - tmp.getStartCenter().y;
						tmp.setStartOffset(startOffset.x, startOffset.y);
						
						if (startOffset.x > TwoDragGestureTriggerOffset || startOffset.x < (-TwoDragGestureTriggerOffset)
							|| startOffset.y > TwoDragGestureTriggerOffset || startOffset.y < (-TwoDragGestureTriggerOffset)) {
							tmp.setState(GESTURE_STATE_STARTED);
							ges->copyData(&tmp);
							retFlag = ResultFlag_Trigger;
						}
					}
				}
				else if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
					ges->setPoint(0, mtPoints[0].coords.x, mtPoints[0].coords.y);
					ges->setPoint(1, mtPoints[1].coords.x, mtPoints[1].coords.y);

					ges->setLastCenter(ges->getCenter().x, ges->getCenter().y);
					ges->setLastOffset(ges->getOffset().x, ges->getOffset().y);

					int centerX = (mtPoints[0].coords.x + mtPoints[1].coords.x)/2;
					int centerY = (mtPoints[0].coords.y + mtPoints[1].coords.y)/2;
					ges->setCenter(centerX, centerY);
					ges->setOffset(centerX - ges->getLastCenter().x, centerY - ges->getLastCenter().y);
						
					GesturePoint startOffset;
					startOffset.x = centerX - ges->getStartCenter().x;
					startOffset.y = centerY - ges->getStartCenter().y;
					ges->setStartOffset(startOffset.x, startOffset.y);

					int fingersDis = hypot(abs(mtPoints[0].coords.x - mtPoints[1].coords.x),  abs(mtPoints[0].coords.y - mtPoints[1].coords.y));
					if (fingersDis > TwoFingersDistance) {
						ges->setState(GESTURE_STATE_FINISHED);
					}
					else {					
						ges->setState(GESTURE_STATE_UPDATED);
					}
					retFlag = ResultFlag_Trigger;
				}
				else {
					int fingersDis = hypot(abs(mtPoints[0].coords.x - mtPoints[1].coords.x),  abs(mtPoints[0].coords.y - mtPoints[1].coords.y));
					if (fingersDis <= TwoFingersDistance) {
						ges->setPoint(0, mtPoints[0].coords.x, mtPoints[0].coords.y);
						ges->setPoint(1, mtPoints[1].coords.x, mtPoints[1].coords.y);
							
						int centerX = (mtPoints[0].coords.x + mtPoints[1].coords.x)/2;
						int centerY = (mtPoints[0].coords.y + mtPoints[1].coords.y)/2;
						ges->setCenter(centerX, centerY);
						ges->setLastCenter(centerX, centerY);
						ges->setStartCenter(centerX, centerY);

						ges->setOffset(0, 0);
						ges->setLastOffset(0, 0);
						ges->setStartOffset(0, 0);

						ges->setState(GESTURE_STATE_MAYBE);
						retFlag = ResultFlag_Ignore;
					}
				}
			}
		}
		break;
		
	case MOTION_EVENT_ACTION_POINTER_UP:
	case MOTION_EVENT_ACTION_UP:
		{
			if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
				if (2 == pointCount) {
					ges->setPoint(0, mtPoints[0].coords.x, mtPoints[0].coords.y);
					ges->setPoint(1, mtPoints[1].coords.x, mtPoints[1].coords.y);
				}
				else if (1 == pointCount) {
					ges->setPoint(0, mtPoints[0].coords.x, mtPoints[0].coords.y);
				}

				ges->setLastCenter(ges->getCenter().x, ges->getCenter().y);
				ges->setLastOffset(ges->getOffset().x, ges->getOffset().y);

				int centerX = (ges->getPoint(0).x + ges->getPoint(1).x)/2;
				int centerY = (ges->getPoint(0).y + ges->getPoint(1).y)/2;
				ges->setCenter(centerX, centerY);
				ges->setOffset(centerX - ges->getLastCenter().x, centerY - ges->getLastCenter().y);
						
				GesturePoint startOffset;
				startOffset.x = centerX - ges->getStartCenter().x;
				startOffset.y = centerY - ges->getStartCenter().y;
				ges->setStartOffset(startOffset.x, startOffset.y);
				
				ges->setState(GESTURE_STATE_FINISHED);
				retFlag = ResultFlag_Trigger;
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
					int fingersDis = hypot(abs(mtPoints[0].coords.x - mtPoints[1].coords.x),  abs(mtPoints[0].coords.y - mtPoints[1].coords.y));
					if (fingersDis <= TwoFingersDistance) {
						ges->setPoint(0, ponits[0]->coords.x, ponits[0]->coords.y);
						ges->setPoint(1, ponits[1]->coords.x, ponits[1]->coords.y);
							
						int centerX = (ponits[0]->coords.x + ponits[1]->coords.x)/2;
						int centerY = (ponits[0]->coords.y + ponits[1]->coords.y)/2;
						ges->setCenter(centerX, centerY);
						ges->setLastCenter(centerX, centerY);
						ges->setStartCenter(centerX, centerY);

						ges->setOffset(0, 0);
						ges->setLastOffset(0, 0);
						ges->setStartOffset(0, 0);

						ges->setState(GESTURE_STATE_MAYBE);
						retFlag = ResultFlag_Ignore;
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

