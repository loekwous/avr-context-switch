#ifndef __OSCONFIG_H__
#define __OSCONFIG_H__

#define TOTAL_HEAP_SIZE (1024u)
#define TASK_STACK_SIZE (128u)
#define MAX_NUMBER_OF_TASKS (4u)

#define WATERMARK_THRESHOLD (20u)
#define USE_WATERMARK_HOOK (0u)

#define TICK_FREQ_1_KHZ (249u)

#define TICK_FREQ_KHZ TICK_FREQ_1_KHZ

#endif  // __OSCONFIG_H__