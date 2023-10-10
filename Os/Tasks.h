#ifndef __TASKS_H__
#define __TASKS_H__

#include <stdint.h>
#include <stdlib.h>

#include "OsConfig.h"
#include "avr/io.h"

typedef void (*TaskFunction)(void);
enum TaskState { READY, RUNNING, BLOCKED };

struct TaskControlBlock {
  uint8_t stackSize;
  uint8_t stackBase[TASK_STACK_SIZE];
  uint8_t* stackTop = stackBase + TASK_STACK_SIZE - 1u;
  uint8_t* stackPointer = NULL;
  uint8_t statusRegister = 0;
  TaskState taskstate = BLOCKED;
  TaskFunction function;
  uint16_t leastAvailableStack = TASK_STACK_SIZE;
  uint16_t blockedTime = 0;
};

namespace OS {
bool CreateTask(TaskFunction function);
}  // namespace OS

#endif  // __TASKS_H__