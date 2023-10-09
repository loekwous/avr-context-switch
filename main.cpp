
#include <avr/io.h>
#include <etl/array.h>
#include <stdint.h>

#include "Os.h"

void IncrementPortC(void) { PORTC += 1; }

void IncrementPortB(uint32_t value) {
  PORTB += static_cast<uint8_t>(value & 0xFF);
}

void FirstTask() {
  volatile uint32_t value = 1;
  while (true) {
    asm("NOP");
    asm("NOP");
    IncrementPortB(value);
    // SwitchContext();
    asm("NOP");
    asm("NOP");
    asm("NOP");
  }
}

void SecondTask() {
  while (true) {
    asm("NOP");
    asm("NOP");
    asm("NOP");
    IncrementPortC();
    // SwitchContext();
    asm("NOP");
    asm("NOP");
    asm("NOP");
  }
}

void ThirdTask() {
  while (true) {
    asm("NOP");
    asm("NOP");
    asm("NOP");
    IncrementPortC();
    // SwitchContext();
    asm("NOP");
    asm("NOP");
    asm("NOP");
  }
}

int main() {
  // Timer_Init();
  PORTB = 0;
  PORTC = 0;
  sei();
  CreateTask(FirstTask);
  CreateTask(SecondTask);
  CreateTask(ThirdTask);
  StartScheduler();

  // Code should never reach here
  while (true) {
    asm volatile("nop");
  }
}