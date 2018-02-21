/*
 * RiOS/include/rios/gdt_idt.h
 * 
 * Copyright (C) 2018 Frank Curie (邱日)
 */
#ifndef _GDT_IDT_H
#define _GDT_IDT_H


#include <rios/type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


void set_get_entry(u8 num, u32 base, u32 limit, u8 access, u8 granularity);
extern void update_gdt();
void init_gdt();



#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif