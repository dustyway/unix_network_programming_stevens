cmake_minimum_required(VERSION 3.22)
project(unix_network_programming_stevens C)

set(CMAKE_C_STANDARD 11)

set(LIBRARY_NAME lib_stevens)

add_subdirectory(lib)
add_subdirectory(intro)
add_subdirectory(reactor)
add_subdirectory(select)
add_subdirectory(tcpcliserv)
