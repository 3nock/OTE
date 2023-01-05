# Compiling The Project

This is just a simple instruction on compiling the project. More details will be added later.
The Steps are easy since the project doesn't have many dependancies for now & only uses QtCreator/qmake for the whole build process.

## Prerequisites

1. Download [Qt5.13.0](https://download.qt.io/archive/qt/5.13/5.13.0) & install. (other versions can be used, i've only tested in Qt5.13 & Qt5.12).
2. Download [Qt Creator 4.9.1](https://www.qt.io/offline-installers) & install. (not a must to use version 4.9.1).
3. Clone the repository (`main branch`) to your local drive. Make sure to include the submodules in your clone command!

 **For main-branch (stable)**
 
`git clone --recurse-submodules -b main https://github.com/3nock/sub3suite.git`

## Project

If you are familiar with Qt-Creator.
1. Load the `sub3suite.pro` project into the Qt creator.
2. Choose the compiler and build path for the project.

## Minor adjustment

Minor adjustment to the gumbo-parser submodule. 
1. Copy the `strings.h` file from `sub3suite/sub3suite/include/`
2. Paste it to `sub3suite/sub3suite/include/gumbo-parser/src/`
3. Change the Include header file `#include <strings.h>` to `#include "strings.h"` for the following files;
	- attribute.c
	- parser.c
	- string_buffer.c
	- string_piece.c
	- utf8.c
	- util.c
	- vector.c
	
Minor adjustment to the breakpad submodule.
1. Create a folder `lss` inside `sub3suite/sub3suite/include/breakpad/src/third_party`.
2. Copy `linux_syscall_support.h` file from `sub3suite/include`.
3. Paste it to `sub3suite/sub3suite/include/breakpad/src/third_party/lss`.
	
## Build

1. If you are using QtCreator. Then just open the project file `sub3suite.pro` in QtCreator, configure compiler & build.
2. If you are using qmake, then just use the following commands to build the project respective to your system.

- Linux
``` bash
mkdir build && cd build
qmake CONFIG+=release ../sub3suite/sub3suite
make
```

- MacOS
``` bash
mkdir build && cd build
qmake CONFIG+=release ../sub3suite/sub3suite
make
macdeployqt sub3suite.app
```

- Windows
``` bash
mkdir build && cd build
qmake CONFIG+=release ../sub3suite/sub3suite
make
windeployqt release/sub3suite.exe
```

## Run

The folder `sub3suite/build` contains all the necessary files required by the sub3suite program. Copy the relevant Qt libraries (Qt5Widgets, Qt5Network, Qt5Gui & Qt5Core) to appropriate locations and the built sub3suite
application into this folder and run.
