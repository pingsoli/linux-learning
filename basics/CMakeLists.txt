cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Debug)

add_executable(dup2           dup2.c)
add_executable(dup            dup.c)
add_executable(getopt         getopt.c)
add_executable(getopt_long    getopt_long.c)
add_executable(resource_limit resource_limit.c)
add_executable(syscall syscall.c)
add_executable(strftime strftime.c)
add_executable(strcpy strcpy.c)
add_executable(memcpy memcpy.c)
add_executable(memmove memmove.c)
