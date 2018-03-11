#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <rios/type.h>
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include <rios/console.h>



void do_syscall(struct TrapFrame *trapframe);
int _syscall(u32 _eax, u32 _ebx, u32 _ecx, u32 _edx);


#define _POSIX_VERSION_   20180308
/*
 * This might be a joke, but Our RiOS kernel is
 * plan to be POSIX compatible, for this reason,
 * system calls should be well designed. 
 * Here are some system calls that I'm going to implement.
 * You can see them in Andrew S. Tanenbaum's book 《Operating 
 * Systems Design and Implementation,3rd Edition》P19 System calls of Minix.
 */
/*DIR. & File System Mgt. syscall*/
#define _SYS_READ 	300
#define _SYS_WRITE 	301

#define _SYS_TESTHD 	400

#define _SYS_MKDIR 	302
#define _SYS_RMDIR	303
#define _SYS_LINK 	304
#define _SYS_UNLINK	305
#define _SYS_MOUNT 	306
#define _SYS_UNMOUNT	307
#define _SYS_SYNC 	308
#define _SYS_CHDIR 	309
#define _SYS_CHROOT 	310

/*Protection syscall*/
#define _SYS_CHMOD	311
#define _SYS_GETUID	312
#define _SYS_GETGID	313
#define _SYS_SETUID	314
#define _SYS_SETGID	315
#define _SYS_CHOWN	316
#define _SYS_UNMASK	317

/*File management syscall*/
#define _SYS_CREAT	318
#define _SYS_MKNOD	319
#define _SYS_OPEN	320
#define _SYS_CLOSE	321
#define _SYS_LSEEK	322
#define _SYS_STAT 	323
#define _SYS_FSTAT	324
#define _SYS_DUP	325
#define _SYS_PIPE	326
#define _SYS_IOCTL	327
#define _SYS_ACCESS	328
#define _SYS_RENAME	329
#define _SYS_FCNTL	330

/*Time management syscall*/
#define _SYS_TIME 	331
#define _SYS_STIME	332
#define _SYS_UTIME	333
#define _SYS_TIMES	334

/*Process management syscall*/
#define _SYS_FORK	335
#define _SYS_WAIRPID	336
#define _SYS_WAIT	337
#define _SYS_EXECVE	338
#define _SYS_EXIT	339 
#define _SYS_BRK	340
#define _SYS_GETPID	341
#define _SYS_GETGPRP	342
#define _SYS_SETSID	343
#define _SYS_PTRACE	344

/*Signals syscall*/
#define _SYS_KILL	345
#define _SYS_ALARM	346
#define _SYS_PAUSE	347

#define _RIOS_READBUF   401
#define _RIOS_WRITEBUF  402

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif