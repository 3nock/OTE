# Compiling The Project

This is just a simple instruction on compiling the project. More details will be added later.
The Steps are easy since the project doesn't have many dependancies for now & only uses Qt Creator for the whole build process.

## Prerequisites

1. Download [Qt5.13.0](https://download.qt.io/archive/qt/5.13/5.13.0), & install.(not a must to use version 5.13.0).
2. Download [Qt Creator 4.9.1](https://www.qt.io/offline-installers), & install.(not a must to use version 4.9.1).
3. Clone the repository (`main or dev branch`) to your local drive. Make sure to include the submodules in your clone command!

 ** For main-branch (stable) **
`git clone --recurse-submodules -b main https://github.com/3nock/sub3suite.git`
 ** For dev-branch (development) **
`git clone --recurse-submodules -b dev https://github.com/3nock/sub3suite.git`

## Project

If you are familiar with Qt.
1. Load the sub3suite.pro project into the Qt creator.
2. Choose the compiler and build path for the project.

## Minor adjustment

Minor adjustment to the GumboParser submodule. 
1. Copy the `strings.h` file from `sub3suite/include/gumbo-parser/visualc/include/`
2. Paste it to `sub3suite/include/gumbo-parser/src/`
3. Include the header file `#include "strings.h"` to the following files
	- attribute.c
	- parser.c
	- string_buffer.c
	- string_piece.c
	- utf8.c
	- vector.c
	
## Build

1. If you are using Qt creator, then just click to build.
2. If you are using qmake, then just use the following commands to build the project respective to your system.

- Linux
``` bash
mkdir build && cd build
qmake sub3suite.pro CONFIG+=release ../sub3suite
make
```

- MacOS
``` bash
mkdir build && cd build
qmake sub3suite.pro CONFIG+=release ../sub3suite
make
macdeployqt sub3suite.app
```

- Windows
``` bash
mkdir build && cd build
qmake sub3suite.pro CONFIG+=release ../sub3suite
nmake
windeployqt release\sub3suite.exe
```

## Run
The required dynamic/shared libraries for this project are `Qt5Core`, `Qt5Gui`, `Qt5Network` & `Qt5Widgets`. You can obtain them
from the Qt installation folder (Make sure you get the appropriate versions).

The folder `sub3suite/bin` contains all the necessary files required by the sub3suite program. Copy the Qt libraries and the built sub3suite
application into this folder and run!.