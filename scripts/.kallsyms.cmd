cmd_scripts/kallsyms := gcc -Wp,-MMD,scripts/.kallsyms.d -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer   -I/home/huqingwei/repositories/XOS-loongarch64/tools/include  -o scripts/kallsyms scripts/kallsyms.c  

source_scripts/kallsyms := scripts/kallsyms.c

deps_scripts/kallsyms := \
    $(wildcard include/config/page/offset.h) \

scripts/kallsyms: $(deps_scripts/kallsyms)

$(deps_scripts/kallsyms):
