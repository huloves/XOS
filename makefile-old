BUILD_DIR = ./build
ENTRY_POINT = 0xc0001500
AS = nasm
CC = gcc
LD = ld
LIB = -I include/
ASFLAGS = -f elf

CFLAGS1 = -Wall $(LIB) -E -fno-builtin  \
         -m32 -fno-stack-protector -nostdinc -fno-pic -gdwarf-2

CFLAGS = -Wall $(LIB) -c -fno-builtin  \
         -m32 -fno-stack-protector -nostdinc -fno-pic -gdwarf-2

LDFLAGS =-m elf_i386 -T script/kernel.ld -Map $(BUILD_DIR)/kernel.map -nostdlib

