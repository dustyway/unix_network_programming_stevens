add_executable(tcpcli01 tcpcli01.c)
target_link_libraries(tcpcli01 PUBLIC ${LIBRARY_NAME})

add_executable(tcpserv01 tcpserv01.c)
target_link_libraries(tcpserv01 PUBLIC ${LIBRARY_NAME})

add_executable(tcpservselect01 tcpservselect01.c)
target_link_libraries(tcpservselect01 PUBLIC ${LIBRARY_NAME})
