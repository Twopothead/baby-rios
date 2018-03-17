#ifndef _HEXDUMP_H
#define _HEXDUMP_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include <rios/type.h>
#include <rios/hd.h>
#include <rios/console.h>
void sector_hexdump(u8 *sector);
void nr_sector_hexdump(int nr);

#ifdef __cplusplus
}
#endif

#endif