#include "unistd.h"
#include "stdio.h"
#include "GLFW/glfw3.h"
#include "Flags.h"
#include "Log.h"
//#include "gestureManager.h"
//#include "gestureObject.h"
//#include "gestureCommonFun.h"
#include "CEGuiRender.h"
#include "Director.h"

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
    // printf("\n onMouseEvent: event:%d x:%d, y:%d flg:0x%08x +++ \n", event.event, event.x, event.y, event.status);

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
    printf("\n framebuffsize_callback:%p  w:%d h:%d +++ \n", window, width, height);
    Director::instance()->setWindowSize(width, height);
    printf("\n framebuffsize_callback --- \n");
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    // printf("\n mouse_callback :%p button:%d action:%d mods:%d +++ \n", window, button, action, mods);
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
    // printf("\n mouse_callback --- \n");
}

void cursorenter_callback(GLFWwindow* window, int enter)
{
    printf("\n cursorenter_callback :%p enter:%d +++ \n", window, enter);

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

    printf("\n cursorenter_callback :%p --- \n", window);
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
    // printf("\n cursorpos_callback :%p x:%d y:%d \n", window, s_mouseevent.x, s_mouseevent.y);
    s_mouseevent.event = MOUSE_MOVE;
    onMouseEvent(s_mouseevent);
}

void dropfile_callback(GLFWwindow* window, int count, const char** path) {
    if (count <= 0 || path == NULL) {
        return;
    }
    printf("\n dropfile_callback :%p count:%d +++ \n", window, count);
    for (int i = 0; i < count; ++i) {
        printf("\n [%d][%s] \n", i, path[i]);
    }
    printf("\n dropfile_callback :%p --- \n", window);
}

void close_callback(GLFWwindow* window)
{
     printf("\n close_callback :%p \n", window);
}

//void scroll_callback(GLFWwindow* window, double x, double y)
//{
//    printf("\n scroll_callback :%p x:%.1lf y:%.1lf +++ \n", window, x, y);
//    printf("\n scroll_callback --- \n");
//}


void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    LOG_BASE_TRACE("process_input_callabck:%p  key:%d scancode:%d  action:%d mods:%d", window, key, scancode, action, mods);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
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

#define DEFAULT_SIZE_WIDTH (1280)
#define DEFAULT_SIZE_HEIGHT (720)
#define DEFAULT_PIXEL_FACTOR (1.0)
#define TITLE_NAME ("Tools")

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
        Director::instance()->setTouchRegion(160, 0, DEFAULT_SIZE_HEIGHT, DEFAULT_SIZE_HEIGHT);
//        loadBmp(0, "/xxxx/Cube.bmp", 150, 150);
//        for (int i = 0; i < 6; ++i)
//        Director::instance()->setPlaneTexture(i, 0);


        while (!glfwWindowShouldClose(window)) {
//            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//            glClear(GL_COLOR_BUFFER_BIT);
//            glfwSwapBuffers(window);

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
