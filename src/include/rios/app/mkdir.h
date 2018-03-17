#ifndef _MKDIR_H
#define _MMKDIR_H
#ifdef __cplusplus
#include <rios/type.h>
extern "C" {
#endif /* __cplusplus */
#include <rios/console.h>	
#include <rios/fs.h>
#include <rios/bitmap.h>
void mkdir(const char *name,u8 mode);	

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif