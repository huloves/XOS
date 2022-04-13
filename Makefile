# target macros
TARGET := kernel# FILL: target filename
MAIN_SRC := # FILL: src file which contains `main()`

# compile macros
DIRS := boot # FILL: only the dirs which contain the src files, in this case, `src` should be added
OBJS := # FILL: only the objects in current dir, and do not include the one contains `main()`

# intermedia compile macros
# NOTE: ALL_OBJS are intentionally left blank, no need to fill it
ALL_OBJS :=

DIST_CLEAN_FILES := $(OBJS)

# recursive wildcard
rwildcard=$(foreach d,$(wildcard $(addsuffix *,$(1))),$(call rwildcard,$(d)/,$(2))$(filter $(subst *,%,$(2)),$(d)))

# default target
default: show-info all

# non-phony targets
$(TARGET): build-subdirs
	@echo -e "\t" CC $(CCFLAGS) $(ALL_OBJS) $(MAIN_SRC) -o $@

# phony targets
.PHONY: all
all: $(TARGET) find-all-objs build/kernel.bin
	@echo Target $(TARGET) build finished.
	cp ./build/kernel.bin ./hdisk/boot/kernel.bin
	sync
	sleep 1

build/kernel.bin: $(ALL_OBJS)
	@echo ${ALL_OBJS}
	#nasm -f elf ./boot/grub_head.S -o ./build/grub_head.o
	${LD} ${LDFLAGS} $(ALL_OBJS) -o $@

.PHONY: clean
clean:
	cd ./build && rm -f ./*

.PHONY: show-info
show-info:
	@echo Building Project

# need to be placed at the end of the file
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
export PROJECT_PATH := $(patsubst %/,%,$(dir $(mkfile_path)))
export MAKE_INCLUDE=$(PROJECT_PATH)/config/make.global
export SUB_MAKE_INCLUDE=$(PROJECT_PATH)/config/submake.global
include $(MAKE_INCLUDE)
