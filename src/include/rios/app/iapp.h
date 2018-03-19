#ifndef _IAPP_H
#define _IAPP_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include <rios/console.h>	
void msg_info_superblock();
void info_service(char* cmd_buffer);
void ls_service(char* cmd_buffer);
void mkdir_service(char* cmd_buffer,int cmd_buffer_index);
void cd_service(char* cmd_buffer,int cmd_buffer_index);
void pwd_service();
int get_dir(char * partname);
#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif