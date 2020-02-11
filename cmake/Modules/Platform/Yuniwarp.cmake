# Skip finding WARPSDK. It will define:
#  WARPSDK_AR
#  WARPSDK_WASM2C
#  WARPSDK_CLANG
#  WARPSDK_LINKER
#include(${CMAKE_CURRENT_LIST_DIR}/../../protopaths.cmake)

#get_filename_component(WARPSDK_ROOT ${CMAKE_CURRENT_LIST_DIR}/../../.. ABSOLUTE)
#set(WARPSDK_SYSROOT "${WARPSDK_ROOT}/sysroot")

list(APPEND CMAKE_MODULE_PATH "${WARPSDK_ROOT}/cmake/Modules")

cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_SYSTEM_NAME Yuniwarp)
set(CMAKE_SYSTEM_VERSION 1) # ??

set(CMAKE_CROSSCOMPILING TRUE) # FIXME: ??

# Hacks
if(CMAKE_HOST_WIN32)
    # CMAKE_DIAGNOSE_UNSUPPORTED_CLANG Workaround
    message(STATUS "Disable Host_Win32 (WAR)")
    set(CMAKE_HOST_WIN32)
endif()

# Emscripten hacks
set(CMAKE_SYSTEM_PROCESSOR x86) # We're 32bits
set(WIN32)
set(APPLE)
if(CMAKE_TOOLCHAIN_FILE)
    # Do nothing, just consume the variable
endif()


set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Warp32 specific variables
set(WARP32_TARGET_TRIPLE "wasm32")
set(WARP32_THREAD_MODEL "") # Enable Pthread defines
set(WARP32_CFLAGS_WAR "")

# FIXME: We still need to provide memcpy memmove memset memcmp REF: https://gcc.gnu.org/onlinedocs/gcc/Standards.html
set(WARP32_CFLAGS "${WARP32_CFLAGS_WAR} --target=${WARP32_TARGET_TRIPLE} --sysroot=${WARPSDK_SYSROOT} -ffreestanding")
set(WARP32_DEFS "-D__WARP32LE__ -D__WARP32__ -D__WARP__")
# FIXME: Per-symbol exports might be prefered -- needs .def though
#        --Wl,--export=SYMBOL
#set(WARP32_LDFLAGS "-nostdlib -Wl,--no-entry -Wl,--export-dynamic -Wl,--allow-undefined-file=${WARPSDK_SYSROOT}/sysroot.syms")
set(WARP32_LDFLAGS "-nostdlib -Wl,--no-entry -Wl,--export-dynamic -Wl,--allow-undefined")
#set(WARP32_LDFLAGS "-nostdlib -Wl,--no-entry") ## For newer LLVM

# Configure compiler templates

set(CMAKE_AR "${WARPSDK_AR}")
foreach(lang C CXX)
    string(APPEND CMAKE_${lang}_FLAGS_INIT " ")
    string(APPEND CMAKE_${lang}_FLAGS_DEBUG_INIT " -g") # ??
    string(APPEND CMAKE_${lang}_FLAGS_MINSIZEREL_INIT " -Os -DNDEBUG")
    string(APPEND CMAKE_${lang}_FLAGS_RELEASE_INIT " -O3 -DNDEBUG")
    string(APPEND CMAKE_${lang}_FLAGS_RELWITHDEBINFO_INIT " -O2 -g -DNDEBUG")
    set(CMAKE_${lang}_COMPILER "${WARPSDK_CLANG}")

    # Object
    set(CMAKE_${lang}_COMPILE_OBJECT "<CMAKE_${lang}_COMPILER> ${WARP32_CFLAGS} ${WARP32_DEFS} <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>")
    set(CMAKE_DEPFILE_FLAGS_${lang} "-MD -MT <OBJECT> -MF <DEPFILE>")

    # File
    set(CMAKE_${lang}_CREATE_STATIC_LIBRARY "<CMAKE_AR> rc <TARGET> <LINK_FLAGS> <OBJECTS>")

    # Executables
    set(CMAKE_${lang}_CREATE_SHARED_MODULE
        "<CMAKE_${lang}_COMPILER> ${WARP32_LDFLAGS} ${WARP32_CFLAGS} <LINK_FLAGS> -o <TARGET> <OBJECTS> <LINK_LIBRARIES>")
    set(CMAKE_${lang}_CREATE_SHARED_LIBRARY
        "<CMAKE_${lang}_COMPILER> ${WARP32_LDFLAGS} ${WARP32_CFLAGS} <LINK_FLAGS> -o <TARGET> <OBJECTS> <LINK_LIBRARIES>")
    # Executables are same for shared libraries
    set(CMAKE_${lang}_LINK_EXECUTABLE
        "<CMAKE_${lang}_COMPILER> ${WARP32_LDFLAGS} ${WARP32_CFLAGS} <LINK_FLAGS> -o <TARGET> <OBJECTS> <LINK_LIBRARIES>")
endforeach()
