### RiOS doc

 Copyright (C) 2018 邱日

----------------------------------

##### Here are some notes in my kernel development.

### inline assembly usage in kernel development
Here is a mannual online of Inline assembly for x86 
 	https://www.ibm.com/developerworks/library/l-ia/index.html
and here is a comparision of GAS and NASM
  	https://www.ibm.com/developerworks/library/l-gas-nasm/index.html

- outsw :output word string to port 

### GDT:

![](gdt.png)

https://en.wikipedia.org/wiki/Global_Descriptor_Table



![](wiki_gdt.png)



![](os-dev.png)

##### Here are some materials about GDT from 《Writing a Simple Operating System —from Scratch》by Nick Blundell.

• Base: 0x0
• Limit: 0xfffff
• Present: 1, since segment is present in memory - used for virtual memory
• Privilige: 0, ring 0 is the highest privilige
• Descriptor type: 1 for code or data segment, 0 is used for traps
• Type:
– Code: 1 for code, since this is a code segment
– Conforming: 0, by not corming it means code in a segment with a lower
privilege may not call code in this segment - this a key to memory protection
– Readable: 1, 1 if readible, 0 if execute-only. Readible allows us to read
constants defined in the code.
– Accessed: 0 This is often used for debugging and virtual memory techniques,
since the CPU sets the bit when it accesses the segment
• Other flags

– Granularity: 1, if set, this multiplies our limit by 4 K (i.e. 16*16*16), so our
0xfffff would become 0xfffff000 (i.e. shift 3 hex digits to the left), allowing
our segment to span 4 Gb of memory
– 32-bit default: 1, since our segment will hold 32-bit code, otherwise we’d use
0 for 16-bit code. This actually sets the default data unit size for operations
(e.g. push 0x4 would expand to a 32-bit number ,etc.)
– 64-bit code segment: 0, unused on 32-bit processor
– AVL: 0, We can set this for our own uses (e.g. debugging) but we will not
use it

Since we are using a simple flat model, with two overlapping code and data segments,
the data segment will be identical but for the type flags:
• Code: 0 for data
• Expand down: 0 . This allows the segment to expand down - TODO
explain this
• Writable: 1. This allows the data segment to be written to, otherwise
it would be read only
• Accessed: 0 This is often used for debugging and virtual memory
techniques, since the CPU sets the bit when it accesses the segment



-----------------------------------------------------------------





### Interrupt Gate and Trap Gate:

https://www.gitbook.com/book/chyyuu/simple_os_book/details  《simple_os_book》P92  

![](interrupt_trap_gate.png)

Gate Descriptors　＝> 

-  1.interrupt-gate descriptpr
-  2.Trap-gate descriptor
-  3.Task-gate descriptor 
-  4.Call-gate descriptor

http://blog.csdn.net/gemini_star/article/details/4438280

VGA显卡内部有一系列寄存器可以用来控制显卡的状态。在标准的PC机上。 0x3d4和0x3d5两个端口可以用来读写显卡的内部寄存器。方法是先向0x3d4端口写入要访问的寄存器编号，再通过0x3d5端口来读写寄存器数据。存放光标位置的寄存器编号为14和15。两个寄存器合起来组成一个16位整数，这个整数就是光标的位置。比如0表示光标在第0行第0列，81表示第1行第1列（屏幕总共80列）。

-----------

### File System

This might be a joke, but Our RiOS kernel is plan to be POSIX compatible, for this reason, system calls should be carefully designed. Here are some system calls that I'm going to implement. You can see them in Andre S. Tanenbaum's book 《Operating Systems Design and Implementation,3rd Edition》P19 System calls of Minix.

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

#### Exception

| Exception # | Description                            | Error Code? |
| ----------- | -------------------------------------- | ----------- |
| 0           | Division By Zero Exception             | No          |
| 1           | Debug Exception                        | No          |
| 2           | Non Maskable Interrupt Exception       | No          |
| 3           | Breakpoint Exception                   | No          |
| 4           | Into Detected Overflow Exception       | No          |
| 5           | Out of Bounds Exception                | No          |
| 6           | Invalid Opcode Exception               | No          |
| 7           | No Coprocessor Exception               | No          |
| 8           | Double Fault Exception                 | Yes         |
| 9           | Coprocessor Segment Overrun Exception  | No          |
| 10          | Bad TSS Exception                      | Yes         |
| 11          | Segment Not Present Exception          | Yes         |
| 12          | Stack Fault Exception                  | Yes         |
| 13          | General Protection Fault Exception     | Yes         |
| 14          | Page Fault Exception                   | Yes         |
| 15          | Unknown Interrupt Exception            | No          |
| 16          | Coprocessor Fault Exception            | No          |
| 17          | Alignment Check Exception (486+)       | No          |
| 18          | Machine Check Exception (Pentium/586+) | No          |
| 19 to 31    | Reserved Exceptions                    |             |

