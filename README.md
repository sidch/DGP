# DGP
Toolkit for CS749: Digital Geometry Processing (http://www.cse.iitb.ac.in/~cs749)

Author: Siddhartha Chaudhuri. Released under the BSD license (see LICENSE.txt)

## Installation

To check out the repository:

```
git clone https://github.com/sidch/DGP
```

There is no separate installation step. Just copy the DGP/DGP subfolder to your source folder.

**Important:** Don't modify the DGP code for an assignment. We will replace your copy of DGP with ours when testing your submission. It should run as expected with our copy of DGP.

Recommended:
- Keep a master copy of DGP on your system, which you update regularly via `git pull origin master`. This copy will have the following directory structure:

```
Projects/DGP
|
+-- DGP
|   |
|   +-- Common.hpp
|   +-- Common.cpp
|   +-- Vector3.hpp
|   +-- ...
|
+-- README.md
+-- ...
```

- For every assignment, copy the latest version of the DGP/DGP subfolder to your assignment source folder.

```
Assigments/HW1
|
+-- src
    |
    +-- DGP
    |   |
    |   +-- Common.hpp
    |   +-- Common.cpp
    |   +-- Vector3.hpp
    |   +-- ...
    |
    +-- Foo.hpp
    +-- Foo.cpp
    +-- ...
```

## Documentation

To generate HTML documentation for the API, run Doxygen (http://www.doxygen.org) in the root DGP folder. Then, open `html/index.html` in a browser.

This is probably the best place to start looking at the toolkit.

Note that many convenience types, such as `Vector3` and `Matrix4`, are typedefs (for `VectorN<3, Real>` and `MatrixMN<3, 3, Real>` respectively) and don't show up in the Class Index. To see them, go to Namespaces --> Namespace Members --> Typedefs.

## Using the code

The toolkit is written in standard C++ and should compile with any modern, standards-compliant, C++11 compiler. The usual candidates are GCC, Clang and Visual C++. You will need to add `-std=c++11` for the first two. The syntax is actually C++98-compatible; only certain library features (such as `unordered_set` and `type_traits`) require C++11.

When building your assignment, make sure the `.cpp` files in the `src/DGP` subfolder are included in the build. If you're using a Makefile, use a wildcard pattern such as `$(wildcard DGP/*.cpp)`. If you're using Visual Studio or other IDE, add the subfolder to the project. You might need to explicitly link the standard math library via `-lm`. On OS X, you will need to add `-framework Carbon`.

In your source files (such as `Foo.cpp` above), include the appropriate DGP headers as needed:

```c++
#include "DGP/Vector3.hpp"
#include "DGP/Ball3.hpp"

class Foo
{
  private:
    DGP::Vector3 position;
    DGP::Ball3 bounding_sphere;

  ...
};
```
For GCC/Clang, I recommend building with `-Wall -g2 -O2` (all **W**arnings, debu**G**gable binaries, **O**ptimize for speed). ``-O2`` messes up the debugging a bit so turn it off temporarily if you can't track down your bug.
