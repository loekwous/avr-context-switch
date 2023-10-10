#include "Scheduler.h"

#include "ContextSwitchMacros.h"
#include "OsConfig.h"
#include "OsHeap.h"
#include "Tasks.h"

#define UPDATE_MIN_TASK_FREE(tcb) \
  tcb.leastAvailableStack = (tcb.stackPointer - tcb.stackBase);

#define STACK_POINTER ((volatile void*)SP)

#define INCREMENT_IN_LIST(BASE, ITEM, LENGTH) \
  ITEM = (BASE + (((uint16_t)(ITEM - BASE) + 1u) % LENGTH))

#define ASSIGN_SP_OF(TCB) SP = (uint16_t)(TCB->stackPointer)

namespace {

#if USE_WATERMARK_HOOK == 1
extern void WaterMarkHook();
#endif

TaskControlBlock tasks[MAX_NUMBER_OF_TASKS];
TaskControlBlock* currentTask = tasks;
TaskControlBlock* delayedTasks[MAX_NUMBER_OF_TASKS];

bool tickIsUpdated = false;
volatile void* defaultStack = nullptr;

static void UpdateTimeTasks() {
  if (tickIsUpdated) {
    tickIsUpdated = false;
    for (uint8_t i = 0; i < MAX_NUMBER_OF_TASKS; i++) {
      if (delayedTasks[i] != nullptr) {
        if (delayedTasks[i]->blockedTime > 0) {
          delayedTasks[i]->blockedTime--;
        } else {
          delayedTasks[i]->taskstate = READY;
          delayedTasks[i] = nullptr;
        }
      }
    }
  }
}

void AddTaskToDelayList(TaskControlBlock* task) {
  for (uint8_t i = 0; i < MAX_NUMBER_OF_TASKS; i++) {
    if (delayedTasks[i] == nullptr) {
      delayedTasks[i] = task;
      break;
    }
  }
}

void DelayInit() {
  for (uint8_t i = 0; i < MAX_NUMBER_OF_TASKS; i++) {
    delayedTasks[i] = nullptr;
  }
}

void Timer_Init(void) {
  constexpr uint8_t CLK_DIV_64 = 3u;
  TCNT1 = 0;
  OCR1A = TICK_FREQ_KHZ;
  TIMSK1 = (1u << OCIE1A);
  TCCR1B = CLK_DIV_64 | (1u << WGM12);
}

void IdleTask() {
  while (true) {
    asm volatile("NOP");
  }
}

}  // namespace

OS_NAKED void OS::SwitchContextFromVoid(void) {
  currentTask->taskstate = RUNNING;
  SAVE_CONTEXT;               // Save original context
  defaultStack = (void*)SP;   // save stack pointer for context switch
  ASSIGN_SP_OF(currentTask);  // Stack pointer points to task stack
  RESTORE_CONTEXT;            // Obtain predefined context
  asm volatile("ret");        // Go to first task
}

OS_NAKED void OS::SwitchContext(void) {
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

#if USE_WATERMARK_HOOK == 1
  if (currentTask->leastAvailableStack <= WATERMARK_THRESHOLD) {
    WaterMarkHook();
  }
#endif

  UpdateTimeTasks();

  // Place old task state to READY
  if (currentTask->taskstate == RUNNING) {
    currentTask->taskstate = READY;
  }

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

bool OS::CreateTask(TaskFunction function) {
  bool assigned = false;
  for (uint8_t i = 0; i < MAX_NUMBER_OF_TASKS; i++) {
    if (tasks[i].stackPointer == NULL) {
      tasks[i].stackPointer = tasks[i].stackTop;
      tasks[i].function = function;
      tasks[i].taskstate = READY;
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

void OS::DelayTask(uint16_t delayTicks) {
  currentTask->blockedTime = delayTicks;
  currentTask->taskstate = BLOCKED;
  AddTaskToDelayList(currentTask);
  SwitchContext();
}

ISR(TIMER1_COMPA_vect, ISR_NAKED) {
  tickIsUpdated = true;
  OS::SwitchContext();
  asm volatile("reti");
}

void OS::StartScheduler(void) {
  CreateTask(IdleTask);
  sei();
  DelayInit();
  Timer_Init();
  SwitchContextFromVoid();
  // Code should never reach here
  while (true) {
    asm volatile("NOP");
  }
}
