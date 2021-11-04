#include "core/Window.h"

int main(int argc, char* argv[]) {
    initGLFW(3,3);
    Window win(900,900,"B-Spline");
    win.initWindow();
    win.setUpRendProp();
    win.mainLoop();
    return 0;
}
