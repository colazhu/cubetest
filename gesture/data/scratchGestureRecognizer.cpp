#include "scratchGestureRecognizer.h"
#include "scratchGesture.h"
#include "gestureCommonFun.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

ScratchGestureRecognizer::ScratchGestureRecognizer():
GestureRecognizer(GESTURE_TYPE_SCRATCH),
mStartPolygonArea(0),
mLastPolygonArea(0),
mStrachNum(0),
mIsNewSentence(true)
{
}

ScratchGestureRecognizer::~ScratchGestureRecognizer()
{
}

GestureRecognizer::ResultFlag ScratchGestureRecognizer::recognize(GestureObject *state, int motionEventAction,
		struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion, void *data)
{
	ResultFlag retFlag = ResultFlag_Ignore;
	ScratchGesture *ges = static_cast<ScratchGesture *>(state);
	if (!ges) {
		return retFlag;
	}

	GESTURE_STATE oldState = ges->getState();	
	if ((GESTURE_STATE_INVALID == oldState 
		|| GESTURE_STATE_CANCECLED == oldState
		|| GESTURE_STATE_FINISHED == oldState)
		&& !isIgnore()) {
		if (ScratchGestureRecognizeValidTime < (getCurTime() - getPressTime())) {
			setIgnore(true);
		}
	}

	if (isIgnore() || !mtPoints || 0 == pointCount) {
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

	int touchType = motionEventAction & MOTION_EVENT_ACTION_MASK;
	if(MOTION_EVENT_ACTION_DOWN != touchType
		&& !mIsNewSentence) {
		return retFlag;
	}

	switch (touchType) {
		case MOTION_EVENT_ACTION_DOWN:
			{
				mStartInfoVector.clear();
				mLastInfoVector.clear();
				mStartPolygonArea = 0;
				mLastPolygonArea = 0;
				mStrachNum = 0;
				mIsNewSentence = true;
				ges->setState(GESTURE_STATE_INVALID);

				if(SCRATCH_GESTURE_MIN_POINTS <= pointCount) {
					std::vector<struct deserialize_point_info> infoVector;
					if (vertexDeserialize(pointCount, mtPoints, infoVector)) {
						mStartInfoVector = infoVector;
						mLastInfoVector = infoVector;
						mStrachNum = 0;
						mStartPolygonArea = calcPolygonArea(infoVector);
						mLastPolygonArea = mStartPolygonArea;

						ges->updateGestureData(pointCount, mtPoints, true);
						ges->setState(GESTURE_STATE_MAYBE);

					}
				}
			}
			break;

		case MOTION_EVENT_ACTION_POINTER_DOWN:
			{
				if(GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
					ges->updateGestureData(pointCount, mtPoints, false);
					ges->setState(GESTURE_STATE_UPDATED);
					retFlag = ResultFlag_Trigger;
				}
				else {
					if(SCRATCH_GESTURE_MIN_POINTS <= pointCount) {
						std::vector<struct deserialize_point_info> infoVector;
						if (vertexDeserialize(pointCount, mtPoints, infoVector)) {
							mStartInfoVector = infoVector;
							mLastInfoVector = infoVector;
							mStrachNum = 0;
							mStartPolygonArea = calcPolygonArea(infoVector);
							mLastPolygonArea = mStartPolygonArea;

							ges->updateGestureData(pointCount, mtPoints, true);
							ges->setState(GESTURE_STATE_MAYBE);

						}
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
					if(SCRATCH_GESTURE_MIN_POINTS > pressNum) {
						ges->setState(GESTURE_STATE_FINISHED);
						mIsNewSentence = false;
					}
					else {
						ges->setState(GESTURE_STATE_UPDATED);
					}

					ges->updateGestureData(pointCount, mtPoints, false);
					retFlag = ResultFlag_Trigger;
				}
				else if (GESTURE_STATE_MAYBE == oldState) {
					if(SCRATCH_GESTURE_MIN_POINTS > pressNum) {
						ges->clearData();
					}
					else {
						std::vector<struct deserialize_point_info> infoVector;
						if (vertexDeserialize(pointCount, mtPoints, infoVector)) {
							mStartInfoVector = infoVector;
							mLastInfoVector = infoVector;
							mStrachNum = 0;
							mStartPolygonArea = calcPolygonArea(infoVector);
							mLastPolygonArea = mStartPolygonArea;

							ges->updateGestureData(pointCount, mtPoints, true);
							ges->setState(GESTURE_STATE_MAYBE);
						}
					}
				}
				else {
				}
			}
			break;

		case MOTION_EVENT_ACTION_MOVE:
			{
				if(GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
					ges->updateGestureData(pointCount, mtPoints, GESTURE_STATE_MAYBE == oldState);
					ges->setState(GESTURE_STATE_UPDATED);
					retFlag = ResultFlag_Trigger;
				}
				else if(GESTURE_STATE_MAYBE == oldState) {
					std::vector<struct deserialize_point_info> infoVector;
					if (vertexDeserialize(pointCount, mtPoints, infoVector)) {
						int polygonArea = calcPolygonArea(infoVector);
						int polygonRate = ((float)(mLastPolygonArea - polygonArea))/((float)mLastPolygonArea)*1000;
						int totalRate = ((float)(mStartPolygonArea - polygonArea))/((float)mStartPolygonArea)*1000;

						int perInsideNum = 0;
						int startInsideNum = 0;
						for (uint32_t i = 0; i<infoVector.size(); ++i) {
							if (insidePloygen(infoVector[i].x, infoVector[i].y, mLastInfoVector)) {
								++perInsideNum;
							}
							else {
							}

							if (insidePloygen(infoVector[i].x, infoVector[i].y, mStartInfoVector)) {
								++startInsideNum;
							}
							else {
							}
						}

						bool valid = false;
						if (((pointCount - 2) <= perInsideNum && 5 <= polygonRate) 
							|| ((pointCount - 1) <= perInsideNum && 2 <= polygonRate)) {
							mStrachNum += (5 > mStrachNum) ? 1 : 0;
						}
						else {
							if (0 < mStrachNum) {
								--mStrachNum;
							}
						}

						if ((pointCount - 2) <= startInsideNum && ((50 <= totalRate && (pointCount - 2) <= mStrachNum)
							|| (300 <= totalRate && 1 <= mStrachNum))) {
							if(!isMultiDrag(infoVector)) {
								valid = true;
							}
						}

						if (valid) {
							ges->updateGestureData(pointCount, mtPoints, false);
							ges->setState(GESTURE_STATE_STARTED);
							retFlag = ResultFlag_Trigger;
						}

						mLastInfoVector = infoVector;
						mLastPolygonArea = polygonArea;
					}
				}
				else {
				}
			}
			break;

		case MOTION_EVENT_ACTION_UP:
			{
				if(GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
					ges->updateGestureData(pointCount, mtPoints, false);
					ges->setState(GESTURE_STATE_FINISHED);
					retFlag = ResultFlag_Trigger;
				}
				else if(GESTURE_STATE_MAYBE == oldState) {
					ges->clearData();
					retFlag = ResultFlag_Ignore;
				}
				else {
				}
			}
			break;

		default:
			break;
	};

	return retFlag;
}

bool ScratchGestureRecognizer::vertexDeserialize(int n, struct MultiTouchPoint* p, std::vector<struct deserialize_point_info>& outVector)
{
	if (!p || n < 3) {
		return false;
	}

	struct deserialize_point_info center = { 0, 0, 0, 0 };
	for (int i = 0;i < n;++i) {
		center.x += p[i].coords.x;
		center.y += p[i].coords.y;
	}

	center.x /= n;
	center.y /= n;
	for (int i = 0; i < n; ++i) {
		struct deserialize_point_info info;
		info.index = i;
		info.x = p[i].coords.x;
		info.y = p[i].coords.y;
		info.angle = RADIAN_TO_DEGREE(atan2(p[i].coords.y - center.y, p[i].coords.x- center.x));

		std::vector<struct deserialize_point_info>::iterator iter;
		bool found = false;
		for(iter = outVector.begin(); iter != outVector.end(); ++iter) {
			if (iter->angle > info.angle) {
				found = true;
				break;
			}
		}

		if (!found) {
			outVector.push_back(info);
		}
		else {
			outVector.insert(iter, 1, info);
		}
	}

	return true;
}

int ScratchGestureRecognizer::cross(const deserialize_point_info &A, const deserialize_point_info &B, const deserialize_point_info &C) {	
    return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
}

bool ScratchGestureRecognizer::insidePloygen(int x, int y, std::vector<struct deserialize_point_info> inVector)
{
	int count = inVector.size();
	if (3 > count) {
		return false;
	}

	int i = 0;
	int j = 0;
    int count1 = 0;
    int count2 = 0;
	for (i = 0, j = count - 1; i < count;j = i++) {
		double value = (x - inVector[j].x) * (inVector[i].y - inVector[j].y)
			- (y - inVector[j].y) * (inVector[i].x - inVector[j].x);
		if (value > 0) {
			++count1;
		}
		else if (value < 0) {
			++count2;
		}
	}

    if (0 == count1 || 0 == count2) {
        return true;
    }

    return false;
}

int ScratchGestureRecognizer::calcPolygonArea(std::vector<struct deserialize_point_info> inVector)
{
	if (inVector.size() < 3) {
		return 0;
	}

	std::vector<struct deserialize_point_info>::iterator iter = inVector.begin();
	int count = inVector.size();

	int area = 0;
	deserialize_point_info temp = { 0, 0, 0, 0 };

	temp.x = 0;
	temp.y = 0;
	for (int i = 0; i < count-1; ++i) {
		area += cross(temp, inVector[i], inVector[i+1]);
    }

	area += cross(temp, iter[count-1], iter[0]);
	area = area / 2;

	return area > 0 ? area : -area;
}

float ScratchGestureRecognizer::interectionAngle(int x1, int y1, int x2, int y2)
{
	return 180/PI * acos((x1 * x2 + y1 * y2) / (sqrt(x1 * x1 + y1 * y1) * sqrt(x2 * x2 + y2 * y2)));
}

bool ScratchGestureRecognizer::isMultiDrag(std::vector<struct deserialize_point_info> inVector)
{
	for(uint32_t i = 0;i< inVector.size()-1;i++) {
		for(uint32_t j =i+1;j<inVector.size();j++) {
			if(0 == (abs(mStartInfoVector[i].x - inVector[i].x) +
				abs(mStartInfoVector[i].y - inVector[i].y)) ||
				0 == (abs(mStartInfoVector[j].x - inVector[j].x) +
				abs(mStartInfoVector[j].y - inVector[j].y)))
				continue;

			if(interectionAngle(mStartInfoVector[i].x - inVector[i].x,
				mStartInfoVector[i].y - inVector[i].y,
				mStartInfoVector[j].x - inVector[j].x,
				mStartInfoVector[j].y - inVector[j].y) > 60) {
				return false;
			}
		}
	}

	return true;
}
