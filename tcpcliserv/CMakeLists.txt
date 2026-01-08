add_executable(tcpcli01 tcpcli01.c)
target_link_libraries(tcpcli01 PUBLIC ${LIBRARY_NAME})

add_executable(tcpcli04 tcpcli04.c)
target_link_libraries(tcpcli04 PUBLIC ${LIBRARY_NAME})

add_executable(tcpcli05 tcpcli05.c)
target_link_libraries(tcpcli05 PUBLIC ${LIBRARY_NAME})

add_executable(tcpcli08 tcpcli08.c str_cli08.c)
target_link_libraries(tcpcli08 PUBLIC ${LIBRARY_NAME})

add_executable(tcpcli09 tcpcli09.c str_cli09.c)
target_link_libraries(tcpcli09 PUBLIC ${LIBRARY_NAME})
target_include_directories(tcpcli09 PUBLIC .)

add_executable(tcpserv01 tcpserv01.c)
target_link_libraries(tcpserv01 PUBLIC ${LIBRARY_NAME})

add_executable(tcpserv02 tcpserv02.c sigchldwait.c)
target_link_libraries(tcpserv02 PUBLIC ${LIBRARY_NAME})

add_executable(tcpserv03 tcpserv03.c sigchldwait.c)
target_link_libraries(tcpserv03 PUBLIC ${LIBRARY_NAME})

add_executable(tcpserv04 tcpserv04.c sigchldwaitpid.c)
target_link_libraries(tcpserv04 PUBLIC ${LIBRARY_NAME})

add_executable(tcpserv08 tcpserv08.c str_echo08.c sigchldwaitpid.c)
target_link_libraries(tcpserv08 PUBLIC ${LIBRARY_NAME})

add_executable(tcpserv09 tcpserv09.c str_echo09.c sigchldwaitpid.c)
target_link_libraries(tcpserv09 PUBLIC ${LIBRARY_NAME})

add_executable(tcpservselect01 tcpservselect01.c)
target_link_libraries(tcpservselect01 PUBLIC ${LIBRARY_NAME})

add_executable(tcpservpoll01 tcpservpoll01.c)
target_link_libraries(tcpservpoll01 PUBLIC ${LIBRARY_NAME})
