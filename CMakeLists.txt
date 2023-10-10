cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/Toolchain_AVR.cmake")

project(AVR_TEST C CXX ASM)

include(cmake/etl.cmake)
include(GNUInstallDirs)

fetchetl()

set(TARGET_NAME ${CMAKE_PROJECT_NAME})
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_subdirectory(${CMAKE_SOURCE_DIR}/Os)

# Executable
add_executable(${TARGET_NAME} "main.cpp" "startup.s")

target_include_directories(
  ${TARGET_NAME}
  PUBLIC "${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/_deps/etl-src/include/")

set_target_properties(${TARGET_NAME} PROPERTIES NO_SYSTEM_FROM_IMPORTED ON)

set(ELF_FILE "${TARGET_NAME}.elf")
set(HEX_FILE "${TARGET_NAME}.hex")
set(ASM_FILE "${TARGET_NAME}.asm")

set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME ${ELF_FILE})

add_custom_command(
  OUTPUT ${HEX_FILE}
  COMMAND ${CMAKE_OBJCOPY} -j .text -j .data -O ihex ${ELF_FILE} ${HEX_FILE}
  DEPENDS ${ELF_FILE})

add_custom_command(
  OUTPUT ${ASM_FILE}
  COMMAND ${TOOLCHAIN_PREFIX}objdump -d ${ELF_FILE} > ${ASM_FILE}
  DEPENDS ${ELF_FILE})

add_custom_target(APPEND_FOR_HEX ALL DEPENDS ${TARGET_NAME} ${HEX_FILE}
                                             ${ASM_FILE})
add_custom_target(APPEND_FOR_ASM ALL DEPENDS ${TARGET_NAME} ${ELF_FILE}
                                             ${ASM_FILE})

target_link_libraries(${TARGET_NAME} PRIVATE OsScheduler)

install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(TARGETS OsScheduler LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES "${CMAKE_BINARY_DIR}/${ASM_FILE}"
        DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES "${CMAKE_BINARY_DIR}/${HEX_FILE}"
        DESTINATION ${CMAKE_INSTALL_BINDIR})
