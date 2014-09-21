
debug=ARGUMENTS.get("debug",0)
if int(debug):
    SConscript("src/SConscript",variant_dir="Debug", duplicate=0)
else:
    SConscript("src/SConscript",variant_dir="Release", duplicate=0)

install_systemd_script=Install("/etc/systemd/system","heinz.service")
install_config=InstallAs("/etc/heinz/heinz.conf","config/example.conf")
install_config2=InstallAs("/etc/heinz/http.conf","config/http.conf")
Alias("install",[install_systemd_script,install_config,install_config2])
