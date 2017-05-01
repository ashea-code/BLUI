from distutils import spawn
from subprocess import call
import platform, urllib2, os, shutil, sys, argparse, glob, fileinput

CEF_VERSION = "2623"
CEF_BUILD_SCRIPT_URL = "https://bitbucket.org/chromiumembedded/cef/raw/master/tools/automate/automate-git.py"
BLUI_PATCH = [
	["./cefbuild/cef/libcef/common/main_delegate.cc", "./browser/cef3_macosx_framework_pathBLUI.patch"],
	["./cefbuild/cef/libcef/resources/framework-Info.plist", "./browser/framework-id-osx.patch"],
	["./cefbuild/cef/include/cef_base.h", "./browser/cef_base.patch"],
	["./cefbuild/cef/include/base/cef_thread_collision_warner.h", "./browser/cef_thread_collision_warner.patch"]
]

parser = argparse.ArgumentParser()
parser.add_argument('-m', '--msvc')
args = parser.parse_args()

class BLUIBuilder():
	def __init__(self):
		self.pre_reqs = []
		# Non platform specific things
		self.git_commands = ["git clone https://github.com/AaronShea/BLUI.git blui", 
				"git clone https://github.com/AaronShea/BluBrowser.git browser"]

	def init(self):
		if self.get_os() == "Windows":
			self.pre_reqs = ["git", "msbuild", "cmake", "python", "patch"]

	def check_pre_reqs(self):
		print "Checking for: " + str(self.pre_reqs)
		for cmd in self.pre_reqs:
			print "Check for " + cmd
			path = spawn.find_executable(cmd)
			if path == None:
				raise Exception("Prereqs were not found! \nCould not find command: %s" % cmd)
		print "All good!"

	def clone_blu(self):
		for gitcmd in self.git_commands:
			call(gitcmd)

	def download_cef_script(self):
		response = urllib2.urlopen(CEF_BUILD_SCRIPT_URL)
		script = response.read()
		f = open("cefbuild.py",'w')
		f.write(script)
		f.close()

	def fetchCEFBuild(self):
		print "We're about to fetch and build Chromium and CEF!"
		print "This is going to take a while, please don't kill"
		print "the process until it's done."

		if self.get_os() == "Windows":
			if args.msvc is not None:
				os.environ["GYP_MSVS_VERSION"] = args.msvc
			else:
				vs_version = str(input("Please input your Visual Studio Version (2012, 2013 etc): "))
				os.environ["GYP_MSVS_VERSION"] = vs_version
			os.environ["DEPOT_TOOLS_WIN_TOOLCHAIN"] = "0"
		call("python cefbuild.py --download-dir=./cefbuild --branch=%s  --no-build --force-update" % CEF_VERSION)

	def patch_files(self):
		print "Applying patches..."
		for patchpair in BLUI_PATCH:
				cmd = "patch --verbose %s < %s" % (patchpair[0], patchpair[1])
				os.system(cmd)

	def buildCEF(self):
		print "-- Building CEF and Chromium. This will take a while! --"
		os.environ["GYP_DEFINES"] = "target_arch=x64"
		call("python cefbuild.py --download-dir=./cefbuild --branch=%s --no-update --force-build --x64-build" % CEF_VERSION)

	def build_browser(self):
		if self.get_os() == "Windows":
			gen = "Visual Studio 14 2015 Win64"
		# Copy in new cefsimple from blubrowser
		rmpath = glob.glob('./cefbuild/chromium/src/cef/binary_distrib/cef_binary_*64/cefsimple')[0]
		shutil.rmtree(rmpath)
		shutil.copytree("./browser/BluBrowser", rmpath)
		call("cmake " + rmpath + "/.. " + "-G\"" + gen + "\"")

		######## Start Windows Build Commands ########
		if self.get_os() == "Windows":
			# Build step 1 - Debug and release

			call("msbuild ALL_BUILD.vcxproj /p:Configuration=Debug")
			call("msbuild ALL_BUILD.vcxproj /p:Configuration=Release")

			# Modify the libcef wrapper project file
			for line in fileinput.input("libcef_dll/libcef_dll_wrapper.vcxproj", inplace=True):
				if line.strip() == "<RuntimeLibrary>MultiThreaded</RuntimeLibrary>":
					line = "<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>"
				if line.strip() == "<RuntimeLibrary>MultiThreadedDebug</RuntimeLibrary>":
					line = "<RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>"
				print "%s" % line,

			# We can now rebuild the wrapper lib as dynamic (see above changes)
			call("msbuild libcef_dll/libcef_dll_wrapper.vcxproj /p:Configuration=Debug")
			call("msbuild libcef_dll/libcef_dll_wrapper.vcxproj /p:Configuration=Release")

			# Copy debug and release libs over
			debugPath = glob.glob("./cefbuild/chromium/src/cef/binary_distrib/cef_binary_*64/Debug/*.lib")
			releasePath = glob.glob("./cefbuild/chromium/src/cef/binary_distrib/cef_binary_*64/Release/*.lib")
			for lib in debugPath:
				shutil.copy(lib, "./libcef_dll/Debug")
			for lib in releasePath:
				shutil.copy(lib, "./libcef_dll/Release")
                        self.packWindows()

        def packWindows(self):
                print "Packing up the plugin..."
                packLibReleasePath = "./libcef_dll/Release/"
                packLibDebugPath = "./libcef_dll/Debug/"
                packShipping = "./cefsimple/Release/"
                packInclude = "./cefbuild/cef/include/"

                # Copy libs
                shutil.copytree(packLibReleasePath, "./blui/ThirdParty/cef/Win/lib")
                shutil.copytree(packLibDebugPath, "./blui/ThirdParty/cef/Win/lib/Debug")

                # Copy shipping files
                shutil.copytree(packShipping, "./blui/ThirdParty/cef/Win/shipping")

                # Copy include files
                shutil.copytree(packInclude, "./blui/ThirdParty/cef/Win/include")

                print "=== ALL DONE! BLUI has been packaged for Windows! [in ./blui]"
        ######## End Windows Build Commands ########     
                

	def get_os(self):
		return platform.system()

builder = BLUIBuilder()

builder.init()
builder.check_pre_reqs()
builder.clone_blu()
builder.download_cef_script()
builder.fetchCEFBuild()
builder.patch_files()
builder.buildCEF()
builder.build_browser()
