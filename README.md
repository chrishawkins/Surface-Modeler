# Software Renderer for SDF for Windows

## Features

* multithreaded renderer
* composable SDF primitives
* minimal dependencies (includes Vector/Math libs)

## References

* SDF formulae: https://iquilezles.org/www/articles/distfunctions/distfunctions.htm
* Intro to Raymarching and SDF: http://jamie-wong.com/2016/07/15/ray-marching-signed-distance-functions/

Note, most tutorials are for GLSL implementations, so the main work here was understanding the GLSL logic and migrating it to C++.

## TODO

1. Support for transforms and camera positions
2. Support for pushing scene geometry to an OpenGL fragment shader for realtime rendering
