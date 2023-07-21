cmake_minimum_required(VERSION 3.5)

include("${CMAKE_SOURCE_DIR}/cc65-find.cmake")
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cc65-toolchain.cmake")

project(kokobatoru C ASM)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_SYSTEM_VERSION "nes")
set(CMAKE_SYSTEM_PROCESSOR "m6502")
set(CMAKE_SKIP_INSTALL_RULES TRUE)
set(CMAKE_EXECUTABLE_SUFFIX_C ".nes")
set(CMAKE_C_FLAGS "-t nes ${CMAKE_C_FLAGS} -Oi")
set(CMAKE_EXE_LINKER_FLAGS "-C nrom_128_horz.cfg ${CMAKE_EXE_LINKER_FLAGS}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable("kokobatoru"
    "${CMAKE_SOURCE_DIR}/kokobatoru.c"
    "${CMAKE_SOURCE_DIR}/asm/crt0.s"
)

add_executable("cocobattleroyale"
    "${CMAKE_SOURCE_DIR}/cocobattleroyale.c"
    "${CMAKE_SOURCE_DIR}/asm/crt0.s"
)
