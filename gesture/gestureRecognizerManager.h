#ifndef GESTURERECOGNIZERMANAGER_H
#define GESTURERECOGNIZERMANAGER_H
#include "gesturePublicDef.h"
#include "gestureDef.h"
#include "gestureRecognizer.h"
#include <vector>
#include <map>

#define TOUCH_GESTURE_EVENT_PERIOD 20000
struct RecognizersSort {
	bool operator()(int s1, int s2) const
	{
		return (s1 > s2);
	}
};

typedef std::map<int, GestureRecognizer*, RecognizersSort> RecognizersMap;
typedef RecognizersMap::iterator RecognizersIterator;

class GestureObject;
class GestureRecognizerManager
{
private:
	struct GestureResult {
		bool isValid;
		bool isUsing;
		GestureRecognizer::ResultFlag tmpResultFlag;
		GestureRecognizer::ResultFlag resultFlag;
		GestureObject* old;
		GestureObject* cur;
	};
	
public:
	GestureRecognizerManager();
	~GestureRecognizerManager();

	void clearAllRecognizer();
	void registerGestureRecognizer(GESTURE_TYPE gtype);
    void unregisterGestureRecognizer(GESTURE_TYPE gtype);

	void processTouchEvent(int motionEventAction, 
			void *data, 
			struct MultiTouchPoint *mtPoints, 
			int pointCount,
            FUNC_NOTIFY_GESTURE_EVENT notify,
            GestureRegion* validRegion,
			long long when);

	void processTimeOutEvent(GestureRecognizer* recognizer, void* data);

private:
	RecognizersMap mRecognizersV;
	GestureResult mResultList[GESTURE_TYPE_NUM];

    inline void createAndAddListItem(struct Gesture_Event_Data_List** head,
        struct Gesture_Event_Data_List** tail,
        struct Gesture_Event_Data* data);
    inline void freeEventDataList(struct Gesture_Event_Data_List* head);
};


#endif /* GESTURERECOGNIZERMANAGER_H */
