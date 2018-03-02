#ifndef _i8253_H
#define _i8253_H



#define LATCH 11930 
/* Programmable Interval Timer (PIT) 8253/8254*/
/* 8253 port address */
#define PIT_CHANNEL0_DATA_PORT 0x40 
#define PIT_CHANNEL1_DATA_PORT 0x41
#define PIT_CHANNEL2_DATA_PORT 0x42
#define PIT_COMMAND_REG 0x43

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void init_8253();


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
