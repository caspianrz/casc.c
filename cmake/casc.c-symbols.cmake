include(CheckSymbolExists)
include(CheckIncludeFiles)
include(CheckFunctionExists)

check_include_files("stdio.h" CASC_HAVE_STDIO_H)
check_include_files("unistd.h" CASC_HAVE_UNISTD_H)
check_include_files("stdlib.h" CASC_HAVE_STDLIB_H)
check_include_files("sys/stat.h" CASC_HAVE_SYS_STAT_H)

check_symbol_exists(mmap "sys/mman.h" CASC_HAVE_MMAP)
check_symbol_exists(munmap "sys/mman.h" CASC_HAVE_MUNMAP)

check_function_exists(malloc CASC_HAVE_MALLOC)
check_function_exists(calloc CASC_HAVE_CALLOC)
check_function_exists(realloc CASC_HAVE_REALLOC)
check_function_exists(free CASC_HAVE_FREE)

check_function_exists(access CASC_HAVE_ACCESS)
check_function_exists(stat CASC_HAVE_STAT)
check_function_exists(fstat CASC_HAVE_FSTAT)

check_function_exists(_stat CASC_HAVE__STAT)
check_function_exists(_access CASC_HAVE__ACCESS)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/casc.c/symbols.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/casc.c/symbols.h
)