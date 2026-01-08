# Construct the CPU_VENDOR_OS string
set(CPU "${CMAKE_SYSTEM_PROCESSOR}")
set(VENDOR "unknown")
set(OS "${CMAKE_SYSTEM_NAME}")

if(APPLE)
    set(VENDOR "apple")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|i686")
    set(VENDOR "pc")
elseif(MINGW OR MSYS)
    set(VENDOR "w64")
endif()

# Normalize OS names
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(OS "darwin")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(OS "linux-gnu")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(OS "windows")
endif()

# Create the triplet string
set(CPU_VENDOR_OS "${CPU}-${VENDOR}-${OS}")

# Build the executable
add_executable(byteorder byteorder.c)
target_link_libraries(byteorder PUBLIC ${LIBRARY_NAME})

# Pass CPU_VENDOR_OS as a compile definition
target_compile_definitions(byteorder PRIVATE
        CPU_VENDOR_OS="${CPU_VENDOR_OS}"
)

add_executable(daytimetcpsrv daytimetcpsrv.c)
target_link_libraries(daytimetcpsrv PUBLIC ${LIBRARY_NAME})

add_executable(daytimetcpsrv1 daytimetcpsrv1.c)
target_link_libraries(daytimetcpsrv1 PUBLIC ${LIBRARY_NAME})

add_executable(daytimetcpcli daytimetcpcli.c)
target_link_libraries(daytimetcpcli PUBLIC ${LIBRARY_NAME})
