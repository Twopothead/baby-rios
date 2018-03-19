#ifndef _STRING_H
#define _STRING_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include <rios/ramdisk.h>
#include <rios/fs.h>
#include <rios/console.h>	
char * cuthead(char * path,char * name);
char * eatcmd(char * command,char * name);
/*delimiter: '/' or ' '*/
const char *get_basename(const char *path,int delimiter);
/* these 4 string library functions comes from Linux2.4/lib/string.c*/
unsigned int strspn(const char *s, const char *accept);
char * strpbrk(const char * cs,const char * ct);
char * strtok(char * s,const char * ct);
char * strrchr(const char * s, int c);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif