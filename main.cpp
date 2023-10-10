
#include <avr/io.h>
#include <etl/array.h>
#include <stdint.h>

#include "Os.h"

void IncrementPortC(void) { PORTC ^= (1u << PC0); }

void IncrementPortB() { PORTB ^= (1u << PB0); }

void FirstTask() {
  volatile uint32_t value = 1;
  while (true) {
    IncrementPortC();
    DelayTask(100u);
  }
}

void SecondTask() {
  while (true) {
    IncrementPortB();
    volatile uint32_t x = 0;
    volatile uint32_t z = 3;
    volatile uint32_t pb = 4;
    DelayTask(500u);
  }
}

int main() {
  DDRB |= (1 << PB0);
  DDRC |= (1 << PC0);
  PORTB = 0;
  PORTC = 0;

  volatile size_t ramEnd = RAMEND;
  volatile size_t heapStart = (size_t)__malloc_heap_start;
  volatile size_t heapEnd = (size_t)__malloc_heap_end;
  volatile size_t brkval = (size_t)(__malloc_margin);

  CreateTask(FirstTask);
  CreateTask(SecondTask);
  StartScheduler();

  // Code should never reach here
  while (true) {
    asm volatile("nop");
  }
}