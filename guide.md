## Guide for RiOS
------------------------------------
### Copyright (C) 2018 qiuri


install to your Udisk
- Prerequisit

```bash 
sudo apt-get install grub qemu xorriso gcc-multilib 
```
grub => bootloader, qemu => virtual machine, xorriso => creates ISO images


- install GRUB2 to your U disk

```bash
df -h # your Udisk may be /dev/sdb
sudo mount /dev/sdb /mnt # mount Udisk to /mnt
mkdir /mnt /boot
sudo grub-install --force --root-directory=/mnt /dev/sdb 
# TAKE CAUTION! DANGERAOUS! make sure you know what you are doing!
# installation finished. No error reported. 
# Now GRUB2 has been installed to your U disk successfully.
```

- Coding

```
cd RiOS 
mkdir -p build/iso/boot/grub
mkdir -p build/arch/i386
mkdir src;cd src;touch grub.cfg linker.ld multiboot.asm boot.asm kmain.c;cd -
touch Makefile;tree
```

- ISO for GRUB

tree

```
build/

--iso
  |__ boot
	|__ grub
	|    |__grub.cfg
	|____kernel.bin
```
--------------------------
You may run into such case:
```
gcc -fno-stack-protector -m32 -fno-builtin -fno-omit-frame-pointer src/kmain.c -o build/arch/i386/kmain.o
/usr/lib/gcc/x86_64-linux-gnu/7/../../../i386-linux-gnu/Scrt1.o: In function `_start':
(.text+0x28): undefined reference to `main'
```
Therefore,gcc Compile with -c OPTION is necessary.

--------------------------
Difference between NASM and GAS
https://www.ibm.com/developerworks/linux/library/l-gas-nasm/index.html
--------------------------

helpful resources :
https://wiki.osdev.org/What_order_should_I_make_things_in
--------------------------
```bash
$ cd RiOS/ 
$ make run
## then you'll see "[ ok ] RiOS " in qemu 
$ make clean 
## remove build files
``` 

