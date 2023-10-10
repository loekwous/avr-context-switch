#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

#include "OsConfig.h"
#include "Tasks.h"

#define OS_NAKED __attribute__((naked))

namespace OS {

OS_NAKED void SwitchContext(void);
OS_NAKED void SwitchContextFromVoid(void);
void StartScheduler(void);
void DelayTask(uint16_t delayTicks);

}  // namespace OS

#endif  // __SCHEDULER_H__