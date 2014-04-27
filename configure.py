import os
import errno
import shutil
import io
import sys

# If we don't have an existing modules.py to import
# copy from the .in
if not os.path.isfile("modules.py"):
	shutil.copy("modules.py.in", "modules.py")

# Generate config.py if it doesn't existing
if not os.path.isfile("config.py"):

	# Prompt for each configuration option
	git_user = input("Git Username: ")
	git_pass = input("Git Password: ")
	
	# Generate
	config = io.open('config.py', 'w')

	for line in io.open('config.py.in', 'r'):
		line = line.replace('$git_user', git_user)
		line = line.replace('$git_pass', git_pass)
		config.write(line)

	config.close()