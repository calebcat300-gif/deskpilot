#include <X11/Xlib.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <vector>
#include <X11/extensions/XTest.h>

struct MouseSample {
    int dx;
    int dy;
};

struct MouseStroke {
    std::vector<MouseSample> samples;
};

int main() {
    Display* display = XOpenDisplay(nullptr);

    if (display == nullptr) {
        std::cout << "Failed to connect to X11\n";
        return 1;
    }

    std::cout << "Connected\n";
    std::cout << "Move your mouse normally for 10 seconds...\n";

    int screen = DefaultScreen(display);
    Window root = RootWindow(display, screen);

    XSetWindowAttributes attributes;
    attributes.override_redirect = True;
    attributes.background_pixel = WhitePixel(display, screen);

    Window aiCursor = XCreateWindow(
        display,
        root,
        100,
        100,
        20,
        20,
        0,
        CopyFromParent,
        InputOutput,
        CopyFromParent,
        CWOverrideRedirect | CWBackPixel,
        &attributes
    );

    XMapWindow(display, aiCursor);
    XFlush(display);

    Window returnedRoot;
    Window returnedChild;

    int mouseX;
    int mouseY;
    int windowX;
    int windowY;

    int lastMouseX = 0;
    int lastMouseY = 0;

    bool firstSample = true;

    unsigned int mask;

    std::vector<MouseStroke> strokes;
    MouseStroke currentStroke;

    int stillSamples = 0;

    auto startTime = std::chrono::steady_clock::now();

    while (true) {
        auto now = std::chrono::steady_clock::now();

        double elapsed =
            std::chrono::duration<double>(
                now - startTime
            ).count();

        if (elapsed >= 10.0) {
            break;
        }

        XQueryPointer(
            display,
            root,
            &returnedRoot,
            &returnedChild,
            &mouseX,
            &mouseY,
            &windowX,
            &windowY,
            &mask
        );

        if (!firstSample) {
            int dx = mouseX - lastMouseX;
            int dy = mouseY - lastMouseY;

            if (dx == 0 && dy == 0) {
                stillSamples++;

                // 3 samples = about 150 ms of being still.
                // Treat that as the end of one mouse stroke.
                if (
                    stillSamples >= 3 &&
                    !currentStroke.samples.empty()
                ) {
                    strokes.push_back(currentStroke);
                    currentStroke.samples.clear();
                }
            }
            else {
                stillSamples = 0;

                MouseSample sample;
                sample.dx = dx;
                sample.dy = dy;

                currentStroke.samples.push_back(sample);
            }
        }

        // AI cursor still follows you while recording.
        XMoveWindow(
            display,
            aiCursor,
            mouseX + 50,
            mouseY + 50
        );

        XFlush(display);

        lastMouseX = mouseX;
        lastMouseY = mouseY;
        firstSample = false;

        std::this_thread::sleep_for(
            std::chrono::milliseconds(50)
        );
    }

    // Save unfinished stroke too.
    if (!currentStroke.samples.empty()) {
        strokes.push_back(currentStroke);
    }

    std::cout
        << "\nCaptured "
        << strokes.size()
        << " mouse strokes.\n";

    if (strokes.empty()) {
        std::cout << "No usable movement captured.\n";

        XDestroyWindow(display, aiCursor);
        XCloseDisplay(display);

        return 1;
    }

    // Pick the stroke containing the most samples.
    int bestStrokeIndex = 0;

    for (int i = 1; i < static_cast<int>(strokes.size()); i++) {
        if (
            strokes[i].samples.size() >
            strokes[bestStrokeIndex].samples.size()
        ) {
            bestStrokeIndex = i;
        }
    }

    MouseStroke& stroke = strokes[bestStrokeIndex];

    std::cout
        << "Using stroke with "
        << stroke.samples.size()
        << " samples.\n";

    // Figure out the total direction/distance
    // of the original human movement.
    double sourceDX = 0.0;
    double sourceDY = 0.0;

    for (const MouseSample& sample : stroke.samples) {
        sourceDX += sample.dx;
        sourceDY += sample.dy;
    }

    double sourceDistance =
        std::sqrt(
            sourceDX * sourceDX +
            sourceDY * sourceDY
        );

    if (sourceDistance < 1.0) {
        std::cout << "Captured stroke was too small.\n";

        XDestroyWindow(display, aiCursor);
        XCloseDisplay(display);

        return 1;
    }

    // Start AI where your cursor finished.
    double cursorX = mouseX + 50;
    double cursorY = mouseY + 50;

    double startX = cursorX;
    double startY = cursorY;

    // Completely new destination.
    double targetX = 2200.0;
    double targetY = 1100.0;

    double targetDX = targetX - startX;
    double targetDY = targetY - startY;

    double targetDistance =
        std::sqrt(
            targetDX * targetDX +
            targetDY * targetDY
        );

    // Determine how much to rotate the original movement.
    double sourceAngle =
        std::atan2(sourceDY, sourceDX);

    double targetAngle =
        std::atan2(targetDY, targetDX);

    double rotation =
        targetAngle - sourceAngle;

    double cosRotation = std::cos(rotation);
    double sinRotation = std::sin(rotation);

    // Resize the original movement so it reaches the new target.
    double scale =
        targetDistance / sourceDistance;

    double pathX = 0.0;
    double pathY = 0.0;

    std::cout << "\nAI moving using your captured motion...\n";

    for (const MouseSample& sample : stroke.samples) {
        // Rotate original human movement.
        double rotatedDX =
            sample.dx * cosRotation -
            sample.dy * sinRotation;

        double rotatedDY =
            sample.dx * sinRotation +
            sample.dy * cosRotation;

        // Scale it for the new travel distance.
        rotatedDX *= scale;
        rotatedDY *= scale;

        pathX += rotatedDX;
        pathY += rotatedDY;

        cursorX = startX + pathX;
        cursorY = startY + pathY;

        XMoveWindow(
            display,
            aiCursor,
            static_cast<int>(cursorX),
            static_cast<int>(cursorY)
        );

        XFlush(display);

        // SAME sampling timing as your recording.
        std::this_thread::sleep_for(
            std::chrono::milliseconds(50)
        );
    }

    // Correct tiny transformation/rounding error at the end.
    XMoveWindow(
        display,
        aiCursor,
        static_cast<int>(targetX),
        static_cast<int>(targetY)
    );

    XTestFakeMotionEvent(
        display,
        screen,
        static_cast<int>(targetX),
        static_cast<int>(targetY),
        CurrentTime
    );

    XFlush(display);

    std::this_thread::sleep_for(
        std::chrono::milliseconds(100)
    );

    XTestFakeButtonEvent(
        display,
        1,
        True,
        CurrentTime
    );

    XTestFakeButtonEvent(
        display,
        1,
        False,
        CurrentTime
    );

    XFlush(display);
    XFlush(display);

    std::cout
        << "Reached target: "
        << targetX
        << "x"
        << targetY
        << '\n';

    std::this_thread::sleep_for(
        std::chrono::seconds(3)
    );

    XDestroyWindow(display, aiCursor);
    XCloseDisplay(display);

    return 0;
}
