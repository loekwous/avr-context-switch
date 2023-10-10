#ifndef __OSHEAP_H__
#define __OSHEAP_H__

#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>

namespace OS {

void* Malloc(size_t size);

size_t SpaceLeft(void);

}  // namespace OS

#endif  // __OSHEAP_H__