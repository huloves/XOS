cmd_scripts/kconfig/lxdialog/inputbox.o := gcc -Wp,-MMD,scripts/kconfig/lxdialog/.inputbox.o.d -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer   -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600 -DCURSES_LOC="<ncurses.h>" -DNCURSES_WIDECHAR=1 -DLOCALE   -c -o scripts/kconfig/lxdialog/inputbox.o scripts/kconfig/lxdialog/inputbox.c

source_scripts/kconfig/lxdialog/inputbox.o := scripts/kconfig/lxdialog/inputbox.c

deps_scripts/kconfig/lxdialog/inputbox.o := \
  scripts/kconfig/lxdialog/dialog.h \

scripts/kconfig/lxdialog/inputbox.o: $(deps_scripts/kconfig/lxdialog/inputbox.o)

$(deps_scripts/kconfig/lxdialog/inputbox.o):
