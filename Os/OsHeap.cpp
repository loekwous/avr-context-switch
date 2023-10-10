
#include "OsHeap.h"

#include "OsConfig.h"

namespace {
uint8_t systemHeapMemory[TOTAL_HEAP_SIZE];
uint8_t* systemHeapLocation = systemHeapMemory;
constexpr uint8_t* systemHeapEnd = systemHeapMemory + TOTAL_HEAP_SIZE - 1u;

}  // namespace

void* OS::Malloc(size_t size) {
  void* allocatedMemory = nullptr;
  if (SpaceLeft() >= size) {
    void* allocatedMemory = static_cast<void*>(systemHeapLocation);
    systemHeapLocation += size;
  }
  return allocatedMemory;
}

size_t OS::SpaceLeft(void) { return (systemHeapEnd - systemHeapLocation); }
