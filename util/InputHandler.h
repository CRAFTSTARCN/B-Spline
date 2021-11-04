#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class InputHandler {

    private:
    static GLFWwindow* bandedWindow;

    static double mousePosX;
    static double mousePosY;

    static bool lClick;
    static bool rClick;
    static bool mClick;

    static bool lRelease;
    static bool rRelease;
    static bool mRelease;

    static bool lStatus;
    static bool rStatus;
    static bool mStatus;

    static double scrollOffsetX;
    static double scrollOffsetY;

    static uint32_t keyStat[];
    static int unitCount;

    static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

    static void cursorPositionCallback(GLFWwindow* window, double x, double y);

    static void mouseDownCallBack(GLFWwindow* window, int button, int action, int mods);

    static void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mod);

    public:
    static void init(GLFWwindow* window);

    static bool getMouseDownL();
    static bool getMouseDownR();
    static bool getMouseDownM();

    static bool getMouseUpL();
    static bool getMouseUpR();
    static bool getMouseUpM();


    static bool getMouseL();
    static bool getMouseR();
    static bool getMouseM();

    static float getMouseX();

    static float getMouseY();

    static bool getKeyDown(int key);

    /*Should be called before poll event, after every thing down*/
    static void clearStatus();

    static float getScrollOffsetX();
    static float getScrollOffsetY();
};

#endif