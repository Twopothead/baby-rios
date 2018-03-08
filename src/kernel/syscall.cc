#include <rios/syscall.h>
#include <rios/trap.h>
#include <rios/serial.h>

void do_syscall(struct TrapFrame *trapframe)
{
	if(trapframe->eax == _SYS_READ){
		print("begin");

		//print(" syscall read");
		nextline();
		print("wait...");
		unsigned char *bbb=(unsigned char *)0x0;
		
		for(int i=0;i<100;i++){
			*(bbb)=7;
			//putnum(*(bbb));
			bbb++;
			
		}
	
		nextline();
		bbb=(unsigned char *)0x0;
		for(int i=0;i<3;i++)
			IDE_write_sector((void *)(bbb+512*i),i+1);
		print("magic");

	bbb=(unsigned char *)0x0;
		for(int i=0;i<100;i++){
			*(bbb)=2;
			//putnum(*(bbb));
			bbb++;
			
		}

		bbb=(unsigned char *)0x0;
		for(int i=0;i<3;i++)
			IDE_read_sector((void *)(bbb+512*i),i+1);
		for(int i=0;i<100;i++){
			putnum(*(bbb+i));
		}
	}
	if(trapframe->eax == _SYS_WRITE)
		print(" syscall write");
}

int _syscall(u32 _eax, u32 _ebx, u32 _ecx, u32 _edx)
{
	int _ret;
	__asm__ volatile("int $0x80": "=a"(_ret):"a"(_eax),\
		"b"(_ebx),"c"(_ecx),"d"(_edx));
	return _ret;
}
