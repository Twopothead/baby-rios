## Guide for RiOS
------------------------------------
### Copyright (C) 2018 qiuri(邱日)


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

- C and C++ code can call each other by taking advantage of **extern "C"**,etc.
- C++ file extension is  ".CC",and  "CC" can be recognized by GNU compiler.

--------------------------

helpful resources :
- OSdev: https://wiki.osdev.org/What_order_should_I_make_things_in
- About the configuration of grub.cfg : http://www.jinbuguo.com/linux/grub.cfg.html

--------------------------
```bash
$ cd RiOS/ 
$ make run
## then you'll see "[ ok ] RiOS " in qemu 
$ make clean 
## remove build files
```

------------------------------

### about Programmable Interrupt Timer(8253/8254)

The PIT chip uses the following I/O ports:

```
I/O port     Usage
0x40         Channel 0 data port (read/write)
0x41         Channel 1 data port (read/write)
0x42         Channel 2 data port (read/write)
0x43         Mode/Command register (write only, a read is ignored)
```

The Mode/Command register at I/O address 0x43 contains the following:

```
Bits         Usage
 6 and 7      Select channel :
                 0 0 = Channel 0
                 0 1 = Channel 1
                 1 0 = Channel 2
                 1 1 = Read-back command (8254 only)
 4 and 5      Access mode :
                 0 0 = Latch count value command
                 0 1 = Access mode: lobyte only
                 1 0 = Access mode: hibyte only
                 1 1 = Access mode: lobyte/hibyte
 1 to 3       Operating mode :
                 0 0 0 = Mode 0 (interrupt on terminal count)
                 0 0 1 = Mode 1 (hardware re-triggerable one-shot)
                 0 1 0 = Mode 2 (rate generator)
                 0 1 1 = Mode 3 (square wave generator)
                 1 0 0 = Mode 4 (software triggered strobe)
                 1 0 1 = Mode 5 (hardware triggered strobe)
                 1 1 0 = Mode 2 (rate generator, same as 010b)
                 1 1 1 = Mode 3 (square wave generator, same as 011b)
 0            BCD/Binary mode: 0 = 16-bit binary, 1 = four-digit BCD
```

https://wiki.osdev.org/Programmable_Interval_Timer

bin(0x34)='0b110100'= 00   11    0 10   0

outb(0x43,0x34) =>

- Channel 0 	(00)  bits 6 and 7 

- Access mode: lobyte/hibyte    (11)  bits 4 and 5

-  0 1 0 = Mode 2 (rate generator)   (010) bits  1 to 3

- 16-bit binary  (0) bit 0

  ​

------------------------

### File System

This might be a joke, but our RiOS kernel is plan to be POSIX compatible, for this reason, system calls should be carefully designed. Here are some system calls that I'm going to implement. You can see them in Andre S. Tanenbaum's book 《Operating Systems Design and Implementation,3rd Edition》P19 System calls of Minix.

---------------------------------------

#### file management

```c
fd = creat(name, mode);
fd = mknod(name, mode ,addr);
fd = open(file, how,...);
s = close(fd);
n = read(fd, buffer, nbytes);
n = write(fd, buffer, nbytes);
pos = lseek(fd, offset, whence);
s = stat(name, &buf);
s = fstat(name, &buf);
fd = dup(fd);
s = pipe(&fd[0])
s = ioctl(fd, request, argp);
s = access(name, amode);
s = rename(old, new);
s = fcntl(fd, cmd,...);

```

 

-------------------

#### directory and file system management

```c++
s = mkdir(name, mode);
s = rmdir(name);
s = link(name1,name2);
s = unlink(name);
s = mount(special, name, flag);
s = unmount(special);
s = sync();
s = chdir(dirname);
s = chroot(dirname);
s = chmod(name,mod);
uid = getuid();
gid = getgid();
s = setgid(uid);
s = setgid(gid);
s = chown(name, owner, group);
oldmask  = umask(complmode);  
```

