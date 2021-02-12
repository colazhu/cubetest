#ifndef BASE_H
#define BASE_H

// C/C++
#include <new>
#include <memory>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cwchar>
#include <cwctype>
#include <cctype>
#include <cmath>
#include <cstdarg>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <list>
#include <set>
#include <stack>
#include <map>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <limits>
#include <functional>
#include <bitset>
#include <typeinfo>
#include <thread>
#include <mutex>
#include <chrono>
#include <stddef.h>
//#include "MacroDef.h"
//#include "PlatformDef.h"
//#include "Types.h"

// #include "Logger.h"

// Bring common functions from C into global namespace
using std::memcpy;
using std::fabs;
using std::sqrt;
using std::cos;
using std::sin;
using std::tan;
using std::isspace;
using std::isdigit;
using std::toupper;
using std::tolower;
using std::size_t;
using std::min;
using std::max;
using std::modf;
using std::atoi;

// Common
#ifndef NULL
#define NULL     0
#endif

/*
   Avoid "unused parameter" warnings
*/
#define UNUSED(x) (void)x;

#define ASSERT(...) // assert(expression)
#define ERROR(...)

// Object deletion macro
#define SAFE_DELETE(x) \
    { \
        delete x; \
        x = NULL; \
    }

// Array deletion macro
#define SAFE_DELETE_ARRAY(x) \
    { \
        delete[] x; \
        x = NULL; \
    }

// Ref cleanup macro
#define SAFE_RELEASE(x) \
    if (x) \
    { \
        (x)->release(); \
        x = NULL; \
    }

#define DISABLE_COPY(Class) \
    Class(const Class &);\
    Class &operator=(const Class &);


#define CHECK_GL_ERROR_DEBUG()
#define CC_SAFE_DELETE(x) SAFE_DELETE(x)

#define OS_ANDROID 0

enum Touch_Event {
    Touch_Down,
    Touch_Up,
    Touch_Move,
    Touch_Cancel
};

#endif
