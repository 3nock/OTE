# OTE's Code Style Guide

This document contains the proposed code style guide for the entire OTE's project code base. This style guide closely resembles the Google's c++ style guide.

## C++ Version

Code should target `C++11`

## Header Files

- In general, every .cpp or .c file should have an associated .h file.

- All header files should have #define guards to prevent multiple inclusion. 
The format of the symbol name should be `<PATH>_<FILE>_H`.

e.g. Header guard for file _**OTE/src/core/config.h**_ header should be:
``` cpp
#define CORE_CONFIG_H
```
- Include only what you use.

- Don’t use forward declaration unless it is necessaly. Always include the headers you need.

## Order of Includes

- Include headers in the following order: Related header, C system headers, C++
standard library headers, Qt library headers, other libraries' headers, project's headers.

e.g for file in _**src/core/config.cpp**_ the includes will be;
``` cpp
#include "config.h"

#include <sys/types.h>
#include <unistd.h>

#include <string>
#include <vector>

#include <QList>
#include <QbyteArray>

#include <EAtomic/EList.h>
#include <EAtomic/EQueue.h>

#include "core/bar.h"
```

- Separate each non-empty group with one blank line.

- Arrange the includes in an alphabetic order

## Inline Functions

Define functions inline only when they are small, say, 10 lines or fewer.

## Namespaces
- With few exceptions, place code in a namespace.

- Do not use using-directives (e.g. `using namespace foo`).

- For this project the main namespace is `OTE`. 

## Naming Conventions

### Files
- C++ source files have a `.cpp` extension, C source files `.c`, and headers for both use `.h`.

- Prefer naming the file the same as the (main) class it contains.

- Avoid having multiple files with the same name in different places within the same library.

### C++ code
- Start types (such as `classes` and `structs`) with a capital letter.

- Start other names (such as `functions` and `variables`) with a lowercase letter.

- C++ interfaces are named with a `Interface` suffix, and abstract base classes with an `Abstract` prefix.

- Private member variables are named with a `m` prefix.

- Global variables are named with a `g` prefix.

<!-- - Static class variables are named with a `s` prefix. -->

- Global constants are often named with a `c` prefix.
