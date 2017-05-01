### BLUI Build Script

#### Windows Steps

Requirements:
```
Python 2.7 (latest)
Visual Studio 2015
CMake
Patch
Git
```

* Ensure that `git, python, msbuild, patch, cmake` are all added to your PATH and can be run from the command line.
* Copy `builder.py` to some other directory.
* Use command prompt to navigate to the builder.py location.
* Run `python builder.py` and provide the Visual Studio version (2015 is recommended)
* Wait. A very long time. Really.
* If no errors. Folder `blui` will contain all source and binary files needed for the `Plugins` folder.

> Other platforms for this script are still a WIP...