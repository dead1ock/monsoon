import os

# Configure Bootstrap Environment
import configure

# Import Module Repository List and Configuration Options
import modules
import config

# Check Out Modules
if not os.path.exists("modules"):
	os.makedirs("modules")
os.chdir("modules")

for i, repository in enumerate(modules.repositories):
	if not os.path.exists("modules/" + repository[0]): # Only check out missing repositories.
		if(len(config.git_user)): # If we have a username to use, check out using authentication
			os.system("git clone " + repository[1].replace("https://", "https://" + config.git_user + ":" + config.git_pass + "@") + " " + repository[0])
			os.system("git checkout " + repository[2])
		else: # Try to checkout anonymously
			os.system("git clone " + repository[1] + " " + repository[0])
			os.system("git checkout " + repository[2])
	

os.chdir("../")

# Generate Build
os.makedirs("build")
os.chdir("build")
os.system("cmake ..")

os.chdir("../")