#ifndef _RAMDISK_H
#define _RAMDISK_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* bootimage and rootimage
 *since code in mm/memory.cc 
 *allocate memory from addr 0x666666
 *our ram disk will start from addr 0x555555
 */
#define ramdisk_mm_start_addr 0x555555
#define ROOTDEV ramdisk_mm_start_addr

void init_ramdisk();

inline void * memcpy(void * dest, const void * src, int num)
{
	__asm__ ("cld; rep; movsb"::"c"(num),"S"((int)src),"D"((int)dest));
	return dest;
}

inline void * memmove(void * dest,const void * src, int num )
{

	if(dest<src){
		__asm__("cld;rep;movsb"::"c"(num),"S"((int)src),"D"((int)dest));

	}else{
		__asm__("std;rep;movsb"::"c"(num),"S"((int)src+num-1),"D"((int)dest+num-1));
	}
	return dest;
}

inline void * memset(void *str ,char ch, int count )
{	
	__asm__("cld;rep stosb"::"a"(ch),"D"(str),"c"(count));
	return str;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif