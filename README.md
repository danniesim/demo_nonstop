demo_nonstop
============

Demo Code for Nonstop Games

Author: Daniel Sim

Overview
--------
2 projects in here:
* coms_server
* slash_engine

Only selected classes are included in this demo. i.e. You can't compile this.

More comments would have been better, I hope the naming conventions help.

coms_server
-----------
An uncompleted multi-player TCP/IP based server daemon that exists in it's current form as a telnet echo server.

It was implemented with pthread and was designed to be capable of accepting hundreds and quite possibly thousands of connections simultaneously.

Code reliablity and ease-of-debug where major considerations in its C++ OOP architecture. The code works in Windows, Linux and OSX.

slash_engine
------------
A multi-platform low-footprint 3D game engine. It cross-compiles in XCode, Android NDK and Windows/MinGW.

OpenAL, GLSL, OpenGL ES 2.0 were used as the foundation to ensure cross-platform operability. Any hacks needed for the engine to communicate with the OS was implemented in sub-classes of OS and UserController.

I looked around for a light-weight physics engine, but figured all I needed was light-weight mesh collision solution, so I made my own.

A highlight was the custom export of skeletal animation data from Autodesk Maya and rendered using GPU-side mesh skinning code.
