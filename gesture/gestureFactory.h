#ifndef GESTUREFACTORY_H
#define GESTUREFACTORY_H
#include "gestureDef.h"

class GestureObject;


struct GestureFactory
{
	static GestureObject* create(GESTURE_TYPE gtype);
};

#endif /* GESTUREFACTORY_H */
