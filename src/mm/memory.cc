/*
 *  RiOS/src/mm/memory.c
 *
 *  Copyright (C) 2017  Curie （邱日）
 */
/*void init_mem_table(struct Mem_table *Mtable);
 *为整合好后的内存管理模块初始化函数，我把它放在initialize()函数里来调用
 *这个函数用来初始化用于内存管理的全局结构体指针RiOSmm
 */
#include <rios/memory.h>
#include <rios/console.h>
Mem_table * RiOSmm=(struct Mem_table *)0x3c0000;/*用于内存管理的全局结构体指针，在initialize里初始化*/
void init_zero_mem_table(Mem_table *Mtable)
{
                Mtable->mem_record_total=0;
                return;
}//~

int mem_record_free(Mem_table *Mtable,unsigned int upointer,unsigned int psize)
{
                int i,j;
                for(i=0;i<Mtable->mem_record_total;i++){
                        if(Mtable->mem_record[i].mem_addr>upointer){
                                break;
                        }
                }/*此时mem_record[i]<upointer<mem_record[i+1]*/
                if(i>0){
/*@1 准备和前面的空间合并，前面当然不能没有空间*/
                        if(Mtable->mem_record[i-1].mem_addr+Mtable->mem_record[i-1].mem_size==upointer){
/*现在送过来的这个指针要free掉，而它前面恰好紧挨着一块已经free的内存，
 *那只需要把现在的这个指针所指内存并入这前面的记录即可，size变大
 *但不需要添加新的记录
 */
                                Mtable->mem_record[i-1].mem_size+=psize;
                                if(i<Mtable->mem_record_total){/*企图和后面的free内存合并*/
                                        if(psize+upointer==Mtable->mem_record[i].mem_addr){
/*如果真的可以合并，也就是说此时
 *前面一块free中间是我准备free的pointer,后面又紧挨着一块free内存
 *等于说这三块是连在一块的，那我就要删除一条记录，但把size弄成三个的合体、
 */
                                                Mtable->mem_record[i-1].mem_size+=Mtable->mem_record[i].mem_size;
                                                Mtable->mem_record_total--;/*删去一条记录*/
                                                for(;i<Mtable->mem_record_total;i++){/*后面整个往前挪一格*/
                                                        Mtable->mem_record[i]=Mtable->mem_record[i+1];
                                                }
                                        }
                                }
                        return 0;/*free成功*/
                        }
                }//~@1

                if((i<Mtable->mem_record_total)&&((upointer+psize)==Mtable->mem_record[i].mem_addr)){
/*@2 若不能和前面的free内存合并
 *但可以和后面的free内存合并*/
                        Mtable->mem_record[i].mem_addr=upointer;
                        Mtable->mem_record[i].mem_size+=psize;
                        return 0;/*free成功*/
                        }//～@2

                #define MAX_MEM_RECORDS 4090
                if(Mtable->mem_record_total<MAX_MEM_RECORDS){
/*||前面的free_i||    ||待free的p所指内存||     ||后面的free_i+1||*/
/*@3 若不能和前面的free内存合并
 *也不能和后面的free内存合并*/
                        for(j=Mtable->mem_record_total;j>i;j--){
/*均向后挪一格*/
                                Mtable->mem_record[j]=Mtable->mem_record[j-1];
                        }
                        Mtable->mem_record_total++;/*来增加我这新的要free的p的记录(变成第i+1条记录)*/
                        Mtable->mem_record[i].mem_addr=upointer;
                        Mtable->mem_record[i].mem_size=psize;
                        return 0;
                }//～@3
/*分配失败*/
                return -1;
}//~

void init_mem_table(Mem_table *Mtable)
{
                init_zero_mem_table(Mtable);
/*注意在第一次使用Ri_malloc之前先要对内存管理的部分init一下，
 *而且 NOTE!! 要把不是系统已经用了的地方给free掉
 *要free，不然全是0，没有可用空间
 */
                int memtotal;
/*前面的0x400000留给我的系统，当然，这是个随便定的值
 *只要别把我的代码区也给malloc了就行
 */
/*memtotal=getmemorysize_bit(0x400000,0xffffffff);
 *本来应该用上面这句话来检测内存大小，但是经过实体机测试，这句话太耗时了
 *那我就来个硬编码，规定memtotal是4G
 */
                memtotal=0xffffffff;
                mem_record_free(Mtable,0x400000,memtotal-0x400000);
/*print "total memory: "; putdec(memtotal>>20);print("MB")*/
                return;
}

void * Ri_malloc(unsigned int size)
{
/*注意此处的RiOSmm是个struct Mem_table *类型的全局结构体指针变量
 *整个系统只有这一个,专门用来内存管理
 */
                return mem_record_alloc(RiOSmm,size);
}

/*int Ri_free(unsigned int upointer,int size){*/
int Ri_free(void* upointer,int size)
{
/*注意此处的RiOSmm是个struct Mem_table *类型的全局结构体指针变量
 *整个系统只有这一个,专门用来内存管理
 */
                return mem_record_free(RiOSmm,(unsigned int)upointer,size);
}

/*int mem_record_alloc(Mem_table *Mtable,unsigned int psize){*/
void* mem_record_alloc(Mem_table *Mtable,unsigned int psize)
{
/*unsigned int i,getp;*/
                unsigned int i;void * getp;/*其实就是unsigned int getp;*/
                for(i=0;i<Mtable->mem_record_total;i++){
                        if(Mtable->mem_record[i].mem_size>=psize){
/*若找到一块大小足够的可用内存*/
                                getp=(void*)Mtable->mem_record[i].mem_addr;
                                Mtable->mem_record[i].mem_addr+=psize;
                                Mtable->mem_record[i].mem_size-=psize;
                                if(Mtable->mem_record[i].mem_size==0){
/*若这块内存恰好分完，没有剩余量，就删去这条记录*/
                                        Mtable->mem_record_total--;
/*删去这条记录以后，后面的记录都要往前喏一格*/
                                        for(;i<Mtable->mem_record_total;i++){
                                                Mtable->mem_record[i]=Mtable->mem_record[i+1];
                                        }
                                }
                                return getp;
                        }
                }
                _panic(" FBI WARNING:run out of memory!!!");
                return 0;/*run out of memory*/
}


int getmemorysize_bit()
{
                unsigned int start_addr=0x666666;//内存检测是从此地址往后检测
/*因为害怕把代码那一块内存给乱改，于是首地址给的比较大，默认start_addr前面的内容是已经使用的内存，不检测
 *如果要检测的时候给电脑或者qemu分配的内存比较小，可能就要把start_addr改小一点了
 *反正只要保证它在我的代码外，但又不超过内存最大地址就行
 */
                unsigned int end_addr=0xffffffff;/*最大能检测4G*/
                unsigned int  i;unsigned before;
                unsigned int  word=0x66886688;
                unsigned int  dorw=0x88668866;
                volatile unsigned int *p;//与硬件相关，提示编译器不要对volatile限定符修饰的对象进行编译器优化
                for(i=start_addr;i<end_addr;i+=0x1000){
                        p=(unsigned int *)i+0x200;
                        before=*p;
                        *p=word;
                        if(*p!=word){
                                *p=before;break;
                                // goto ENDofMEMORY;
                        }//ENDofMEMORY:
                        *p=before;
/*这里得到的i是bit数，若要返回MB(2^20B)的话要右移20位*/
                }
                return i;
}//~

int getmemorysize_MB()
{               int bits=getmemorysize_bit();/*这里得到的i是bit数，若要返回MB(2^20B)的话要右移20位*/
                return (bits>>20);
}//~

void init_mm()
{
        msg_mm_ok1();
        init_mem_table(RiOSmm);
        /*int mm_available=getmemorysize_MB();
         *由于这句话过于耗时　我就暂时硬编码了，最后应该恢复过来
         */
        int mm_available = 666;
        msg_mm_ok2(mm_available);
}

