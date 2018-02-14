## Guide for RiOS
------------------------------------
### Copyright (C) 2017 qiuri


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
- 
--------------------------
$ cd RiOS/ 
$ make run
then you'll see "[ ok ] RiOS " in qemu 
$ make clean 
remove build files
 
