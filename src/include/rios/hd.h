#ifndef _HD_H
#define _HD_H

#define ATA_PORT_DATA 0x1f0
#define ATA_PORT_ERROR 0x1f1
#define ATA_PORT_SECT_COUNT 0x1f2
#define ATA_PORT_LBA_LOW 0x1f3
#define ATA_PORT_LBA_MID 0x1f4
#define ATA_PORT_LBA_HIGH 0x1f5
#define ATA_PORT_STATUS 0x1f7
#define ATA_PORT_COMMAND 0x1f7
/**
 *When read, 0x1f7 => status;
 *When write, 0x1f7 => command
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void wait_ATA();
void init_hd();
void do_hd();
void judge_disk1_exist();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif