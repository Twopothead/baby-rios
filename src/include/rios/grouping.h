#ifndef _GROUPING_H
#define _GROUPING_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include <asm/x86.h>
#include <rios/dpt.h>
#include <rios/fs.h>
#include <rios/hd.h>
#include <rios/ramdisk.h>
#include <rios/grouping.h>
#include <rios/syscall.h>
#include <rios/fs.h>
#include <rios/console.h>
#include <rios/ramdisk.h>
#include <asm/x86.h>
#include <rios/format.h>

/* here, nr_group counts from 1 */	

/*　Free space management :grouping (空闲块成组链接)　*/
#define BLKS_PER_GROUP 		64				/*每组64块*/
#define TOTAL_GROUP 		128				/*一共128组*/
#define SECTOR_PER_BLOCK 	2				/*每个块２个扇区,1KB*/

extern union Super_Block_Sect rios_superblock;
union free_space_grouping_head {/*成组链接法，各组空闲块的头*/
	u16 bytes[512] = {0};/*占坑位　2 sectors*/
	struct {
		u16 s_free;
		u16 s_free_blk_nr[BLKS_PER_GROUP];/*[64]*/
	};
/*s_free_blk_nr[0] is next free group's nr*/
};
void visit_all_free_blks();
void init_free_space_grouping();
/*group NR range from 1 to 128, group's block nr range from 1 to 8192 */
union free_space_grouping_head get_blk_nr_free_group(int nr_blk);
void set_blk_nr_free_group(union free_space_grouping_head g_head,int nr_blk);

union Super_Block_Sect * get_super();
void set_super();

void free_blk_traverse();

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif