#include "unistd.h"
#include "stdio.h"
#include "GLFW/glfw3.h"
#include "Flags.h"
#include "Log.h"
#include "CEGuiRender.h"
#include "Director.h"

#define NLOG(...) // LOG_BASE(__VA_ARGS__)
#define NLOG_TRACE(...) // LOG_BASE_TRACE(__VA_ARGS__)

enum MOUSE_STATUS {
    MOUSE_ENTER = 0x0001,
    MOUSE_LEFT = 0x0002,
    MOUSE_RIGHT = 0x0004,
    MOUSE_MIDDLE = 0x0008,
};

DECLARE_FLAGS(MouseStatus, MOUSE_STATUS)

enum MOUSE_EVENT {
    MOUSE_DOWN_LEFT,
    MOUSE_UP_LEFT,
    MOUSE_DOWN_RIGHT,
    MOUSE_UP_RIGHT,
    MOUSE_DOWN_MIDDLE,
    MOUSE_UP_MIDDLE,
    MOUSE_DOWN_ALL,
    MOUSE_UP_ALL,
    MOUSE_MOVE,

    MOUSE_DOWN = MOUSE_DOWN_LEFT,
    MOUSE_UP = MOUSE_UP_LEFT,
};

struct MouseEvent {
    int event;
    int x;
    int y;
    MouseStatus status;
};
static MouseEvent s_mouseevent;


void onMouseEvent(const MouseEvent& event) {
    NLOG("onMouseEvent: event:%d x:%d, y:%d flg:0x%04x", event.event, event.x, event.y, event.status);

    TouchAction touchevent;
    switch (event.event) {
    case MOUSE_DOWN:
        touchevent = TouchAction_Down;
        break;
    case MOUSE_UP:
        touchevent = TouchAction_Up;
        break;
    case MOUSE_MOVE:
        touchevent = TouchAction_Move;
        break;
    default:
        return;
    }

    TouchEvent touchev;
    touchev.id = 0;
    touchev.action = touchevent;
    touchev.x = event.x;
    touchev.y = event.y;
    Director::instance()->injectTouch(touchev);
}

void framebuffsize_callback(GLFWwindow* window, int width, int height) {
    NLOG_TRACE("framebuffsize_callback:%p  w:%d h:%d ", window, width, height);
    Director::instance()->setWindowSize(width, height);
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    NLOG_TRACE("mouse_callback :%p button:%d action:%d mods:%d", window, button, action, mods);
    enum {
        MOUSEBUTTON_LEFT = 0,
        MOUSEBUTTON_RIGHT,
        MOUSEBUTTON_MIDDLE
    };

    if (!s_mouseevent.status.testFlag(MOUSE_ENTER)) {
        return;
    }

    switch (button) {
    case MOUSEBUTTON_LEFT:
        if (GLFW_PRESS == action) {
            s_mouseevent.event = MOUSE_DOWN_LEFT;
            s_mouseevent.status.setFlag(MOUSE_LEFT, true);
        }
        else {
            s_mouseevent.event = MOUSE_UP_LEFT;
            s_mouseevent.status.setFlag(MOUSE_LEFT, false);
        }
        break;
    case MOUSEBUTTON_RIGHT:
        if (GLFW_PRESS == action) {
            s_mouseevent.event = MOUSE_DOWN_RIGHT;
            s_mouseevent.status.setFlag(MOUSE_RIGHT, true);
        }
        else {
            s_mouseevent.event = MOUSE_UP_RIGHT;
            s_mouseevent.status.setFlag(MOUSE_RIGHT, false);
        }
        break;
    case MOUSEBUTTON_MIDDLE:
        if (GLFW_PRESS == action) {
            s_mouseevent.event = MOUSE_DOWN_MIDDLE;
            s_mouseevent.status.setFlag(MOUSE_MIDDLE, true);
        }
        else {
            s_mouseevent.event = MOUSE_UP_MIDDLE;
            s_mouseevent.status.setFlag(MOUSE_MIDDLE, false);
        }
        break;
    default:
        return;
    }
    onMouseEvent(s_mouseevent);
}

void cursorenter_callback(GLFWwindow* window, int enter)
{
    NLOG_TRACE("cursorenter_callback :%p enter:%d ", window, enter);

    bool preEnter = s_mouseevent.status.testFlag(MOUSE_ENTER);
    bool curEnter = (enter == GL_TRUE) ? true : false;
    if (preEnter == curEnter) {
        return;
    }
    s_mouseevent.status.setFlag(MOUSE_ENTER, curEnter);

    if (s_mouseevent.status.testFlag(MOUSE_ENTER)) {
        return;
    }

    if (!s_mouseevent.status.testFlag(MOUSE_LEFT)
        && !s_mouseevent.status.testFlag(MOUSE_RIGHT)
        && !s_mouseevent.status.testFlag(MOUSE_MIDDLE)) {
        return;
    }

    s_mouseevent.event = MOUSE_UP_ALL;
    s_mouseevent.status.setFlag(MOUSE_LEFT, false);
    s_mouseevent.status.setFlag(MOUSE_RIGHT, false);
    s_mouseevent.status.setFlag(MOUSE_MIDDLE, false);
    onMouseEvent(s_mouseevent);

}

void cursorpos_callback(GLFWwindow* window, double x, double y)
{
    s_mouseevent.x = (int)x;
    s_mouseevent.y = (int)y;
    if (!s_mouseevent.status.testFlag(MOUSE_ENTER)) {
        return;
    }

    if (!s_mouseevent.status.testFlag(MOUSE_LEFT)
        && !s_mouseevent.status.testFlag(MOUSE_RIGHT)
        && !s_mouseevent.status.testFlag(MOUSE_MIDDLE)) {
        return;
    }
    NLOG("cursorpos_callback :%p x:%d y:%d", window, s_mouseevent.x, s_mouseevent.y);
    s_mouseevent.event = MOUSE_MOVE;
    onMouseEvent(s_mouseevent);
}

void dropfile_callback(GLFWwindow* window, int count, const char** path) {
    if (count <= 0 || path == NULL) {
        return;
    }
    NLOG_TRACE("dropfile_callback :%p count:%d ", window, count);
    for (int i = 0; i < count; ++i) {
        NLOG(" [%d][%s] ", i, path[i]);
    }

}

void close_callback(GLFWwindow* window)
{
     NLOG("close_callback :%p ", window);
}

//void scroll_callback(GLFWwindow* window, double x, double y)
//{
//    NLOG_TRACE("scroll_callback :%p x:%.1lf y:%.1lf ", window, x, y);
//}


void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    LOG_BASE_TRACE("process_input_callabck:%p  key:%d scancode:%d  action:%d mods:%d", window, key, scancode, action, mods);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        return;
    }

    KeyEvent event;
    event.action = action;
    event.key = key;
    event.mods = mods;
    event.scancode = scancode;

    Director::instance()->injectKey(event);
}

GLFWwindow* initGLWindow(const char* title, int w, int h)
{

    GLFWwindow* window = glfwCreateWindow(w, h, title ? title : " ", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetCursorPosCallback(window, cursorpos_callback);
    glfwSetCursorEnterCallback(window, cursorenter_callback);
    // glfwSetScrollCallback(window, scroll_callback);

    glfwSetFramebufferSizeCallback(window, framebuffsize_callback);
    glfwSetWindowCloseCallback(window, close_callback);
    glfwSetDropCallback(window, dropfile_callback);
    return window;
}

#define DEFAULT_SIZE_WIDTH (256*5)
#define DEFAULT_SIZE_HEIGHT (160*5)
#define DEFAULT_PIXEL_FACTOR (1.0)
#define TITLE_NAME ("Test")

int main(int argc, char** argv)
{  

    glfwInit();
    CEGuiRender render;

    do {
        GLFWwindow* window = initGLWindow(TITLE_NAME, DEFAULT_SIZE_WIDTH, DEFAULT_SIZE_HEIGHT);
        if (NULL == window) {
            break;
        }

        // glViewport(0, 0, DEFAULT_SIZE_WIDTH, DEFAULT_SIZE_HEIGHT);
        render.initRender();

        Director::instance()->init();
        Director::instance()->setWindowSize(DEFAULT_SIZE_WIDTH, DEFAULT_SIZE_HEIGHT);
        Director::instance()->setTouchRegion((DEFAULT_SIZE_WIDTH-DEFAULT_SIZE_HEIGHT)/2, 0, DEFAULT_SIZE_HEIGHT, DEFAULT_SIZE_HEIGHT);

        while (!glfwWindowShouldClose(window)) {
//            if (render.loop()) {
//                glfwSwapBuffers(window);
//            }

            Director::instance()->loop();
            glfwSwapBuffers(window);

            glfwPollEvents();
        }
    } while (0);

    Director::instance()->deinit();
    Director::destroy();
    render.deinitRender();
    glfwTerminate();
    return 0;
}
