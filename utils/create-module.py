import sys
import os
import errno
import shutil

module_name = sys.argv[1]
module_filepath = str("modules/" + module_name)

# If the module already exists, bail.
if os.path.exists(module_filepath):
	print ('Module ' + module_name + ' already exists!')
	sys.exit()

print ('Creating Module - ' , module_name)

type = input("Type? [lib, exe, dynlib]: ")

os.makedirs(module_filepath)

# Initialize a new git repo.
os.chdir(module_filepath)
os.system("git init")
os.chdir("../../")

# Create Standard Directory Structure
os.makedirs(module_filepath + "/doc")
os.makedirs(module_filepath + "/src")
os.makedirs(module_filepath + "/dsrc")
os.makedirs(module_filepath + "/external")
os.makedirs(module_filepath + "/include/" + module_name)
os.makedirs(module_filepath + "/example")
os.makedirs(module_filepath + "/test")

# Create stub .gitkeep files in order to ensure consistency across all modules.
open(module_filepath + "/doc/.gitkeep", 'a').close()
open(module_filepath + "/src/.gitkeep", 'a').close()
open(module_filepath + "/dsrc/.gitkeep", 'a').close()
open(module_filepath + "/external/.gitkeep", 'a').close()
open(module_filepath + "/include/" + module_name + "/.gitkeep", 'a').close()
open(module_filepath + "/example/.gitkeep", 'a').close()
open(module_filepath + "/test/.gitkeep", 'a').close()

cmakeFile = open(module_filepath + "/CMakeLists.txt", 'a')
cmakeFile.write("CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)\n")
cmakeFile.write("PROJECT(" + module_name + " C CXX)\n\n")

cmakeFile.write("INCLUDE_DIRECTORIES(include/"+ module_name + ")\n")
cmakeFile.write("INCLUDE(Sources.cmake)\n")

if type == "lib":
	cmakeFile.write("ADD_LIBRARY(" + module_name + " STATIC ${MODULE_SOURCES})\n")
	cmakeFile.write("SET_TARGET_PROPERTIES(" + module_name + " PROPERTIES LINKER_LANGUAGE CXX)\n")
elif type == "exe":
	cmakeFile.write("ADD_EXECUTABLE(" + module_name + " ${MODULE_SOURCES})\n");
elif type == "dynlib":
	cmakeFile.write("ADD_DEFINITIONS(-DMONSOON_DYNLIB_EXPORT)\n")
	cmakeFile.write("ADD_LIBRARY(" + module_name + " SHARED ${MODULE_SOURCES})\n")
	cmakeFile.write("SET_TARGET_PROPERTIES(" + module_name + " PROPERTIES LINKER_LANGUAGE CXX)\n")

cmakeFile.close()

shutil.copyfile("Sources.cmake.in", module_filepath + "/Sources.cmake")

readmeFile = open(module_filepath + "/README.md", 'a')
readmeFile.write("#Monsoon Game Engine\n##Module: " + module_name + "\n")
readmeFile.close()

# Do initial commit.
os.chdir(module_filepath)
os.system("git add .")
os.system("git commit -m \"Initial commit.\"")
os.chdir("../../")

