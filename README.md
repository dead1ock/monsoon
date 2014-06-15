#Monsoon
Monsoon is an open source, modular, game and real-time simulation engine. For more information, visit http://www.monsoonengine.com.

##Platforms
* Windows

##Dependencies
###Required
* C++11
* Git
* CMake 2.8+
* Python 3.4
* Boost 1.55

## How To Build

###Configure Environment
Before the environment can be "bootstrapped" the environment needs a few variables. To do so run the configure.py script located at the root of the master repository. This script will prompt you for various environment configuration options and then automatically generate config.py from the template file config.py.in.

####Git Username & Password (Optional)
These will be used when checking out module repositories, if you wish to check out modules anonymously, leave these blank.

###Bootstrap Environment
To prep the environment for building, run bootstrap.py. This script will clone the appropriate module repositories (based on modules.py) into the "modules/" directory and generate
build files using cmake.

## Tips

### Creating A New Module
To create a new module run the python script "create-module.py" located in the utils directory. You will be prompted for the name of your new module and it's type (exe or lib). A new folder will be created in the modules directory and will be populated with the standard module directory structure, a README markdown file, and  CMakeLists.txt file to get you going.

If you want your module to be cloned automatically when bootstrapping the environment, add it to modules.py.in and reconfigure the environment.

####Module Directory Structure
All modules conform to the following directory structure:
doc/ - Module documentation, usually in markdown format.
dsrc/ - Module data files.
example/ - If this module is a library, use this directory for example/demo projects.
external/ - Dependencies for this specific module.
include/ - Public header files only.
src/ - Private header files and source files.
test/ - Unit tests.
