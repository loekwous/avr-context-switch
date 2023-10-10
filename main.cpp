
#include <avr/io.h>
#include <stdint.h>

#include "Scheduler.h"
#include "Tasks.h"

void IncrementPortC(void) { PORTC ^= (1u << PC0); }

void IncrementPortB() { PORTB ^= (1u << PB0); }

void FirstTask() {
  volatile uint32_t value = 1;
  while (true) {
    IncrementPortC();
    OS::DelayTask(100u);
  }
}

void SecondTask() {
  while (true) {
    IncrementPortB();
    volatile uint32_t x = 0;
    volatile uint32_t z = 3;
    volatile uint32_t pb = 4;
    OS::DelayTask(500u);
  }
}

int main() {
  DDRB |= (1 << PB0);
  DDRC |= (1 << PC0);
  PORTB = 0;
  PORTC = 0;

  OS::CreateTask(FirstTask);
  OS::CreateTask(SecondTask);
  OS::StartScheduler();

  // Code should never reach here
  while (true) {
    asm volatile("nop");
  }
}