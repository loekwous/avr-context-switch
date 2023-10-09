cmake_minimum_required(VERSION 3.22.1)

include (cmake/Toolchain_AVR.cmake)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/Toolchain_AVR.cmake")

project(AVR_TEST C CXX ASM)

# Set the C standard
set(CMAKE_C_STANDARD 11)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)


include (cmake/etl.cmake)

FetchETL()


add_executable(${CMAKE_PROJECT_NAME} "main.cpp" "startup.s")

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC 
    "${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/_deps/etl-src/include/"
    "${CMAKE_SOURCE_DIR}/Application"
)

file(GLOB_RECURSE AVR_SOURCES "${CMAKE_SOURCE_DIR}/Application/*.cpp")

target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${AVR_SOURCES})

set_target_properties(${CMAKE_PROJECT_NAME}
    PROPERTIES
    NO_SYSTEM_FROM_IMPORTED ON
    )

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(ELF_FILE "${CMAKE_PROJECT_NAME}.elf")
set(HEX_FILE "${CMAKE_PROJECT_NAME}.hex")
set(ASM_FILE "${CMAKE_PROJECT_NAME}.asm")

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME ${ELF_FILE})

add_custom_command(
		OUTPUT ${HEX_FILE}
		COMMAND
			${CMAKE_OBJCOPY} -j .text -j .data -O ihex ${ELF_FILE} ${HEX_FILE}
		DEPENDS ${ELF_FILE}
	)

    add_custom_command(
		OUTPUT ${ASM_FILE}
		COMMAND
			${TOOLCHAIN_PREFIX}objdump -d ${ELF_FILE} > ${ASM_FILE}
		DEPENDS ${ELF_FILE}
	)

add_custom_target(APPEND_FOR_HEX ALL DEPENDS ${CMAKE_PROJECT_NAME} ${HEX_FILE} ${ASM_FILE})
add_custom_target(APPEND_FOR_ASM ALL DEPENDS ${CMAKE_PROJECT_NAME} ${ELF_FILE} ${ASM_FILE})
