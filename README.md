# deskpilot

**a modular c++ desktop control engine for autonomous ai agents.**

deskpilot is an experimental linux-based desktop automation engine designed to give ai agents the ability to visually navigate and interact with graphical environments through independent cursor control, human-like movement, and simulated input.

the project aims to provide a lightweight, extensible interface for autonomous desktop interaction, with future integration into larger ai agent systems.

## features

**currently implemented**
- x11-based cursor visualization
- human mouse movement recording
- movement sampling and stroke detection
- recorded trajectory transformation and playback
- basic mouse input simulation via xtest

**planned**
- independent cursor and keyboard control
- isolated desktop environments
- screen capture and visual understanding
- natural cursor movement generation
- window management and application interaction
- autonomous task execution
- external ai agent integration

## architecture

deskpilot is being developed in modular c++, separating cursor management, movement generation, input recording, and desktop interaction into independent components.

```text
src/
├── main.cpp
├── AICursor.hpp
├── AICursor.cpp
├── MouseRecorder.hpp
├── MouseRecorder.cpp
├── MovementEngine.hpp
└── MovementEngine.cpp
```

## tech stack

- **language:** c++
- **platform:** linux
- **display system:** x11
- **libraries:** xlib, xtest
- **build tools:** g++

## status

> early development — experimental prototype.

deskpilot is an ongoing learning and development project. its architecture, functionality, and interfaces are subject to change as development progresses.

## roadmap

- [x] initial cursor prototype
- [x] movement recording and playback
- [ ] modularize core components
- [ ] implement isolated input control
- [ ] develop visual desktop perception
- [ ] integrate autonomous agent reasoning
- [ ] expose a reusable desktop control api

## license

no license has been selected yet.
