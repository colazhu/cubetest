#ifndef GESTURERECOGNIZERFACTORY_H
#define GESTURERECOGNIZERFACTORY_H

#include "gestureDef.h"

class GestureRecognizer;
struct GestureRecognizerFactory
{
	static GestureRecognizer* create(GESTURE_TYPE gtype);
};

#endif /* GESTURERECOGNIZERFACTORY_H */
