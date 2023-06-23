# OTE's Code Style Guide

This document contains the proposed code style guide for the entire OTE's project code base. This style guide closely resembles the Google's c++ style guide.

## C++ Version

Code should target `C++11`

## Header Files

- In general, every .cpp or .c file should have an associated .h file.

- Header files should be self-contained (compile on their own) and end in .h. Non-header
files that are meant for inclusion should end in .inc and be used sparingly.

- All header files should have #define guards to prevent multiple inclusion. 
The format of the symbol name should be `PATH>_<FILE>_H`.

e.g. Header guard for file _**SpiderSuite/src/core/config.h**_ header should be:
``` cpp
#define CORE_CONFIG_H
```
- Include only what you use.

- Don’t use forward declaration unless it is necessaly. Always include the headers you need.

## Order of Includes

- Include headers in the following order: Related header, C system headers, C++
standard library headers, Qt library headers, other libraries' headers, your project's headers.

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
#include "third_party/absl/flags/flag.h"
```
- Sometimes, system-specific code needs conditional includes. Such code can put
conditional includes after other includes. Of course, keep your system-specific code
small and localized.
``` cpp
#include "core/bar.h"
#include "core/basictypes.h"
#include "third_party/absl/flags/flag.h"

#ifdef LANG_CXX11
#include <initializer_list>
#endif // LANG_CXX11
```

- Separate each non-empty group with one blank line.

- Arrange the includes in an alphabetic order

## Inline Functions

Define functions inline only when they are small, say, 10 lines or fewer.

## Namespaces
- With few exceptions, place code in a namespace.

- Namespaces should have unique names based on 
the project name, and possibly its path. 

- Do not use using-directives (e.g. `using namespace foo`). 

- Do not use inline namespaces.

- For this project the main namespace is `SSuite`. 

## Name Conventions