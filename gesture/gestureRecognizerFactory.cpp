#include "gestureRecognizerFactory.h"
#include "tapGestureRecognizer.h"
#include "scratchGestureRecognizer.h"
#include "dragGestureRecognizer.h"
#include "doubleClickGestureRecognizer.h"
#include "flickGestureRecognizer.h"
#include "pinchGestureRecognizer.h"
#include "rotateGestureRecognizer.h"
#include "twoDragGestureRecognizer.h"
#include "twoFlickGestureRecognizer.h"
#include "threeFlickGestureRecognizer.h"
#include "longPressGestureRecognizer.h"
#include "twoLongPressGestureRecognizer.h"
#include "twoTapGestureRecognizer.h"
#include "multiLongPressGestureRecognizer.h"

GestureRecognizer* GestureRecognizerFactory::create(GESTURE_TYPE gtype)
{
	GestureRecognizer* gesture = 0;
	
	switch (gtype) {
		case GESTURE_TYPE_TAP:
            gesture = new TapGestureRecognizer();
            break;
    	case GESTURE_TYPE_DRAG:
            gesture = new DragGestureRecognizer();
			break;			
    	case GESTURE_TYPE_FLICK:
            gesture = new FlickGestureRecognizer();
			break;			
    	case GESTURE_TYPE_DOUBLECLICK:
			gesture = new DoubleClickGestureRecognizer();
			break;			
    	case GESTURE_TYPE_PINCH:
            gesture = new PinchGestureRecognizer();
			break;
        case GESTURE_TYPE_2ROTARY:
            gesture = new RotateGestureRecognizer();
            break;
		case GESTURE_TYPE_2FLICK:
			gesture = new TwoFlickGestureRecognizer();
			break;
		case GESTURE_TYPE_2DRAG:
			gesture = new TwoDragGestureRecognizer();
			break;
		case GESTURE_TYPE_3FLICK:
			gesture = new ThreeFlickGestureRecognizer();
			break;			
    	case GESTURE_TYPE_SCRATCH:
			gesture = new ScratchGestureRecognizer();
			break;			
		case GESTURE_TYPE_LONGPRESS:
			gesture = new LongPressGestureRecognizer();
			break;
        case GESTURE_TYPE_2LONGPRESS:
            gesture = new TwoLongPressGestureRecognizer();
            break;
        case GESTURE_TYPE_2TAP:
		    gesture = new TwoTapGestureRecognizer();
			break;
        case GESTURE_TYPE_MULTILONGPRESS:
            gesture = new MultiLongPressGestureRecognizer();
            break;			
		default:
			break;
	};
	
	return gesture;
}
