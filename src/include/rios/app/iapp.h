#ifndef _IAPP_H
#define _IAPP_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include <rios/console.h>
#include <rios/app/mkdir.h>
#include <rios/grouping.h>
#include <rios/bitmap.h>
#include <rios/string.h>
#define get_path_basename(p)  get_basename(p,'/')
#define get_cmd_basename(p)   get_basename(p,' ')	
void info_service(char* cmd_buffer);
void ls_service(char* cmd_buffer);
int mkdir_service(char* cmd_buffer,int cmd_buffer_index);
void cd_service(char* cmd_buffer,int cmd_buffer_index);
void pwd_service();
int get_dir(char * partname);
void rmdir_service(char* cmd_buffer,int cmd_buffer_index);
/* rmdir.cc */
void rmdir(const char *name,u8 mode);
/* touch.cc */
void touch(const char *name,u8 mode);

int silent_mkdir(char* cmd_buffer,int cmd_buffer_index);
void silent_cd(char* cmd_buffer,int cmd_buffer_index);

void touch_service(char* cmd_buffer,int cmd_buffer_index);
void cat_service(char* cmd_buffer,int cmd_buffer_index);
void cat(const char *name,u8 mode);
void slow_cat(const char *name,u8 mode);
void slow_cat_service(char* cmd_buffer,int cmd_buffer_index);

void rm_service(char* cmd_buffer,int cmd_buffer_index);
void rm(const char *name,u8 mode);
#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif