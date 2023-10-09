message("Using AVR")

# Define the toolchain
SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR avr)

# Specify the cross compiler
SET(TOOLCHAIN_PREFIX "avr-")
SET(CMAKE_C_COMPILER "${TOOLCHAIN_PREFIX}gcc")
SET(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}g++")
SET(CMAKE_ASM_COMPILER "${TOOLCHAIN_PREFIX}gcc")
SET(CMAKE_OBJCOPY "${TOOLCHAIN_PREFIX}objcopy")
SET(CMAKE_SIZE "${TOOLCHAIN_PREFIX}size")
set(STM32_STARTUP_SCRIPT "${CMAKE_SOURCE_DIR}/startup.s")

set(COMPILER_OPTIONS "-mmcu=atmega328p -Os --max-errors=5")

# Set the compiler flags
set(CMAKE_ASM_FLAGS "${COMPILER_OPTIONS} -c -x assembler-with-cpp")

set(CMAKE_C_FLAGS "${COMPILER_OPTIONS} -O0 -g -ffunction-sections -fdata-sections -fshort-enums")
# set(CMAKE_C_FLAGS_DEBUG "-g -Og")
# set(CMAKE_C_FLAGS_RELEASE "-Os")

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} --std=c++17")
# set(CMAKE_CXX_FLAGS_DEBUG "-g -Og")
# set(CMAKE_CXX_FLAGS_RELEASE "-Os")

# Set linker flags
#SET(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/STM32F103C6TX_FLASH.ld")
#SET(CMAKE_EXE_LINKER_FLAGS "${COMPILER_OPTIONS} -Wl,--print-memory-usage -T${LINKER_SCRIPT} -Wl,-Map=${CMAKE_BINARY_DIR}/${PROJECT_NAME}.map -Wl,--gc-sections -static -Wl,--start-group -lc -lm -Wl,--end-group")
SET(CMAKE_EXE_LINKER_FLAGS "${COMPILER_OPTIONS} -Wl,--print-memory-usage -Wl,-Map=${CMAKE_BINARY_DIR}/${PROJECT_NAME}.map -Wl,--gc-sections -static -Wl,--start-group -lc -lm -Wl,--end-group")

# Define the target
SET(CMAKE_EXECUTABLE_SUFFIX ".hex")

add_compile_definitions(ETL_NO_STL=1)
add_compile_definitions(ETL_NO_64BIT_TYPES=1)
add_compile_definitions(F_CPU=16000000)
add_compile_definitions(ARDUINO)


include_directories(SYSTEM ${STM32_SYSTEM_DIRECTORIES} ${STM32_FREERTOS_SYSTEM_DIRECTORIES})
