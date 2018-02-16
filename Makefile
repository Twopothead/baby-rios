#  "RiOS" -- a Naive kernel  
# 
# RiOS/makefile
# Copyright (C) 2018 qiuri

# Compiler settings and options
CC = gcc
LD = ld 
CFLAGS = -fno-stack-protector -m32 \
	 -c \
	 -fno-builtin -fno-omit-frame-pointer \
	 -fno-pic  -fno-exceptions \
	 -ffreestanding \
	 -I./src/include \
	 -Wall \
	 -fno-rtti 
	#-nostdinc 

# Platform i386
arch ?= i386

# Files
iso := build/RiOS-$(arch).iso
kernel :=build/kernel-$(arch).bin
linker_script := src/linker.ld
grub_cfg :=src/grub.cfg

# c_source :=$(wildcard src/*.c)  
# Alternatively, c_source =$(shell find src/ -name "*.c")
# asm_source := $(wildcard src/*.asm)
include GNUMakefile 
# add another file here

asm_obj := $(patsubst src/%.asm,\
	build/arch/$(arch)/%.o, $(wildcard src/*.asm))
cc_obj := $(patsubst src/%.cc,\
	build/arch/$(arch)/%.o, $(wildcard src/*.cc)) $(console_objs) \
	$(mm_objs) \
	$(kernel_objs)

gas_obj := $(patsubst src/kernel/gas/%.S,\
	build/arch/$(arch)/kernel/gas/%.o, $(wildcard src/kernel/gas/*.S))

objs := asm_obj cc_obj gas_obj

	
.PHONY: clean run iso help

run : $(iso)
	qemu-system-x86_64 -cdrom $(iso) -monitor stdio

iso : $(iso)

$(iso): $(kernel) $(grub_cfg)
	mkdir -p build/iso/boot/grub
	cp $(kernel) build/iso/boot/kernel.bin
	cp $(grub_cfg) build/iso/boot/grub
	grub-mkrescue -o $(iso) build/iso/
	

$(kernel):  $(asm_obj) $(gas_obj) $(cc_obj) $(linker_script) 
	$(LD) -n -m elf_i386 -T $(linker_script) -o $(kernel)  $(asm_obj) $(gas_obj) $(cc_obj)


# Compile .asm files with nasm
build/arch/$(arch)/%.o : src/%.asm
	mkdir -p $(shell dirname $@)
	nasm -f elf32 $< -o $@  
	
# Compile .cc files (cplusplus)
build/arch/$(arch)/%.o: src/%.cc
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS)  $< -o $@


# Compile .S files with gcc
build/arch/$(arch)/kernel/gas/%.o :src/kernel/gas/%.S
	mkdir -p $(shell dirname $@)
	$(CC) -c -m32 $< -o $@


	
clean :
	rm -r build

help :
	@echo "Usage: make [OPTION...]"
	@echo "$ make run"
	@echo "$ make clean" 
	@echo "Please make sure that xorriso,qemu have be correctly installed."
	@echo "Report bugs to 3103204417@qq.com"
	
# 1.wildcard :扩展通配符
# 2.patsubst :替换通配符
# eg. wildcard *.c => get all c files
# patsubst %c,%o,$(wildcard *.c) =>.c to .o,get all obj files
# ?= if not set,set it
# .PHONY 定义一些伪目标
# grub-mkrescue相当于把那个文件夹打包成iso
# GNU make手册：http://www.gnu.org/software/make/manual/make.html
# Compile with "-fno-stack-protector" ，otherwise :(
#	 "undefined reference to `__stack_chk_fail'"
# ld -n ... "-n" Option:pass "-nmagic" flag to the linker,
# 	-n disables the automatic section alignment in the executebale 
#	,otherwise GRUB2 may fail to find Multiboot
# Our kernel is for i386,which is x86-32. However,thanks to x86's backward
# compatibility , RiOS will be OK with x86-64 architecture.
# for more about $<,$@ ,please refer to
#    https://www.gnu.org/software/make/manual/html_node/Pattern-Rules.html#Pattern-Rules
# gcc with -c OPTION,otherwise "undefined reference to 'main' " :(
#
# -I./src/include \ :add search PATH,this depends on the location of Makefile
# #include <blabla.h>
# The option -ffreestanding directs the compiler to not assume that standard 
#    functions have their usual definition.
# target: ***build/RiOS-i386.iso*** => write to U disk
# 	qemu-system-x86_64 -kernel kernel-i386.bin
# 	qemu-system-x86_64 RiOS-i386.iso
# 
#-nostdinc -Iinclude 不从系统中的库去找，而从自己的地方去找 
