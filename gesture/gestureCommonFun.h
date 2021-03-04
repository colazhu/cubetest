#ifndef GESTURECOMMONFUN_H
#define GESTURECOMMONFUN_H

// #include <pixman.h>
#include "gesturePublicDef.h"
#include "gestureDef.h"
#include "stdint.h"

class GestureRegion;
class GestureObject;
struct MultiTouchPoint;
struct GestureCommonFun
{
    static unsigned long currentTime();
    static GESTURE_TYPE gestureTypeConvert(WL_GESTURE_CLASS gclass, int separateType);
    static bool insideRegion(uint32_t count, struct MultiTouchPoint* points, GestureRegion* region);
    static bool insideRegion(uint32_t count, struct GesturePoint* points, GestureRegion* region);


    // the return buffer must be free outside
    static Gesture_Event_Data* gestureToData(GestureObject* object);
    static GestureObject* gestureToObject(Gesture_Event_Data* data);

    static void printGestureData(const Gesture_Event_Data* data);
    static void printGestureDataList(const Gesture_Event_Data_List* data);
};

#endif /* GESTURECOMMONFUN_H */
