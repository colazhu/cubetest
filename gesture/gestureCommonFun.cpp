#include "gestureCommonFun.h"
#include "malloc.h"
#include <string.h>
#include "tapGesture.h"
#include "scratchGesture.h"
#include "dragGesture.h"
#include "pinchGesture.h"
#include "flickGesture.h"
#include "doubleClickGesture.h"
#include "rotateGesture.h"
#include "twoDragGesture.h"
#include "twoFlickGesture.h"
#include "threeFlickGesture.h"
#include "longPressGesture.h"
#include "twoLongPressGesture.h"
#include "twoTapGesture.h"
#include "multiLongPressGesture.h"
#include <sys/syscall.h>
#include <sys/time.h>

unsigned long GestureCommonFun::currentTime()
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return time.tv_sec * 1000 + time.tv_nsec / 1000000;

//    struct timeval tv;
//    gettimeofday(&tv, NULL);
//    unsigned long curTime = (tv.tv_sec * 1000000 + tv.tv_usec)/1000;
//    return curTime;
}

GESTURE_TYPE GestureCommonFun::gestureTypeConvert(WL_GESTURE_CLASS gclass, int separateType)
{
    GESTURE_TYPE ret = GESTURE_TYPE_UNKNOWN;
	if (WL_GESTURE_CLASS_SYSTEM == gclass) {
		switch (separateType) {
			case WL_SYSTEM_GESTURE_TYPE_FLICK:
				ret = GESTURE_TYPE_3FLICK;
				break;
			case WL_SYSTEM_GESTURE_TYPE_SCRATCH:
				ret = GESTURE_TYPE_SCRATCH;
				break;
            case WL_SYSTEM_GESTURE_TYPE_MULTILONGPRESS:
                ret = GESTURE_TYPE_MULTILONGPRESS;
                break;
			default:
				break;
		};
	}
	else {
		switch (separateType) {
		case WL_COMMON_GESTURE_TYPE_TAP:
			ret = GESTURE_TYPE_TAP;
			break;
        case WL_COMMON_GESTURE_TYPE_DRAG:
            ret = GESTURE_TYPE_DRAG;
            break;
        case WL_COMMON_GESTURE_TYPE_FLICK:
            ret = GESTURE_TYPE_FLICK;
            break;
        case WL_COMMON_GESTURE_TYPE_DOUBLECLICK:
            ret = GESTURE_TYPE_DOUBLECLICK;
            break;
        case WL_COMMON_GESTURE_TYPE_PINCH:
            ret = GESTURE_TYPE_PINCH;
            break;
        case WL_COMMON_GESTURE_TYPE_2FLICK:
            ret = GESTURE_TYPE_2FLICK;
            break;
        case WL_COMMON_GESTURE_TYPE_2DRAG:
            ret = GESTURE_TYPE_2DRAG;
            break;
        case WL_COMMON_GESTURE_TYPE_2ROTARY:
            ret = GESTURE_TYPE_2ROTARY;
            break;
        case WL_COMMON_GESTURE_TYPE_LONGPRESS:
            ret = GESTURE_TYPE_LONGPRESS;
            break;
        case WL_COMMON_GESTURE_TYPE_2LONGPRESS:
            ret = GESTURE_TYPE_2LONGPRESS;
            break;
		case WL_COMMON_GESTURE_TYPE_2TAP:
            ret = GESTURE_TYPE_2TAP;
            break;
        default:
            break;
		};
	}
	
	return ret;
}

bool GestureCommonFun::insideRegion(uint32_t count, struct MultiTouchPoint* points, GestureRegion* region)
{
    if (!points || !region) {
        return false;
    }

    for (uint32_t i = 0; i < count; i++) {
        if (!region->contain((int)(points[i].coords.x),
                           (int)(points[i].coords.y))) {
            return false;
        }
//		if (!pixman_region32_contains_point(region,
//						   (int)(points[i].coords.x),
//						   (int)(points[i].coords.y),
//						   NULL)) {
//			return false;
//		}
    }

    return true;
}

bool GestureCommonFun::insideRegion(uint32_t count, GesturePoint* points, GestureRegion* region)
{
    if (!points) {
        return false;
    }

    for (uint32_t i = 0; i < count; i++) {
        if (!region->contain(points[i].x, points[i].y)) {
            return false;
        }

//		if (!pixman_region32_contains_point(region,
//						   points[i].x,
//						   points[i].y,
//						   NULL)) {
//			return false;
//		}
    }

    return true;
}

#if 1
static Gesture_Event_Data*
tapGestureToData(TapGesture* gesture)
{
	if (!gesture) {
		return 0;
	}

    unsigned int size = sizeof(Gesture_Event_Data) + sizeof(wl_tap_gesture_data);
    Gesture_Event_Data* event =reinterpret_cast<Gesture_Event_Data*>(malloc(size));
	if (!event) {
		return 0;
	}

	memset(event, 0, size);
	event->gclass = WL_GESTURE_CLASS_COMMON;
	event->gtype = WL_COMMON_GESTURE_TYPE_TAP;
	event->len = sizeof(wl_tap_gesture_data);

	wl_tap_gesture_data* data = reinterpret_cast<wl_tap_gesture_data*>(event->data);
	data->pos.x = gesture->getPoint().x;
	data->pos.y = gesture->getPoint().y;
	event->state = gesture->getState();

	return event;
}

static Gesture_Event_Data*
flickGestureToData(FlickGesture* gesture)
{
    if (!gesture) {
        return 0;
    }

    unsigned int size = sizeof(Gesture_Event_Data) + sizeof(wl_flick_gesture_data);
    Gesture_Event_Data* event =reinterpret_cast<Gesture_Event_Data*>(malloc(size));
    if (!event) {
        return 0;
    }

    memset(event, 0, size);
    event->gclass = WL_GESTURE_CLASS_COMMON;
    event->gtype = WL_COMMON_GESTURE_TYPE_FLICK;
    event->len = sizeof(wl_flick_gesture_data);

    wl_flick_gesture_data* data = reinterpret_cast<wl_flick_gesture_data*>(event->data);
    data->speed = gesture->getSpeed();
    data->angle = gesture->getAngle();
    data->pos.x = gesture->getPoint().x;
    data->pos.y = gesture->getPoint().y;
    event->state = gesture->getState();
    return event;
}

static Gesture_Event_Data*
dragGestureToData(DragGesture* gesture)
{
    if (!gesture) {
        return 0;
    }

    unsigned int size = sizeof(Gesture_Event_Data) + sizeof(wl_drag_gesture_data);
    Gesture_Event_Data* event =reinterpret_cast<Gesture_Event_Data*>(malloc(size));
    if (!event) {
        return 0;
    }

    memset(event, 0, size);
    event->gclass = WL_GESTURE_CLASS_COMMON;
    event->gtype = WL_COMMON_GESTURE_TYPE_DRAG;
    event->len = sizeof(wl_drag_gesture_data);

    wl_drag_gesture_data* data = reinterpret_cast<wl_drag_gesture_data*>(event->data);

    data->startPos.x = gesture->startPos().x;
    data->startPos.y = gesture->startPos().y;
    data->lastPos.x = gesture->lastPos().x;
    data->lastPos.y = gesture->lastPos().y;
    data->pos.x = gesture->getPoint().x;
    data->pos.y = gesture->getPoint().y;
    event->state = gesture->getState();
    return event;
}

static Gesture_Event_Data*
pinchGestureToData(PinchGesture* gesture)
{
    if (!gesture) {
        return 0;
    }

    unsigned int size = sizeof(Gesture_Event_Data) + sizeof(wl_pinch_gesture_data);
    Gesture_Event_Data* event =reinterpret_cast<Gesture_Event_Data*>(malloc(size));
    if (!event) {
        return 0;
    }

    memset(event, 0, size);
    event->gclass = WL_GESTURE_CLASS_COMMON;
    event->gtype = WL_COMMON_GESTURE_TYPE_PINCH;
    event->len = sizeof(wl_pinch_gesture_data);
	event->state = gesture->getState();
	
    wl_pinch_gesture_data* data = reinterpret_cast<wl_pinch_gesture_data*>(event->data);

    data->center.x = gesture->getCenter().x;
    data->center.y = gesture->getCenter().y;
    data->lastCenter.x = gesture->getLastCenter().x;
    data->lastCenter.y = gesture->getLastCenter().y;
    data->startCenter.x = gesture->getStartCenter().x;
    data->startCenter.y = gesture->getStartCenter().y;

    data->rotationAngle = gesture->getRotationAngle();
    data->lastRotationAngle = gesture->getLastRotationAngle();
    data->totalRotationAngle = gesture->getTotalRotationAngle();

    data->scaleFactor = gesture->getScaleFactor();
    data->lastScaleFactor = gesture->getLastScaleFactor();
    data->totalScaleFactor = gesture->getTotalScaleFactor();

    GesturePoint* points;
    points = gesture->getPoints();
    for (int i = 0; i < 2; i++) {
        data->pos[i].x = points[i].x;
        data->pos[i].y = points[i].y;
    }

    return event;
}

static Gesture_Event_Data*
rotateGestureToData(RotateGesture* gesture)
{
    if (!gesture) {
        return 0;
    }

    unsigned int size = sizeof(Gesture_Event_Data) + sizeof(wl_2rotary_gesture_data);
    Gesture_Event_Data* event =reinterpret_cast<Gesture_Event_Data*>(malloc(size));
    if (!event) {
        return 0;
    }

    memset(event, 0, size);
    event->gclass = WL_GESTURE_CLASS_COMMON;
    event->gtype = WL_COMMON_GESTURE_TYPE_2ROTARY;
    event->len = sizeof(wl_2rotary_gesture_data);
	event->state = gesture->getState();

    wl_2rotary_gesture_data* data = reinterpret_cast<wl_2rotary_gesture_data*>(event->data);

    data->center.x = gesture->getCenter().x;
    data->center.y = gesture->getCenter().y;
    data->lastCenter.x = gesture->getLastCenter().x;
    data->lastCenter.y = gesture->getLastCenter().y;
    data->startCenter.x = gesture->getStartCenter().x;
    data->startCenter.y = gesture->getStartCenter().y;

    data->rotationAngle = gesture->getRotationAngle();
    data->lastRotationAngle = gesture->getLastRotationAngle();
    data->totalRotationAngle = gesture->getTotalRotationAngle();

    GesturePoint* points;
    points = gesture->getPoints();
    for (int i = 0; i < 2; i++) {
        data->pos[i].x = points[i].x;
        data->pos[i].y = points[i].y;
    }

    return event;
}

static Gesture_Event_Data*
twoFlickGestureToData(TwoFlickGesture* gesture)
{
	if (!gesture) {
		return 0;
	}

    unsigned int size = sizeof(Gesture_Event_Data) + sizeof(wl_2flick_gesture_data);
    Gesture_Event_Data* event =reinterpret_cast<Gesture_Event_Data*>(malloc(size));
	if (!event) {
		return 0;
	}

	memset(event, 0, size);
	event->gclass = WL_GESTURE_CLASS_COMMON;
	event->gtype = WL_COMMON_GESTURE_TYPE_2FLICK;
	event->len = sizeof(wl_2flick_gesture_data);
	event->state = gesture->getState();

	wl_2flick_gesture_data* data = reinterpret_cast<wl_2flick_gesture_data*>(event->data);

	data->pos[0].x = gesture->getPoint(0).x;
	data->pos[0].y = gesture->getPoint(0).y;
	data->pos[1].x = gesture->getPoint(1).x;
	data->pos[1].y = gesture->getPoint(1).y;

	data->center.x = gesture->getCenter().x;
	data->center.y = gesture->getCenter().y;

	data->speed = gesture->getSpeed();
	data->angle = gesture->getAngle();
	
	return event;
}

static Gesture_Event_Data*
twoDragGestureToData(TwoDragGesture* gesture)
{
	if (!gesture) {
		return 0;
	}

    unsigned int size = sizeof(Gesture_Event_Data) + sizeof(wl_2drag_gesture_data);
    Gesture_Event_Data* event =reinterpret_cast<Gesture_Event_Data*>(malloc(size));
	if (!event) {
		return 0;
	}

	memset(event, 0, size);
	event->gclass = WL_GESTURE_CLASS_COMMON;
	event->gtype = WL_COMMON_GESTURE_TYPE_2DRAG;
	event->len = sizeof(wl_2drag_gesture_data);
	event->state = gesture->getState();

	wl_2drag_gesture_data* data = reinterpret_cast<wl_2drag_gesture_data*>(event->data);
	
	data->pos[0].x = gesture->getPoint(0).x;
	data->pos[0].y = gesture->getPoint(0).y;
	data->pos[1].x = gesture->getPoint(1).x;
	data->pos[1].y = gesture->getPoint(1).y;
	
	data->center.x = gesture->getCenter().x;
	data->center.y = gesture->getCenter().y;
	data->lastCenter.x = gesture->getLastCenter().x;
	data->lastCenter.y = gesture->getLastCenter().y;
	data->startCenter.x = gesture->getStartCenter().x;
	data->startCenter.y = gesture->getStartCenter().y;

	data->offset.x = gesture->getOffset().x;
	data->offset.y = gesture->getOffset().y;
	data->lastOffset.x = gesture->getLastOffset().x;
	data->lastOffset.y = gesture->getLastOffset().y;
	data->startOffset.x = gesture->getStartOffset().x;
	data->startOffset.y = gesture->getStartOffset().y;

	return event;
}

static Gesture_Event_Data*
doubleClickGestureToData(DoubleClickGesture* gesture)
{
	if (!gesture) {
		return 0;
	}

    unsigned int size = sizeof(Gesture_Event_Data) + sizeof(wl_doubleClick_gesture_data);
    Gesture_Event_Data* event = reinterpret_cast<Gesture_Event_Data*>(malloc(size));
	if (!event) {
		return 0;
	}

	memset(event, 0, size);
	event->gclass = WL_GESTURE_CLASS_COMMON;
	event->gtype = WL_COMMON_GESTURE_TYPE_DOUBLECLICK;
	event->len = sizeof(wl_doubleClick_gesture_data);

	GesturePoint point1 = gesture->getPoint1();
	GesturePoint point2 = gesture->getPoint2();

	wl_doubleClick_gesture_data* data = reinterpret_cast<wl_doubleClick_gesture_data*>(event->data);
	data->pos.x = (point1.x + point2.x)/2;
	data->pos.y = (point1.y + point2.y)/2;
	event->state = gesture->getState();

	return event;
}

static Gesture_Event_Data*
longPressGestureToData(LongPressGesture* gesture)
{
	if (!gesture) {
		return 0;
	}

    unsigned int size = sizeof(Gesture_Event_Data) + sizeof(wl_longPress_gesture_data);
    Gesture_Event_Data* event = reinterpret_cast<Gesture_Event_Data*>(malloc(size));
	if (!event) {
		return 0;
	}

	memset(event, 0, size);
	event->gclass = WL_GESTURE_CLASS_COMMON;
	event->gtype = WL_COMMON_GESTURE_TYPE_LONGPRESS;
	event->len = sizeof(wl_longPress_gesture_data);
	event->state = gesture->getState();

	wl_longPress_gesture_data* data = reinterpret_cast<wl_longPress_gesture_data*>(event->data);
	data->pos.x = gesture->getPoint().x;
	data->pos.y = gesture->getPoint().y;

	struct timeval timeCur;
	gettimeofday(&timeCur, NULL);
	struct timeval pressTime = gesture->getPressTime();

	data->pressTime = (timeCur.tv_sec * 1000 + timeCur.tv_usec / 1000) - (pressTime.tv_sec * 1000 + pressTime.tv_usec / 1000);
	data->actionInterval = gesture->getActionInterval();

	return event;
}

static Gesture_Event_Data*
twoLongPressGestureToData(TwoLongPressGesture* gesture)
{
    if (!gesture) {
        return 0;
    }

    unsigned int size = sizeof(Gesture_Event_Data) + sizeof(wl_2longpress_gesture_data);
    Gesture_Event_Data* event = reinterpret_cast<Gesture_Event_Data*>(malloc(size));
    if (!event) {
        return 0;
    }

    memset(event, 0, size);
    event->gclass = WL_GESTURE_CLASS_COMMON;
    event->gtype = WL_COMMON_GESTURE_TYPE_2LONGPRESS;
    event->len = sizeof(wl_2longpress_gesture_data);
    event->state = gesture->getState();

    wl_2longpress_gesture_data* data = reinterpret_cast<wl_2longpress_gesture_data*>(event->data);
    data->pos[0].x = gesture->getPoint(0).x;
    data->pos[0].y = gesture->getPoint(0).y;
    data->pos[1].x = gesture->getPoint(1).x;
    data->pos[1].y = gesture->getPoint(1).y;

    struct timeval timeCur;
    gettimeofday(&timeCur, NULL);
    struct timeval pressTime = gesture->getPressTime();

    data->pressTime = (timeCur.tv_sec * 1000 + timeCur.tv_usec / 1000) - (pressTime.tv_sec * 1000 + pressTime.tv_usec / 1000);
    data->actionInterval = gesture->getActionInterval();

    return event;
}

static Gesture_Event_Data*
twoTapGestureToData(TwoTapGesture* gesture)
{
    if (!gesture) {
        return 0;
    }

    unsigned int size = sizeof(Gesture_Event_Data) + sizeof(wl_2tap_gesture_data);
    Gesture_Event_Data* event =reinterpret_cast<Gesture_Event_Data*>(malloc(size));
    if (!event) {
        return 0;
    }

    memset(event, 0, size);
    event->gclass = WL_GESTURE_CLASS_COMMON;
    event->gtype = WL_COMMON_GESTURE_TYPE_2TAP;
    event->len = sizeof(wl_2tap_gesture_data);
    event->state = gesture->getState();

    wl_2tap_gesture_data* data = reinterpret_cast<wl_2tap_gesture_data*>(event->data);

    data->center.x = gesture->getCenter().x;
    data->center.y = gesture->getCenter().y;
    data->offset.x = gesture->getOffset().x;
    data->offset.y = gesture->getOffset().y;

    data->startPos.x = gesture->getStartCenter().x;
    data->startPos.y = gesture->getStartCenter().y;

    return event;
}


static Gesture_Event_Data*
multiLongPressGestureToData(MultiLongPressGesture* gesture)
{
    if (!gesture) {
        return 0;
    }

    unsigned int size = sizeof(Gesture_Event_Data) + sizeof(wl_multilongpress_gesture_data);
    Gesture_Event_Data* event = reinterpret_cast<Gesture_Event_Data*>(malloc(size));
    if (!event) {
        return 0;
    }

    memset(event, 0, size);
    event->gclass = WL_GESTURE_CLASS_SYSTEM;
    event->gtype = WL_SYSTEM_GESTURE_TYPE_MULTILONGPRESS;
    event->len = sizeof(wl_multilongpress_gesture_data);
    event->state = gesture->getState();

    wl_multilongpress_gesture_data* data = reinterpret_cast<wl_multilongpress_gesture_data*>(event->data);


    data->pointNum = gesture->getPointNum();

    for(int i = 0;i < gesture->getPointNum(); ++i) {
        data->pos[i].x = gesture->getPoint(i).x;
        data->pos[i].y = gesture->getPoint(i).y;
    }

    struct timeval timeCur;
    gettimeofday(&timeCur, NULL);
    struct timeval pressTime = gesture->getPressTime();

    data->pressTime = (timeCur.tv_sec * 1000 + timeCur.tv_usec / 1000) - (pressTime.tv_sec * 1000 + pressTime.tv_usec / 1000);
    data->actionInterval = gesture->getActionInterval();

    return event;
}

static Gesture_Event_Data*
scratchGestureToData(ScratchGesture* gesture)
{
	if (!gesture) {
		return 0;
	}

    unsigned int size = sizeof(Gesture_Event_Data) + sizeof(wl_scratch_gesture_data);
    Gesture_Event_Data* event = reinterpret_cast<Gesture_Event_Data*>(malloc(size));
	if (!event) {
		return 0;
	}

	memset(event, 0, size);
	event->gclass = WL_GESTURE_CLASS_SYSTEM;
	event->gtype = WL_SYSTEM_GESTURE_TYPE_SCRATCH;
	event->len = sizeof(wl_scratch_gesture_data);

	wl_scratch_gesture_data* data = reinterpret_cast<wl_scratch_gesture_data*>(event->data);
	event->state = gesture->getState();

    data->center.x = gesture->getCenter().x;
    data->center.y = gesture->getCenter().y;

    data->lastCenter.x = gesture->getLastCenter().x;
    data->lastCenter.y = gesture->getLastCenter().y;

    data->startCenter.x = gesture->getStartCenter().x;
    data->startCenter.y = gesture->getStartCenter().y;

    data->offset.x = gesture->getOffset().x;
    data->offset.y = gesture->getOffset().y;

    data->lastOffset.x = gesture->getLastOffset().x;
    data->lastOffset.y = gesture->getLastOffset().y;

    data->startOffset.x = gesture->getStartOffset().x;
    data->startOffset.y = gesture->getStartOffset().y;

	GesturePoint points[5];
	gesture->getPoints(points, 5, &(data->pointNum));
	for (int i = 0; i < data->pointNum; i++) {
		data->points[i].x = points[i].x;
		data->points[i].y = points[i].y;
	}

    for (int i = 0; i < 5; i++) {
        GesturePoint& points = gesture->getStartPoint(i);
        data->startpoints[i].x = points.x;
        data->startpoints[i].y = points.y;
    }

	return event;
}

static Gesture_Event_Data*
threeFlickGestureToData(ThreeFlickGesture* gesture)
{
	if (!gesture) {
		return 0;
	}

    unsigned int size = sizeof(Gesture_Event_Data) + sizeof(wl_3flick_gesture_data);
    Gesture_Event_Data* event =reinterpret_cast<Gesture_Event_Data*>(malloc(size));
	if (!event) {
		return 0;
	}

	memset(event, 0, size);
	event->gclass = WL_GESTURE_CLASS_SYSTEM;
	event->gtype = WL_SYSTEM_GESTURE_TYPE_FLICK;
    event->len = sizeof(wl_3flick_gesture_data);

	wl_3flick_gesture_data* data = reinterpret_cast<wl_3flick_gesture_data*>(event->data);
	event->state = gesture->getState();

	for (int i = 0; i < gesture->getPointNum(); i++) {
		data->points[i].x = gesture->getPoint(i).x;
		data->points[i].y = gesture->getPoint(i).y;
	}

	data->pointNum = gesture->getPointNum();

	data->center.x = gesture->getCenter().x;
	data->center.y = gesture->getCenter().y;
	data->lastCenter.x = gesture->getLastCenter().x;
	data->lastCenter.y = gesture->getLastCenter().y;
	data->startCenter.x = gesture->getStartCenter().x;
	data->startCenter.y = gesture->getStartCenter().y;

	data->offset.x = gesture->getOffset().x;
	data->offset.y = gesture->getOffset().y;
	data->lastOffset.x = gesture->getLastOffset().x;
	data->lastOffset.y = gesture->getLastOffset().y;
	data->startOffset.x = gesture->getStartOffset().x;
	data->startOffset.y = gesture->getStartOffset().y;
	
	return event;
}
#endif

// the return buffer must be free outside
Gesture_Event_Data* GestureCommonFun::gestureToData(GestureObject* object)
{
    if (!object) {
        return 0;
    }

    switch(object->getType()) {
        case GESTURE_TYPE_TAP:
            return tapGestureToData(static_cast<TapGesture*>(object));
        case GESTURE_TYPE_DRAG:
            return dragGestureToData(static_cast<DragGesture*>(object));
        case GESTURE_TYPE_FLICK:
            return flickGestureToData(static_cast<FlickGesture*>(object));
        case GESTURE_TYPE_DOUBLECLICK:
            return doubleClickGestureToData(static_cast<DoubleClickGesture*>(object));
        case GESTURE_TYPE_PINCH:
            return pinchGestureToData(static_cast<PinchGesture*>(object));
        case GESTURE_TYPE_2FLICK:
            return twoFlickGestureToData(static_cast<TwoFlickGesture*>(object));
        case GESTURE_TYPE_2ROTARY:
            return rotateGestureToData(static_cast<RotateGesture*>(object));
        case GESTURE_TYPE_2DRAG:
            return twoDragGestureToData(static_cast<TwoDragGesture*>(object));
        case GESTURE_TYPE_3FLICK:
            return threeFlickGestureToData(static_cast<ThreeFlickGesture*>(object));
        case GESTURE_TYPE_SCRATCH:
            return scratchGestureToData(static_cast<ScratchGesture*>(object));
        case GESTURE_TYPE_LONGPRESS:
            return longPressGestureToData(static_cast<LongPressGesture*>(object));
        case GESTURE_TYPE_2LONGPRESS:
            return twoLongPressGestureToData(static_cast<TwoLongPressGesture*>(object));
        case GESTURE_TYPE_2TAP:
            return twoTapGestureToData(static_cast<TwoTapGesture*>(object));
        case GESTURE_TYPE_MULTILONGPRESS:
            return multiLongPressGestureToData(static_cast<MultiLongPressGesture*>(object));
        default:
            return 0;
    };

    return 0;
}

void GestureCommonFun::printGestureDataList(const Gesture_Event_Data_List* eventlist)
{
    const Gesture_Event_Data_List* curList = eventlist;
    while (curList) {
        printGestureData(curList->data);
        curList = curList->next;
    }
}

void GestureCommonFun::printGestureData(const Gesture_Event_Data* event)
{
    if (!event || event->state != GESTURE_STATE_FINISHED) {
        // printf("\n !!! invalid gesture event data !!!\n");
        return;
    }

    if (event->gclass == WL_GESTURE_CLASS_SYSTEM) {

        switch (event->gtype) {
        case WL_SYSTEM_GESTURE_TYPE_FLICK:
            printf("\n GestureEvent: WL_SYSTEM_GESTURE_TYPE_FLICK \n");
            break;
        case WL_SYSTEM_GESTURE_TYPE_SCRATCH:
            printf("\n GestureEvent: WL_SYSTEM_GESTURE_TYPE_SCRATCH \n");
            break;
        case WL_SYSTEM_GESTURE_TYPE_MULTILONGPRESS:
            printf("\n GestureEvent: WL_SYSTEM_GESTURE_TYPE_MULTILONGPRESS \n");
            break;
        default:
            break;
        }
    }
    else if (event->gclass == WL_GESTURE_CLASS_COMMON) {
        switch (event->gtype) {
        case WL_COMMON_GESTURE_TYPE_TAP:
            printf("\n GestureEvent: WL_COMMON_GESTURE_TYPE_TAP \n");
            break;
        case WL_COMMON_GESTURE_TYPE_DRAG:
            printf("\n GestureEvent: WL_COMMON_GESTURE_TYPE_DRAG \n");
            break;
        case WL_COMMON_GESTURE_TYPE_FLICK:
            printf("\n GestureEvent: WL_COMMON_GESTURE_TYPE_FLICK \n");
            break;
        case WL_COMMON_GESTURE_TYPE_PINCH:
            printf("\n GestureEvent: WL_COMMON_GESTURE_TYPE_PINCH \n");
            break;
        case WL_COMMON_GESTURE_TYPE_DOUBLECLICK:
            printf("\n GestureEvent: WL_COMMON_GESTURE_TYPE_DOUBLECLICK \n");
            break;
        case WL_COMMON_GESTURE_TYPE_2FLICK:
            printf("\n GestureEvent: WL_COMMON_GESTURE_TYPE_2FLICK \n");
            break;
        case WL_COMMON_GESTURE_TYPE_2ROTARY:
            printf("\n GestureEvent: WL_COMMON_GESTURE_TYPE_2ROTARY \n");
            break;
        case WL_COMMON_GESTURE_TYPE_2DRAG:
            printf("\n GestureEvent: WL_COMMON_GESTURE_TYPE_2DRAG \n");
            break;
        case WL_COMMON_GESTURE_TYPE_LONGPRESS:
            printf("\n GestureEvent: WL_COMMON_GESTURE_TYPE_LONGPRESS \n");
            break;
        case WL_COMMON_GESTURE_TYPE_2LONGPRESS:
            printf("\n GestureEvent: WL_COMMON_GESTURE_TYPE_2LONGPRESS \n");
            break;
        case WL_COMMON_GESTURE_TYPE_2TAP:
            printf("\n GestureEvent: WL_COMMON_GESTURE_TYPE_2TAP \n");
            break;
        default:
            break;
        }
    }
    else {

    }
}


GestureObject* threeFlickGestureDataToObject(Gesture_Event_Data* event)
{
    if (!event) {
        return 0;
    }
    ThreeFlickGesture* gesture = new ThreeFlickGesture();
    gesture->setState((GESTURE_STATE)event->state);
    wl_3flick_gesture_data* data = reinterpret_cast<wl_3flick_gesture_data*>(event->data);

    gesture->setPointNum(data->pointNum);
    for (int i = 0; i < data->pointNum; i++) {
        gesture->setPoint(i, data->points[i].x, data->points[i].y);
    }
    gesture->setCenter(data->center.x, data->center.y);
    gesture->setLastCenter(data->lastCenter.x, data->lastCenter.y);
    gesture->setStartCenter(data->startCenter.x, data->startCenter.y);
    gesture->setOffset(data->offset.x, data->offset.y);
    gesture->setLastOffset(data->lastOffset.x, data->lastOffset.y);
    gesture->setStartOffset(data->startOffset.x, data->startOffset.y);
    return gesture;
}

GestureObject* scratchGestureDataToObject(Gesture_Event_Data* event)
{
    if (!event) {
        return 0;
    }

    ScratchGesture* gesture = new ScratchGesture();
    gesture->setState((GESTURE_STATE)event->state);
    wl_scratch_gesture_data* data = reinterpret_cast<wl_scratch_gesture_data*>(event->data);

    gesture->setCenter(data->center.x, data->center.y);
    gesture->setLastCenter(data->lastCenter.x, data->lastCenter.y);
    gesture->setStartCenter(data->startCenter.x, data->startCenter.y);
    gesture->setOffset(data->offset.x, data->offset.y);
    gesture->setLastOffset(data->lastOffset.x, data->lastOffset.y);
    gesture->setStartOffset(data->startOffset.x, data->startOffset.y);

    for (int i = 0; i < 5; i++) {
        gesture->setStartPoint(i, data->startpoints[i].x, data->startpoints[i].y);
    }


    for (int i = 0; i < data->pointNum; i++) {
        gesture->setPoint(i, data->points[i].x, data->points[i].y);
    }

    return gesture;
}

GestureObject* multiLongPressGestureDataToObject(Gesture_Event_Data* event)
{
    if (!event) {
        return 0;
    }

    MultiLongPressGesture* gesture = new MultiLongPressGesture();
    gesture->setState((GESTURE_STATE)event->state);
    wl_multilongpress_gesture_data* data = reinterpret_cast<wl_multilongpress_gesture_data*>(event->data);

    gesture->setPointNum(data->pointNum);

    for(int i = 0;i < data->pointNum; ++i) {
        gesture->setPoint(i, data->pos[i].x, data->pos[i].y);
    }
    // todo cola
//    struct timeval timeCur;
//    gettimeofday(&timeCur, NULL);
//    struct timeval pressTime = gesture->setPressTime();
//    data->pressTime = (timeCur.tv_sec * 1000 + timeCur.tv_usec / 1000) - (pressTime.tv_sec * 1000 + pressTime.tv_usec / 1000);
    gesture->setActionInterval(data->actionInterval);
    return gesture;
}

GestureObject* tapGestureDataToObject(Gesture_Event_Data* event)
{
    if (!event) {
        return 0;
    }

    TapGesture* gesture = new TapGesture();
    gesture->setState((GESTURE_STATE)event->state);
    wl_tap_gesture_data* data = reinterpret_cast<wl_tap_gesture_data*>(event->data);
    gesture->setPoint(data->pos.x, data->pos.y);
    return gesture;
}

GestureObject* dragGestureDataToObject(Gesture_Event_Data* event)
{
    if (!event) {
        return 0;
    }

    DragGesture* gesture = new DragGesture();
    gesture->setState((GESTURE_STATE)event->state);
    wl_drag_gesture_data* data = reinterpret_cast<wl_drag_gesture_data*>(event->data);

    gesture->setStartPos(data->startPos.x, data->startPos.y);
    gesture->setLastPos(data->lastPos.x, data->lastPos.y);
    gesture->setPoint(data->pos.x, data->pos.y);

    return gesture;
}

GestureObject* flickGestureDataToObject(Gesture_Event_Data* event)
{
    if (!event) {
        return 0;
    }

    FlickGesture* gesture = new FlickGesture();
    gesture->setState((GESTURE_STATE)event->state);
    wl_flick_gesture_data* data = reinterpret_cast<wl_flick_gesture_data*>(event->data);

    gesture->setSpeed(data->speed);
    gesture->setAngle(data->angle);
    gesture->setPoint(data->pos.x, data->pos.y);
    return gesture;
}

GestureObject* doubleClickGestureDataToObject(Gesture_Event_Data* event)
{
    if (!event) {
        return 0;
    }

    DoubleClickGesture* gesture = new DoubleClickGesture();
    gesture->setState((GESTURE_STATE)event->state);
    wl_doubleClick_gesture_data* data = reinterpret_cast<wl_doubleClick_gesture_data*>(event->data);

    gesture->setPoint1(data->pos.x, data->pos.y);
    gesture->setPoint2(data->pos.x, data->pos.y);

    return gesture;
}

GestureObject* pinchGestureDataToObject(Gesture_Event_Data* event)
{
    if (!event) {
        return 0;
    }
    PinchGesture* gesture = new PinchGesture();
    gesture->setState((GESTURE_STATE)event->state);

    wl_pinch_gesture_data* data = reinterpret_cast<wl_pinch_gesture_data*>(event->data);
    gesture->setCenterPoint({data->center.x, data->center.y});
    gesture->setLastCenterPoint({data->lastCenter.x, data->lastCenter.y});
    gesture->setStartCenterPoint({data->startCenter.x, data->startCenter.y});
    gesture->setRotationAngle(data->rotationAngle);
    gesture->setLastRotationAngle(data->lastRotationAngle);
    gesture->setTotalRotationAngle(data->totalRotationAngle);
    gesture->setScaleFactor(data->scaleFactor);
    gesture->setLastScaleFactor(data->lastScaleFactor);
    gesture->setTotalScaleFactor(data->totalScaleFactor);
    gesture->setPoints(data->pos[0].x , data->pos[0].y,
                       data->pos[1].x , data->pos[1].y);

    return gesture;
}

GestureObject* twoFlickGestureDataToObject(Gesture_Event_Data* event)
{
    if (!event) {
        return 0;
    }

    TwoFlickGesture* gesture = new TwoFlickGesture();
    gesture->setState((GESTURE_STATE)event->state);
    wl_2flick_gesture_data* data = reinterpret_cast<wl_2flick_gesture_data*>(event->data);

    gesture->setPoint(0, data->pos[0].x, data->pos[0].y);
    gesture->setPoint(1, data->pos[1].x, data->pos[1].y);
    gesture->setCenter(data->center.x, data->center.y);
    gesture->setSpeed(data->speed);
    gesture->setAngle(data->angle);

    return gesture;
}

GestureObject* rotateGestureDataToObject(Gesture_Event_Data* event)
{
    if (!event) {
        return 0;
    }

    RotateGesture* gesture = new RotateGesture();
    gesture->setState((GESTURE_STATE)event->state);
    wl_2rotary_gesture_data* data = reinterpret_cast<wl_2rotary_gesture_data*>(event->data);


    gesture->setCenter({ data->center.x, data->center.y});
    gesture->setLastCenter( { data->lastCenter.x, data->lastCenter.y});
    gesture->setStartCenter( {data->startCenter.x, data->startCenter.y});
    gesture->setRotationAngle(data->rotationAngle);
    gesture->setLastRotationAngle(data->lastRotationAngle);
    gesture->setTotalRotationAngle(data->totalRotationAngle);
    gesture->setPoints(data->pos[0].x, data->pos[0].y,
                       data->pos[1].x, data->pos[1].y);


    return gesture;
}

GestureObject* twoDragGestureDataToObject(Gesture_Event_Data* event)
{
    if (!event) {
        return 0;
    }
    TwoDragGesture* gesture = new TwoDragGesture();
    gesture->setState((GESTURE_STATE)event->state);

    wl_2drag_gesture_data* data = reinterpret_cast<wl_2drag_gesture_data*>(event->data);

    gesture->setPoint(0, data->pos[0].x, data->pos[0].y);
    gesture->setPoint(1, data->pos[1].x, data->pos[1].y);

    gesture->setCenter(data->center.x, data->center.y);
    gesture->setLastCenter(data->lastCenter.x, data->lastCenter.y);
    gesture->setStartCenter(data->startCenter.x, data->startCenter.y);

    gesture->setOffset(data->offset.x, data->offset.y);
    gesture->setLastOffset(data->lastOffset.x, data->lastOffset.y);
    gesture->setStartOffset(data->startOffset.x, data->startOffset.y);
    return gesture;
}

GestureObject* longPressGestureDataToObject(Gesture_Event_Data* event)
{
    if (!event) {
        return 0;
    }
    LongPressGesture* gesture = new LongPressGesture();
    gesture->setState((GESTURE_STATE)event->state);

    wl_longPress_gesture_data* data = reinterpret_cast<wl_longPress_gesture_data*>(event->data);

    gesture->setPoint(data->pos.x, data->pos.y);
    // todo
//    struct timeval timeCur;
//    gettimeofday(&timeCur, NULL);
//    struct timeval pressTime = gesture->getPressTime();
//    data->pressTime = (timeCur.tv_sec * 1000 + timeCur.tv_usec / 1000) - (pressTime.tv_sec * 1000 + pressTime.tv_usec / 1000);
    gesture->setActionInterval(data->actionInterval);

    return gesture;
}

GestureObject* twoLongPressGestureDataToObject(Gesture_Event_Data* event)
{
    if (!event) {
        return 0;
    }
    TwoLongPressGesture* gesture = new TwoLongPressGesture();
    gesture->setState((GESTURE_STATE)event->state);

    wl_2longpress_gesture_data* data = reinterpret_cast<wl_2longpress_gesture_data*>(event->data);

    gesture->setPoint(0, data->pos[0].x, data->pos[0].y);
    gesture->setPoint(1, data->pos[1].x, data->pos[1].y);

    // todo
//    struct timeval timeCur;
//    gettimeofday(&timeCur, NULL);
//    struct timeval pressTime = gesture->getPressTime();
//    data->pressTime = (timeCur.tv_sec * 1000 + timeCur.tv_usec / 1000) - (pressTime.tv_sec * 1000 + pressTime.tv_usec / 1000);
    gesture->setActionInterval(data->actionInterval);

    return gesture;
}


GestureObject* twoTapGestureDataToObject(Gesture_Event_Data* event)
{
    if (!event) {
        return 0;
    }
    TwoDragGesture* gesture = new TwoDragGesture();
    gesture->setState((GESTURE_STATE)event->state);

    wl_2tap_gesture_data* data = reinterpret_cast<wl_2tap_gesture_data*>(event->data);

    gesture->setCenter(data->center.x, data->center.y);
    gesture->setOffset(data->offset.x, data->offset.y);
    gesture->setStartCenter(data->startPos.x, data->startPos.y);

    return gesture;
}


GestureObject* GestureCommonFun::gestureToObject(Gesture_Event_Data* data)
{
    if (!data) {
        return 0;
    }

#define GESTURE_INFO(WL_TYPE, CLS_TYPE) \
    case WL_TYPE: { \
        GestureObject* object = new GestureObject(CLS_TYPE); \
        object->setState((GESTURE_STATE)data->state); \
        return object; \
    }

    if (data->gclass == WL_GESTURE_CLASS_SYSTEM) {
        switch(data->gtype) {
            case WL_SYSTEM_GESTURE_TYPE_FLICK:
                return threeFlickGestureDataToObject(data);
            case WL_SYSTEM_GESTURE_TYPE_SCRATCH:
                return scratchGestureDataToObject(data);
            case WL_SYSTEM_GESTURE_TYPE_MULTILONGPRESS:
                return multiLongPressGestureDataToObject(data);

//            GESTURE_INFO(WL_SYSTEM_GESTURE_TYPE_FLICK, GESTURE_TYPE_3FLICK)
//            GESTURE_INFO(WL_SYSTEM_GESTURE_TYPE_SCRATCH, GESTURE_TYPE_SCRATCH)
//            GESTURE_INFO(WL_SYSTEM_GESTURE_TYPE_MULTILONGPRESS, GESTURE_TYPE_MULTILONGPRESS)
            default:
                return 0;
        }
    }
    else {
        switch(data->gtype) {
            case WL_COMMON_GESTURE_TYPE_TAP:
                return tapGestureDataToObject(data);
            case WL_COMMON_GESTURE_TYPE_DRAG:
                return dragGestureDataToObject(data);
            case WL_COMMON_GESTURE_TYPE_FLICK:
                return flickGestureDataToObject(data);
            case WL_COMMON_GESTURE_TYPE_PINCH:
                return pinchGestureDataToObject(data);
            case WL_COMMON_GESTURE_TYPE_DOUBLECLICK:
                return doubleClickGestureDataToObject(data);
            case WL_COMMON_GESTURE_TYPE_2FLICK:
                return twoFlickGestureDataToObject(data);
            case WL_COMMON_GESTURE_TYPE_2ROTARY:
                return rotateGestureDataToObject(data);
           case WL_COMMON_GESTURE_TYPE_2DRAG:
               return twoDragGestureDataToObject(data);
            case WL_COMMON_GESTURE_TYPE_LONGPRESS:
                return longPressGestureDataToObject(data);
            case WL_COMMON_GESTURE_TYPE_2LONGPRESS:
                return twoLongPressGestureDataToObject(data);
            case WL_COMMON_GESTURE_TYPE_2TAP:
                return twoTapGestureDataToObject(data);
//            GESTURE_INFO(WL_COMMON_GESTURE_TYPE_TAP, GESTURE_TYPE_TAP)
//            GESTURE_INFO(WL_COMMON_GESTURE_TYPE_DRAG, GESTURE_TYPE_DRAG)
//            GESTURE_INFO(WL_COMMON_GESTURE_TYPE_FLICK, GESTURE_TYPE_FLICK)
//            GESTURE_INFO(WL_COMMON_GESTURE_TYPE_PINCH, GESTURE_TYPE_PINCH)
//            GESTURE_INFO(WL_COMMON_GESTURE_TYPE_DOUBLECLICK, GESTURE_TYPE_DOUBLECLICK)
//            GESTURE_INFO(WL_COMMON_GESTURE_TYPE_2FLICK, GESTURE_TYPE_2FLICK)
//            GESTURE_INFO(WL_COMMON_GESTURE_TYPE_2ROTARY, GESTURE_TYPE_2ROTARY)
//            GESTURE_INFO(WL_COMMON_GESTURE_TYPE_2DRAG, GESTURE_TYPE_2DRAG)
//            GESTURE_INFO(WL_COMMON_GESTURE_TYPE_LONGPRESS, GESTURE_TYPE_LONGPRESS)
//            GESTURE_INFO(WL_COMMON_GESTURE_TYPE_2LONGPRESS, GESTURE_TYPE_2LONGPRESS)
//            GESTURE_INFO(WL_COMMON_GESTURE_TYPE_2TAP, GESTURE_TYPE_2TAP)
            default:
                return 0;
        };



    }

    return 0;
}





