#ifndef _DPT_H
#define _DPT_H

#include <rios/type.h>
#include <rios/console.h>
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*Disk Partition Table*/
void setup_DPT();
struct DiskPartitionTable{
	u8 boot_ind;
	u8 head;
	u8 sector;
	u8 cyl;
	u8 sys_ind;
	u8 end_head;
	u8 end_sector;
	u8 end_cyl;
	u32 start_sector; /*count from zero*/
	u32 num_sectors;/*num of sectors that partition takes up*/
};

#define NORMAL 0x2e
void sys_setup();

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif