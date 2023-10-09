#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <avr/interrupt.h>
#include <avr/io.h>

#include "Port.h"
#include "Tasks.h"

#define UPDATE_MIN_TASK_FREE(tcb) \
  tcb.leastAvailableStack = (tcb.stackPointer - tcb.stackBase);

#define STACK_POINTER ((volatile void*)SP)

#define INCREMENT_IN_LIST(BASE, ITEM, LENGTH) \
  ITEM = (BASE + (((uint16_t)(ITEM - BASE) + 1u) % LENGTH))

#define ASSIGN_SP_OF(TCB) SP = (uint16_t)(TCB->stackPointer)

uint8_t SystemBuffer[TOTAL_HEAP_SIZE];
TaskControlBlock* delayedTasks[MAX_NUMBER_OF_TASKS];

static uint32_t tickTimer = 0;

volatile void* defaultStack = nullptr;

__attribute__((naked)) void SwitchContext(void);
__attribute__((naked)) void SwitchContextFromVoid(void);
__attribute__((naked)) void StartScheduler(void);
__attribute__((naked)) void DelayTask(uint16_t delayTicks);

__attribute__((naked)) void SwitchContextFromVoid(void) {
  currentTask->taskstate = RUNNING;
  SAVE_CONTEXT;               // Save original context
  defaultStack = (void*)SP;   // save stack pointer for context switch
  ASSIGN_SP_OF(currentTask);  // Stack pointer points to task stack
  RESTORE_CONTEXT;            // Obtain predefined context
  asm volatile("ret");        // Go to first task
}

__attribute__((naked)) void SwitchContext(void) {
  SAVE_CONTEXT;  // Save task context

  currentTask->stackPointer =
      (uint8_t*)STACK_POINTER;  // Save task stack pointer
  SP = (uint16_t)defaultStack;  // Obtain original stack pointer
  RESTORE_CONTEXT;              // Get original context

  // Below calculate mimimal free stack
  register uint16_t currentFree =
      (currentTask->stackPointer - currentTask->stackBase);
  currentTask->leastAvailableStack =
      (currentFree < currentTask->leastAvailableStack)
          ? currentFree
          : currentTask->leastAvailableStack;

  // Place old task state to READY
  currentTask->taskstate = READY;

  // Find next free task
  do {
    INCREMENT_IN_LIST(tasks, currentTask, MAX_NUMBER_OF_TASKS);
  } while (currentTask->taskstate != READY);

  // Assign new task to be running
  currentTask->taskstate = RUNNING;
  // Store original stack
  SAVE_CONTEXT;
  // Store stack pointer to default stack
  defaultStack = (void*)SP;

  // Assign stack pointer to current task
  ASSIGN_SP_OF(currentTask);

  // Get context for current task
  RESTORE_CONTEXT;

  // Go to current task
  asm volatile("reti");
}

void DelayTask(uint16_t delayTicks) {
  currentTask->blockedTime = delayTicks;
  currentTask->taskstate = BLOCKED;
}

void DelayInit() {
  for (uint8_t i = 0; i < MAX_NUMBER_OF_TASKS; i++) {
    delayedTasks[i] = nullptr;
  }
}

void Timer_Init(void) {
  constexpr uint8_t CLK_DIV_1024 = 5u;
  TCNT1 = 0;
  OCR1A = TICK_REQ_100_KHZ;
  TIMSK1 = (1u << OCIE1A);
  TCCR1B = CLK_DIV_1024 | (1u << WGM12);
}

__attribute__((naked)) void StartScheduler(void) {
  sei();
  DelayInit();
  Timer_Init();
  SwitchContextFromVoid();
  // Code should never reach here
  while (true) {
    asm volatile("NOP");
  }
}

ISR(TIMER1_COMPA_vect, ISR_NAKED) {
  tickTimer++;
  SwitchContext();
}

#endif  // __SCHEDULER_H__