import shutil
import os
	
if os.path.exists("build"):
	shutil.rmtree("build")

if os.path.exists("__pycache__"):
	shutil.rmtree("__pycache__")

if os.path.isfile("config.py"):	
	os.remove("config.py")
	
if os.path.isfile("modules.py"):
	os.remove("modules.py")
