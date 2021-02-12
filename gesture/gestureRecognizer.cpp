#include "gestureRecognizer.h"
#include <sys/time.h>
#include <time.h>

GestureRecognizer::GestureRecognizer(GESTURE_TYPE gtype):
mIgnore(false),
mType(gtype),
mPressTime(0),
mCurEventTime(0)
{
}

GestureRecognizer::~GestureRecognizer()
{
}

unsigned long GestureRecognizer::getCurTime()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000 + tv.tv_usec);
}
