set(LIBRARY_SOURCES
        error.c
        readline.c
        str_cli08.c
        str_echo.c
        wraplib.c
        wrapsock.c
        wrapstdio.c
        wrapunix.c
        writen.c
)
set(LIBRARY_HEADERS
        "unp.h")

add_library(${LIBRARY_NAME} STATIC
        ${LIBRARY_SOURCES}
        ${LIBRARY_HEADERS})
target_include_directories(${LIBRARY_NAME} PUBLIC
        "./")