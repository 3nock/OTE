# Compiling OTE Project

This is just a simple instruction on compiling the project. More details will be added later.
The Steps are easy since the project doesn't have many dependancies for now & only uses QtCreator/qmake for the whole build process.

## Prerequisites

1. Download [Qt5.13.0](https://download.qt.io/archive/qt/5.14/5.14.2) & install. (other versions can be used, i've only tested in Qt5.14).
2. Download [Qt Creator](https://www.qt.io/offline-installers) & install.
3. Clone the repository (`main branch`) to your local drive.

 **For main-branch (stable)**
 
`git clone https://github.com/3nock/OTE.git`

## Project

If you are familiar with Qt-Creator.
1. Load the `OTE.pro` project into the Qt creator.
2. Choose the compiler and build path for the project.
	
## Build

1. If you are using QtCreator. Then just open the project file `OTE.pro` in QtCreator, configure compiler & build.
2. If you are using qmake, then just use the following commands to build the project respective to your system.

- Linux
``` bash
mkdir build && cd build
qmake CONFIG+=release ../OTE/OTE
make
```

- MacOS
``` bash
mkdir build && cd build
qmake CONFIG+=release ../OTE/OTE
make
macdeployqt OTE.app
```

- Windows
``` bash
mkdir build && cd build
qmake CONFIG+=release ../OTE/OTE
make
windeployqt release/OTE.exe
```
