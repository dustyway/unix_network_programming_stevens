add_executable(tcpcli01s tcpcli01.c strcliselect01.c)
target_link_libraries(tcpcli01s PUBLIC ${LIBRARY_NAME})

add_executable(tcpcli02s tcpcli02.c strcliselect02.c)
target_link_libraries(tcpcli02s PUBLIC ${LIBRARY_NAME})

add_executable(tcpcli03s tcpcli03.c)
target_link_libraries(tcpcli03s PUBLIC ${LIBRARY_NAME})
