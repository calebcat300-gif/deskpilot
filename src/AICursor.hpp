#pragma once

#include <X11/Xlib.h>

class AICursor {
public:
    AICursor(Display* display, Window root, int screen);

    void show();
    void moveTo(int x, int y);

private:
    Display* display;
    Window window;
    int screen;
};