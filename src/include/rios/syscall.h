#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <rios/type.h>
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include <rios/console.h>

#define _SYS_READ 300
#define _SYS_WRITE 301


void do_syscall(struct TrapFrame *trapframe);
int _syscall(u32 _eax, u32 _ebx, u32 _ecx, u32 _edx);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif