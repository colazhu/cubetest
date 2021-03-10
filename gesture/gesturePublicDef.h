#ifndef GESTUREPUBLIC_DEF_H
#define GESTUREPUBLIC_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

// static const int wl_gesture_invalid_coordinate = -1;
#define WL_GESTURE_MAX_POINTS  5

enum WL_GESTURE_STATE {
    WL_GESTURE_STATE_STARTED  = 0,
    WL_GESTURE_STATE_UPDATED  = 1,
    WL_GESTURE_STATE_FINISHED = 2,
    WL_GESTURE_STATE_CANCECLED = 3,
};

enum WL_GESTURE_CLASS {
    WL_GESTURE_CLASS_SYSTEM = 0,
    WL_GESTURE_CLASS_COMMON  = 1,
};

enum WL_COMMON_GESTURE_TYPE {
    WL_COMMON_GESTURE_TYPE_TAP = 0,
    WL_COMMON_GESTURE_TYPE_DRAG = 1,
    WL_COMMON_GESTURE_TYPE_FLICK = 2,
    WL_COMMON_GESTURE_TYPE_PINCH = 3,
    WL_COMMON_GESTURE_TYPE_DOUBLECLICK = 4,
    WL_COMMON_GESTURE_TYPE_2FLICK = 5,
    WL_COMMON_GESTURE_TYPE_2ROTARY = 6,
    WL_COMMON_GESTURE_TYPE_2DRAG = 7,
    WL_COMMON_GESTURE_TYPE_LONGPRESS = 8,
    WL_COMMON_GESTURE_TYPE_2LONGPRESS = 9,
	WL_COMMON_GESTURE_TYPE_2TAP = 10,
    WL_COMMON_GESTURE_TYPE_NUM = 11
	
};

enum WL_COMMON_GESTURE_TYPE_FLAG {
    WL_COMMON_GESTURE_TYPE_TAP_FLAG = 0x0001,
    WL_COMMON_GESTURE_TYPE_DRAG_FLAG = 0x0002,
    WL_COMMON_GESTURE_TYPE_FLICK_FLAG = 0x0004,
    WL_COMMON_GESTURE_TYPE_PINCH_FLAG = 0x0008,
    WL_COMMON_GESTURE_TYPE_DOUBLECLICK_FLAG = 0x0010,
    WL_COMMON_GESTURE_TYPE_2FLICK_FLAG = 0x0020,
    WL_COMMON_GESTURE_TYPE_2ROTARY_FLAG = 0x0040,
    WL_COMMON_GESTURE_TYPE_2DRAG_FLAG = 0x0080,
    WL_COMMON_GESTURE_TYPE_LONGPRESS_FLAG = 0x0100,
    WL_COMMON_GESTURE_TYPE_2LONGPRESS_FLAG = 0x0200,
    WL_COMMON_GESTURE_TYPE_2TAP_FLAG = 0x0400,

    WL_COMMON_GESTURE_TYPE_FLAG_ALL = 0xFFFF
};

enum WL_SYSTEM_GESTURE_TYPE {
	WL_SYSTEM_GESTURE_TYPE_FLICK = 0,
    WL_SYSTEM_GESTURE_TYPE_SCRATCH = 1,
    WL_SYSTEM_GESTURE_TYPE_MULTILONGPRESS = 2,
    WL_SYSTEM_GESTURE_TYPE_NUM = 3
};

enum WL_SYSTEM_GESTURE_TYPE_FLAG {
	WL_SYSTEM_GESTURE_TYPE_FLICK_FLAG = 0x0001,
    WL_SYSTEM_GESTURE_TYPE_SCRATCH_FLAG = 0x0002,
    WL_SYSTEM_GESTURE_TYPE_MULTILONGPRESS_FLAG = 0x0004,

    WL_SYSTEM_GESTURE_TYPE_FLAG_ALL = 0xFFFF
};

//struct wl_gestures_event {
//	unsigned int count;
//	unsigned int size;
//	char data[0];
//};

struct Gesture_Event_Data {
    unsigned char gclass;
    unsigned char gtype;
    unsigned char state;
    unsigned char reserve[1];
    unsigned int len;
    char data[0];
};

struct Gesture_Event_Data_List {
    struct Gesture_Event_Data* data;
    struct Gesture_Event_Data_List* next;
};


struct wl_gesture_point {
    int x;
    int y;
};

struct wl_gesture_point_offset {
    int x;
    int y;
};

//**********one finger gesture**********//
struct wl_tap_gesture_data {
    struct wl_gesture_point pos;
};

struct wl_drag_gesture_data {
    struct wl_gesture_point pos;
    struct wl_gesture_point lastPos;
    struct wl_gesture_point startPos;
};

struct wl_flick_gesture_data {
    struct wl_gesture_point pos;
    unsigned int speed;
    float angle;
};

struct wl_pinch_gesture_data {
    struct wl_gesture_point pos[2];
    struct wl_gesture_point center;
    struct wl_gesture_point lastCenter;
    struct wl_gesture_point startCenter;
    float rotationAngle;
    float lastRotationAngle;
    float totalRotationAngle;
    float scaleFactor;
    float lastScaleFactor;
    float totalScaleFactor;
};

struct wl_doubleClick_gesture_data {
    struct wl_gesture_point pos;
};

struct wl_longPress_gesture_data {
    struct wl_gesture_point pos;
    int pressTime;
    int actionInterval;
};

//**********two fingers gesture**********//
struct wl_2flick_gesture_data {
    struct wl_gesture_point pos[2];
    struct wl_gesture_point center;
    unsigned int speed;
    float angle;
};

struct wl_2rotary_gesture_data {
    struct wl_gesture_point pos[2];
    struct wl_gesture_point center;
    struct wl_gesture_point lastCenter;
    struct wl_gesture_point startCenter;
    float rotationAngle;
    float lastRotationAngle;
    float totalRotationAngle;
};

struct wl_2drag_gesture_data {
    struct wl_gesture_point pos[2];
    struct wl_gesture_point center;
    struct wl_gesture_point lastCenter;
    struct wl_gesture_point startCenter;
    struct wl_gesture_point_offset offset;
    struct wl_gesture_point_offset lastOffset;
    struct wl_gesture_point_offset startOffset;
};

struct wl_2longpress_gesture_data {
    struct wl_gesture_point pos[2];
    int pressTime;
    int actionInterval;
};

struct wl_2tap_gesture_data {
    struct wl_gesture_point center;
    struct wl_gesture_point_offset offset;
    struct wl_gesture_point startPos;
};

//**********more than 2 fingers gesture**********//
#define WL_3FLICK_GESTURE_MAX_POINTS  4
struct wl_3flick_gesture_data {
    int pointNum;
    struct wl_gesture_point points[WL_3FLICK_GESTURE_MAX_POINTS];
    struct wl_gesture_point center;
    struct wl_gesture_point lastCenter;
    struct wl_gesture_point startCenter;
    struct wl_gesture_point_offset offset;
    struct wl_gesture_point_offset lastOffset;
    struct wl_gesture_point_offset startOffset;
};

#define WL_SCRATCH_GESTURE_MAX_POINTS  5
struct wl_scratch_gesture_data {
    int pointNum;
    struct wl_gesture_point points[WL_SCRATCH_GESTURE_MAX_POINTS];
    struct wl_gesture_point startpoints[WL_SCRATCH_GESTURE_MAX_POINTS];
    struct wl_gesture_point center;
    struct wl_gesture_point lastCenter;
    struct wl_gesture_point startCenter;
    struct wl_gesture_point_offset offset;
    struct wl_gesture_point_offset lastOffset;
    struct wl_gesture_point_offset startOffset;
};

#define WL_MULTILONGPRESS_GESTURE_MAX_POINTS  5
struct wl_multilongpress_gesture_data {
    struct wl_gesture_point pos[WL_MULTILONGPRESS_GESTURE_MAX_POINTS];
    int pressTime;
    int actionInterval;
    int pointNum;
};

typedef void (*FUNC_NOTIFY_GESTURE_EVENT)(const struct Gesture_Event_Data_List*, void *data);


/*
 * Pointer coordinate data.
 */
struct TouchPointerCoords
{
    float x;
    float y;
//    float pressure;
//    float size;
//    float touchMajor;
//    float touchMinor;
//    float toolMajor;
//    float toolMinor;
//    float orientation;
};

enum TouchPointState {
    TouchPointInvalid = 0x00,
    TouchPointPressed = 0x01,
    TouchPointMoved = 0x02,
    // TouchPointStationary = 0x04,
    TouchPointReleased = 0x08
};

struct MultiTouchPoint {
    int id;
    int isPrimary;
    struct TouchPointerCoords coords;
    enum TouchPointState state;
};

/* Bit shift for the action bits holding the pointer index as
 * defined by MOTION_EVENT_ACTION_POINTER_INDEX_MASK.
 */
#define MOTION_EVENT_ACTION_POINTER_INDEX_SHIFT 8

enum {
    /* Bit mask of the parts of the action code that are the action itself.
     */
    MOTION_EVENT_ACTION_MASK = 0x00ff,

    /* Bits in the action code that represent a pointer index, used with
     * MOTION_EVENT_ACTION_POINTER_DOWN and MOTION_EVENT_ACTION_POINTER_UP.  Shifting
     * down by MOTION_EVENT_ACTION_POINTER_INDEX_SHIFT provides the actual pointer
     * index where the data for the pointer going up or down can be found.
     */
    MOTION_EVENT_ACTION_POINTER_INDEX_MASK  = 0xff00,

    /* A pressed gesture has started, the motion contains the initial starting location.
     */
    MOTION_EVENT_ACTION_DOWN = 0,

    /* A pressed gesture has finished, the motion contains the final release location
     * as well as any intermediate points since the last down or move event.
     */
    MOTION_EVENT_ACTION_UP = 1,

    /* A change has happened during a press gesture (between MOTION_EVENT_ACTION_DOWN and
     * MOTION_EVENT_ACTION_UP).  The motion contains the most recent point, as well as
     * any intermediate points since the last down or move event.
     */
    MOTION_EVENT_ACTION_MOVE = 2,

    /* A non-primary pointer has gone down.
     * The bits in MOTION_EVENT_ACTION_POINTER_INDEX_MASK indicate which pointer changed.
     */
    MOTION_EVENT_ACTION_POINTER_DOWN = 3,

    /* A non-primary pointer has gone up.
     * The bits in MOTION_EVENT_ACTION_POINTER_INDEX_MASK indicate which pointer changed.
     */
    MOTION_EVENT_ACTION_POINTER_UP = 4,
};

#ifdef __cplusplus
}
#endif

#endif /* GESTUREPUBLIC_DEF_H */
