
debug=ARGUMENTS.get("debug",0)
raspi=ARGUMENTS.get("raspi",0)
if int(raspi):
    SConscript("src/SConscript",variant_dir="Release", duplicate=0)
if int(debug):
    SConscript("src/SConscript",variant_dir="Debug", duplicate=0)
else:
    SConscript("src/SConscript",variant_dir="Release", duplicate=0)
