import SCons.Script as scons
import os

PACKAGE_TMP_DIR="packages"

def make_debian(packagename, files, env):
	package_file="{}/{}-{}-{}.deb".format(PACKAGE_TMP_DIR,packagename, env["VERSION_STRING"], env["TARGET_ARCHITECTURE"])
	package_folder=scons.Dir("{}/{}".format(PACKAGE_TMP_DIR,packagename))

#	controlfile=env.Textfile(target="{}/DEBIAN/control".format(package_folder), source=[File("{}/DEBIAN/control.template".format(package_folder)),"Version: $VERSION_STRING\nArchitecture: $TARGET_ARCHITECTURE"],TEXTFILESUFFIX="")
	
	#controlfile=env.Command("{}/DEBIAN/control".format(package_folder),"{}/DEBIAN/control.template".format(package_folder),
	controlfile=env.Command("{}/DEBIAN/control".format(package_folder),"{}/{}.template".format(PACKAGE_TMP_DIR,packagename),
			[
				scons.Copy("$TARGET","$SOURCE"),
				"echo 'Version: {}' >> $TARGET".format(env["VERSION_STRING"]),
				"echo 'Architecture: {}' >> $TARGET".format(env["TARGET_ARCHITECTURE"])
			])
	package_files=[env.Command("{}{}".format(package_folder,f[1]),f[0],scons.Copy("$TARGET","$SOURCE")) for f in files]
	package_dependencies=[package_folder]+package_files+[controlfile
			,scons.Glob("{}/DEBIAN/*".format(package_folder))
			]
	package_file_node=env.Command(package_file, package_dependencies, "dpkg-deb --build $SOURCE $TARGET".format(package_folder,package_file))
	return package_file_node
