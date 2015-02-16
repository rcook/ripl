# Richard's image-processing library

## Overview

This is more a historial curiosity than anything else. This is the first "real"
code I ever wrote as a graduate student working in signal processing back in
1998. This code was the main side product of my master's dissertation entitled
"Image-processing techniques with application to image restoration". My coding
skills have come on along a great deal since then.

## Building

I've updated this project to use [CMake](http://www.cmake.org/). Run the `build`
script from the root of the repository to build the project. This will generate
a Unix-style Makefile and build it, for example:

```bash
$ ./build Debug
```

The original build scripts for Borland C++ 4.x and whatever version of HP-UX was
current in 1998 are provided in the `src` subdirectory. Fortunately, the project
compiles almost completely cleanly using a modern toolchain.

## Licence

Released under the MIT licence.

