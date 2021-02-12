#include "gestureFactory.h"
#include "tapGesture.h"
#include "scratchGesture.h"
#include "dragGesture.h"
#include "pinchGesture.h"
#include "flickGesture.h"
#include "doubleClickGesture.h"
#include "rotateGesture.h"
#include "longPressGesture.h"
#include "twoLongPressGesture.h"
#include "twoTapGesture.h"
#include "multiLongPressGesture.h"
#include "threeFlickGesture.h"
#include "twoFlickGesture.h"
#include "twoDragGesture.h"

GestureObject* GestureFactory::create(GESTURE_TYPE gtype)
{
	GestureObject* gesture = 0;	
	switch (gtype) {
		case GESTURE_TYPE_TAP:
			gesture = new TapGesture();
			break;			
    	case GESTURE_TYPE_DRAG:
            gesture = new DragGesture();
			break;			
    	case GESTURE_TYPE_FLICK:
            gesture = new FlickGesture();
			break;			
    	case GESTURE_TYPE_DOUBLECLICK:
            gesture = new DoubleClickGesture();
            break;			
    	case GESTURE_TYPE_PINCH:
            gesture = new PinchGesture();
            break;
        case GESTURE_TYPE_2ROTARY:
            gesture = new RotateGesture();
            break;
		case GESTURE_TYPE_2FLICK:
			gesture = new TwoFlickGesture();
			break;
		case GESTURE_TYPE_2DRAG:
			gesture = new TwoDragGesture();
			break;
		case GESTURE_TYPE_3FLICK:
			gesture = new ThreeFlickGesture();
			break;			
    	case GESTURE_TYPE_SCRATCH:
			gesture = new ScratchGesture();
			break;
		case GESTURE_TYPE_LONGPRESS:
			gesture = new LongPressGesture();
            break;
        case GESTURE_TYPE_2LONGPRESS:
            gesture = new TwoLongPressGesture();
            break;
        case GESTURE_TYPE_2TAP:
		    gesture = new TwoTapGesture();
			break;
        case GESTURE_TYPE_MULTILONGPRESS:
            gesture = new MultiLongPressGesture();
            break;
        default:
            break;
	};
	
	return gesture;
}

