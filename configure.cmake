set(WARPSDK_CLANG c:/prog/llvm90/bin/clang.exe)

execute_process(
    COMMAND ${CMAKE_COMMAND} 
    -DWARPSDK_CLANG=${WARPSDK_CLANG}
    -DWARPSDK_SYSROOT=${CMAKE_CURRENT_LIST_DIR}/proxylibc/sysroot
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_CURRENT_LIST_DIR}/cmake/Modules/Platform/Yuniwarp.cmake
    -G Ninja ${CMAKE_CURRENT_LIST_DIR})
