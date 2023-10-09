#ifndef __OSCONFIG_H__
#define __OSCONFIG_H__

#define TOTAL_HEAP_SIZE (512u)
#define TASK_STACK_SIZE (128u)
#define MAX_NUMBER_OF_TASKS (5u)
#define TICK_REQ_1_KHZ (15625u)
#define TICK_REQ_10_KHZ (1562u)
#define TICK_REQ_100_KHZ (156u)

#define TICK_FREQ_KHZ TICK_REQ_10_KHZs

#endif  // __OSCONFIG_H__