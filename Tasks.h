#ifndef __TASKS_H__
#define __TASKS_H__

#include "OsConfig.h"

typedef void (*TaskFunction)(void);
enum TaskState { READY, RUNNING, BLOCKED };

struct TaskControlBlock {
  uint8_t stackSize;
  uint8_t stackBase[TASK_STACK_SIZE];
  uint8_t* stackTop = stackBase + TASK_STACK_SIZE - 1u;
  uint8_t* stackPointer = NULL;
  uint8_t statusRegister = 0;
  TaskState taskstate = READY;
  TaskFunction function;
  uint16_t leastAvailableStack = TASK_STACK_SIZE;
  uint16_t blockedTime = 0;
};

static TaskControlBlock tasks[MAX_NUMBER_OF_TASKS];
static TaskControlBlock* currentTask = tasks;

bool CreateTask(TaskFunction function) {
  bool assigned = false;
  for (uint8_t i = 0; i < MAX_NUMBER_OF_TASKS; i++) {
    if (tasks[i].stackPointer == NULL) {
      tasks[i].stackPointer = tasks[i].stackTop;
      tasks[i].function = function;
      void* locationOfFunction = (void*)(tasks[i].function);
      *(tasks[i].stackPointer) =
          reinterpret_cast<uint16_t>(locationOfFunction) & 0xFFu;
      (tasks[i].stackPointer)--;
      *(tasks[i].stackPointer) =
          (reinterpret_cast<uint16_t>(locationOfFunction) & 0xFF00u) >> 8u;
      (tasks[i].stackPointer) -= 2u;
      *(tasks[i].stackPointer) = 0x80u;
      (tasks[i].stackPointer) -= 32;
      assigned = true;
      break;
    }
  }
  return assigned;
}

#endif  // __TASKS_H__