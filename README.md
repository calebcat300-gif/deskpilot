# deskpilot

**a cross-platform c++ desktop control engine for autonomous ai agents.**

deskpilot is an experimental desktop interaction engine designed to enable ai agents to visually navigate, interact with, and operate graphical applications through independent input control and human-like behavior.

the long-term goal is to provide a lightweight, modular, and cross-platform desktop automation framework compatible with windows, linux, and a wide range of graphical environments.

## vision

deskpilot aims to give ai agents the ability to operate a computer much like a human user, while maintaining an independent workspace that doesn't interfere with the host user's input.

the engine is designed around realistic interaction, visual understanding, platform independence, and integration with larger autonomous ai systems.

## features

**currently implemented**
- x11-based cursor visualization
- human mouse movement recording
- movement sampling and stroke detection
- recorded trajectory transformation and playback
- basic mouse input simulation via xtest

**planned**
- independent mouse and keyboard control
- isolated desktop sessions
- cross-platform desktop interaction
- screen capture and visual understanding
- realistic cursor movement generation
- clicking, scrolling, dragging, and typing
- window management and application control
- autonomous task execution and verification
- external ai agent integration
- reusable desktop control api

## platform compatibility

| platform | status |
|---|---|
| linux / x11 | experimental prototype |
| linux / wayland | planned |
| windows | planned |
| additional graphical environments | future support |

platform-specific backends will be developed to accommodate differences in graphics, input handling, and desktop security.

## architecture

deskpilot is designed around a modular c++ architecture, separating desktop interaction, input recording, movement generation, and platform-specific implementations.

```text
             ai agent / omp
                   |
           desktop control api
                   |
       +-----------+-----------+
       |           |           |
     input       vision     movement
       |           |           |
       +-----------+-----------+
                   |
           platform backends
                   |
          +--------+--------+
          |                 |
       linux             windows
     x11/wayland         win32
```

## tech stack

- **language:** c++
- **current platform:** linux
- **current display system:** x11
- **current libraries:** xlib, xtest
- **compiler:** g++

## roadmap

- [x] initial cursor prototype
- [x] mouse movement recording and basic playback
- [ ] modularize core c++ components
- [ ] implement independent desktop input
- [ ] develop visual desktop perception
- [ ] introduce a platform abstraction layer
- [ ] implement windows compatibility
- [ ] investigate wayland support
- [ ] build reusable desktop control api
- [ ] integrate autonomous agent reasoning
- [ ] integrate with omp

## status

> early development — experimental prototype.

deskpilot currently targets linux/x11. cross-platform functionality, independent input isolation, and ai-driven desktop interaction remain under development.

## license

no license has been selected yet.
