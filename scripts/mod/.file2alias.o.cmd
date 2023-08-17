cmd_scripts/mod/file2alias.o := gcc -Wp,-MMD,scripts/mod/.file2alias.o.d -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer     -c -o scripts/mod/file2alias.o scripts/mod/file2alias.c

source_scripts/mod/file2alias.o := scripts/mod/file2alias.c

deps_scripts/mod/file2alias.o := \
  scripts/mod/modpost.h \
  scripts/mod/elfconfig.h \
  scripts/mod/devicetable-offsets.h \
  scripts/mod/../../include/linux/mod_devicetable.h \

scripts/mod/file2alias.o: $(deps_scripts/mod/file2alias.o)

$(deps_scripts/mod/file2alias.o):
