cmd_scripts/mod/empty.o := ../cross-tools/bin/loongarch64-unknown-linux-gnu-gcc -Wp,-MMD,scripts/mod/.empty.o.d  -nostdinc -isystem /home/huqingwei/repositories/cross-tools/bin/../lib/gcc/loongarch64-unknown-linux-gnu/13.0.0/include -I/home/huqingwei/repositories/XOS-loongarch64/arch/loongarch/include -Iinclude  -Iarch/loongarch/include/generated -Iarch/loongarch/include/generated/uapi -include include/generated/autoconf.h -Iarch/loongarch/include/generated/ -Iinclude/asm-generic/ -Iinclude/generated/ -Iusr/include/asm-generic/ -Iinclude/usr/asm-generic/ -Iinclude -Iadapter/klibc/include/ -I/home/huqingwei/repositories/XOS-loongarch64/arch/loongarch/include/uapi -Iarch/loongarch/include/generated/uapi -I/home/huqingwei/repositories/XOS-loongarch64/arch/loongarch/include/uapi -I/home/huqingwei/repositories/XOS-loongarch64/include/uapi -Iinclude/generated/uapi -Iadapter/klibc/include/klibc/uapi -include /home/huqingwei/repositories/XOS-loongarch64/include/linux/kconfig.h         -D__KERNEL__ -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Wno-format-security -fno-delete-null-pointer-checks -fno-tree-scev-cprop -Wmissing-declarations -Wmissing-prototypes -O0 -fno-pic -nostdinc -I./arch/loongarch/include -I./include -I./arch/loongarch/include/uapi -I./include/uapi -Wall -Wundef -Werror=strict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -fshort-wchar -fno-PIE -Werror=implicit-function-declaration -Werror=implicit-int -Werror=return-type -Wno-format-security -funsigned-char -std=gnu11 -mabi=lp64s -G0 -pipe -msoft-float -mexplicit-relocs -ffreestanding -mno-check-zero-division -fno-asynchronous-unwind-tables -fno-delete-null-pointer-checks -Wno-frame-address -Wno-format-truncation -Wno-format-overflow -Wno-address-of-packed-member -O2 -fno-allow-store-data-races -Wframe-larger-than=2048 -fstack-protector-strong -Wno-main -Wno-unused-but-set-variable -Wno-unused-const-variable -Wno-dangling-pointer -fomit-frame-pointer -ftrivial-auto-var-init=zero -fno-stack-clash-protection -Wdeclaration-after-statement -Wvla -Wno-pointer-sign -Wcast-function-type -Wno-stringop-truncation -Wno-stringop-overflow -Wno-restrict -Wno-maybe-uninitialized -Wno-alloc-size-larger-than -Wimplicit-fallthrough=5 -fno-strict-overflow -fno-stack-check -fconserve-stack -Werror=date-time -Werror=incompatible-pointer-types -Werror=designated-init -Wno-packed-not-aligned -fpie -Os -DDISABLE_BRANCH_PROFILING -ffreestanding -fno-stack-protector -mdirect-extern-access -fno-stack-protector -fomit-frame-pointer -g -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack  -fno-tree-scev-cprop  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(empty)"  -D"KBUILD_MODNAME=KBUILD_STR(empty)" -c -o scripts/mod/empty.o scripts/mod/empty.c

source_scripts/mod/empty.o := scripts/mod/empty.c

deps_scripts/mod/empty.o := \
  /home/huqingwei/repositories/XOS-loongarch64/include/linux/kconfig.h \
    $(wildcard include/config/h.h) \
    $(wildcard include/config/.h) \
    $(wildcard include/config/booger.h) \
    $(wildcard include/config/foo.h) \

scripts/mod/empty.o: $(deps_scripts/mod/empty.o)

$(deps_scripts/mod/empty.o):
