#include <rios/inode.h>
/*iget: disk =>memory*/
struct m_inode * iget(struct m_inode * inode, int nr)
{
/*inode nr counts from 0*/	
	u8 sector[512] = {0};int NR_inode_blk_start = NR_INODE_BLK(rios_superblock);
	int m = nr*sizeof(struct d_inode)/512;
	int _m = (nr+1)*sizeof(struct d_inode)/512;
	int n = nr*sizeof(struct d_inode)%512;
	int _n = (nr+1)*sizeof(struct d_inode)%512;
	if(m==_m){/*即将要取的inode不跨越两个扇区*/
		IDE_read_sector((void *)&sector,NR_inode_blk_start + m);
		memcpy(  inode,(void *) sector + n, sizeof(struct d_inode));
/*内容上，d_inode是m_inode的子集，这里把m_inode的前半段(即d_inode)抄送磁盘d_inode*/
		IDE_write_sector((void *)&sector,NR_inode_blk_start + m);
	}else{/*即将要取的inode将跨越两个扇区*/
/*copy 前半截 offset: n ~ 512*/		
		IDE_read_sector((void *)&sector,NR_inode_blk_start + m);
		memcpy( inode, (void *) (sector + n) ,512-n);
		IDE_write_sector((void *)&sector,NR_inode_blk_start + m);
/*copy 后半截 offset: 0 ~ _n*/	memset(&sector,0x00,512);/*清零*/
		IDE_read_sector((void *)&sector,NR_inode_blk_start + m + 1);
		memcpy( (void *)(inode + (512-n)), (void *)(sector + 0),_n);
		IDE_write_sector((void *)&sector,NR_inode_blk_start + m + 1);
	}
/*内容上，d_inode是m_inode的子集，这里把部分m_inode抄送d_inode*/	
	return inode;
}

/*iput : memory => disk*/
void iput(struct m_inode * inode, int nr)
{/*inode nr counts from 0*/	
	u8 sector[512] = {0};int NR_inode_blk_start = NR_INODE_BLK(rios_superblock);
	int m = nr*sizeof(struct d_inode)/512;
	int _m = (nr+1)*sizeof(struct d_inode)/512;
	int n = nr*sizeof(struct d_inode)%512;
	int _n = (nr+1)*sizeof(struct d_inode)%512;
	if(m==_m){/*即将要存的inode不跨越两个扇区*/
		IDE_read_sector((void *)&sector,NR_inode_blk_start + m );
		memcpy( (void *)(sector + n), inode , sizeof(struct d_inode));
/*内容上，d_inode是m_inode的子集，这里把m_inode的前半段(即d_inode)抄送磁盘d_inode*/
		IDE_write_sector((void *)&sector,NR_inode_blk_start + m);
	 }else{/*即将要存的inode将跨越两个扇区*/
/*copy 前半截 offset: n ~ 512*/		
		IDE_read_sector((void *)&sector,NR_inode_blk_start + m);
		memcpy( (void *)(sector + n), inode ,512-n);
		IDE_write_sector((void *)&sector,NR_inode_blk_start + m);
/*copy 后半截 offset: 0 ~ _n	memset(&sector,0x00,512);*//*清零*/
		IDE_read_sector((void *)&sector,NR_inode_blk_start + m + 1);
		memcpy( (void *)(sector + 0), inode + (512-n) ,_n);
		IDE_write_sector((void *)&sector,NR_inode_blk_start + m + 1);
	}
	return;	
}

void free_inode(int inode)
{
	u8 sector[512]={0};
	IDE_read_sector((void *)&sector,NR_INODE_MAP_BLK(rios_superblock));
	bitmap_clear_bit(inode,sector);
	IDE_write_sector((void *)&sector,NR_INODE_MAP_BLK(rios_superblock));
}

/*You can hexdump NR_INODE_MAP_BLK(rios_superblock),eg "hexdump 8" to see this zone*/
int new_inode()
{
	u8 sector[512]={0};
	int i = 0;rios_superblock.s_startsect = 1;
	IDE_read_sector((void *)&sector,NR_INODE_MAP_BLK(rios_superblock));
	for(i=0;i<512*8;i++){
		if(bitmap_test_bit(i,sector)){
			;
		}else{
			bitmap_set_bit(i,sector);
			IDE_write_sector((void *)&sector,NR_INODE_MAP_BLK(rios_superblock));
			return i;
		}
	}
	return i;
}