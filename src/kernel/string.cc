#include <rios/string.h>
/* cuthead("/usr/bin",name) => name:usr*/
char * cuthead(char * path,char * name){
	char *s;
	int len;
	while(* path == '/') path ++;
	if(* path == 0) return 0;
	s = path;
	while((* path!='/') && (*path !=0)) path ++;
	len = path - s;
	if(len >= MAX_FULL_PATH_LEN)
		memmove(name,s,MAX_FULL_PATH_LEN);
	else{
		memmove(name,s,len);
		name[len] = 0;
	}
	while(* path == '/') path++;
	return path;/*return path whose head has been cut.*/
}

/* 'eatcmd' works just like 'cuthead',
 * the difference is its delimiter is not '/' but ' ',
 * as 'cuthead' is used in 'namei' to parse directory path,
 * and 'eatcmd' is used in command interpreter.  
 */
char * eatcmd(char * command,char * name){
	char *s;
	int len;
	while(* command == ' ') command ++;
	if(* command == 0) return 0;
	s = command;
	while((* command!=' ') && (*command !=0)) command ++;
	len = command - s;
	if(len >= MAX_CMD_LEN)
		memmove(name,s,MAX_CMD_LEN);
	else{
		memmove(name,s,len);
		name[len] = 0;
	}
	while(* command == ' ') command++;
	return command;/*return cmd whose head has been cut.*/
}
/*
cmd = "hello world cafe babe"
kprintf("%s\n",eatcmd(tmp,name));				hello world cafe babe
kprintf("%s\n",eatcmd(eatcmd(tmp,name),name));			world cafe babe
kprintf("%s\n",eatcmd(eatcmd(eatcmd(tmp,name),name),name));	cafe babe
*/

/*Here,"strspn","strpbrk","strtok" these 3 functions are from Linux 2.4 kernel/lib/string.c*/
char * ___strtok;
#ifndef __HAVE_ARCH_STRSPN
unsigned int strspn(const char *s, const char *accept)
{
	const char *p;
	const char *a;
	unsigned int  count = 0;

	for (p = s; *p != '\0'; ++p) {
		for (a = accept; *a != '\0'; ++a) {
			if (*p == *a)
				break;
		}
		if (*a == '\0')
			return count;
		++count;
	}

	return count;
}
#endif

#ifndef __HAVE_ARCH_STRPBRK
char * strpbrk(const char * cs,const char * ct)
{
	const char *sc1,*sc2;

	for( sc1 = cs; *sc1 != '\0'; ++sc1) {
		for( sc2 = ct; *sc2 != '\0'; ++sc2) {
			if (*sc1 == *sc2)
				return (char *) sc1;
		}
	}
	return (char *)NULL;
}
#endif

#ifndef __HAVE_ARCH_STRTOK
char * strtok(char * s,const char * ct)
{
	char *sbegin, *send;

	sbegin  = s ? s : ___strtok;
	if (!sbegin) {
		return (char *)NULL;
	}
	sbegin += strspn(sbegin,ct);
	if (*sbegin == '\0') {
		___strtok = (char *)NULL;
		return( (char *)NULL );
	}
	send = strpbrk( sbegin, ct);
	if (send && *send != '\0')
		*send++ = '\0';
	___strtok = send;
	return (sbegin);
}
#endif