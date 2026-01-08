add_executable(tcpserv01r
        DiagnosticsClient.c
        DiagnosticsServer.c
        Main.c
        PollReactor.c
        TcpServer.c
)
target_include_directories(tcpserv01r PUBLIC .)
target_link_libraries(tcpserv01r PUBLIC ${LIBRARY_NAME})