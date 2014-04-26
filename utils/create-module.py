import sys
import os
import errno

module_name = sys.argv[1]
module_filepath = str("modules/" + module_name)

# If the module already exists, bail.
if os.path.exists(module_filepath):
	print ('Module ' + module_name + ' already exists!')
	sys.exit()

print ('Creating Module - ' , module_name)

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
os.makedirs(module_filepath + "/include")
os.makedirs(module_filepath + "/example")
os.makedirs(module_filepath + "/test")

# Create stub .gitkeep files in order to ensure consistency across all modules.
open(module_filepath + "/doc/.gitkeep", 'a').close()
open(module_filepath + "/src/.gitkeep", 'a').close()
open(module_filepath + "/dsrc/.gitkeep", 'a').close()
open(module_filepath + "/external/.gitkeep", 'a').close()
open(module_filepath + "/include/.gitkeep", 'a').close()
open(module_filepath + "/example/.gitkeep", 'a').close()
open(module_filepath + "/test/.gitkeep", 'a').close()

cmakeFile = open(module_filepath + "/CMakeLists.txt", 'a')
cmakeFile.write("PROJECT(" + module_name + ")")
cmakeFile.close()

readmeFile = open(module_filepath + "/README.md", 'a')
readmeFile.write("Monsoon Game Engine: " + module_name + " Module\n========================")
readmeFile.close()

# Do initial commit.
os.chdir(module_filepath)
os.system("git add .")
os.system("git commit -m \"Initial commit.\"")
os.chdir("../../")

