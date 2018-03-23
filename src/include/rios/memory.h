/*
 * RiOS/src/include/memory.h
 * 
 * Copyright (C) 2018 Frank Curie (邱日)
 */
#ifndef _MEMORY_H
#define _MEMORY_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct Mem_record
{
    unsigned int mem_addr;
    unsigned int mem_size;
}Mem_record;

typedef struct Mem_table
{
    unsigned int mem_record_total;
    Mem_record mem_record[4090];
}Mem_table;


void init_mm();

void init_zero_mem_table( Mem_table *Mtable);
int mem_record_free( Mem_table *Mtable,unsigned int upointer,unsigned psize);
void init_mem_table(Mem_table *Mtable);
void* Ri_malloc(unsigned int size);
/*int Ri_free(unsigned int upointer,int size);*/
int Ri_free(void* upointer,int size);
/*int mem_record_alloc( Mem_table *Mtable,unsigned int psize);*/
void* mem_record_alloc( Mem_table *Mtable,unsigned int psize);
int getmemorysize_bit();
int getmemorysize_MB();
/**usage : 
 * Ri_free((void *)p,sizeof(p)),p=NULL;
 * p=(QueuePtr)Ri_malloc(sizeof(QNode));
 */
#define kfree(My_p) Ri_free((void *)My_p,sizeof(My_p)),My_p=NULL
#define kmalloc(sizeof_Mytype) (void *)Ri_malloc(sizeof_Mytype)
#define kfree_size(My_p,size) Ri_free((void *)My_p,size),My_p=NULL
#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif