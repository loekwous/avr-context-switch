message("Using AVR")

# Define the toolchain
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)

# Specify the cross compiler
set(TOOLCHAIN_PREFIX "avr-")
set(CMAKE_C_COMPILER "${TOOLCHAIN_PREFIX}gcc")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}g++")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_PREFIX}gcc")
set(CMAKE_OBJCOPY "${TOOLCHAIN_PREFIX}objcopy")
set(CMAKE_SIZE "${TOOLCHAIN_PREFIX}size")
set(STM32_STARTUP_SCRIPT "${CMAKE_SOURCE_DIR}/startup.s")

set(COMPILER_OPTIONS "-mmcu=atmega328p --max-errors=5")

# Set the compiler flags
set(CMAKE_ASM_FLAGS "${COMPILER_OPTIONS} -c -x assembler-with-cpp")

set(CMAKE_C_FLAGS
    "${COMPILER_OPTIONS} -O0 -g -ffunction-sections -fdata-sections -fshort-enums -Wl,--print-memory-usage"
)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} --std=c++17")

# Define the target
set(CMAKE_EXECUTABLE_SUFFIX ".hex")

add_compile_definitions(ETL_NO_STL=1)
add_compile_definitions(ETL_NO_64BIT_TYPES=1)
add_compile_definitions(F_CPU=16000000)
add_compile_definitions(ARDUINO)

include_directories(SYSTEM ${STM32_SYSTEM_DIRECTORIES}
                    ${STM32_FREERTOS_SYSTEM_DIRECTORIES})
