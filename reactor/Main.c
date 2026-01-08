#include "unp.h"

#include "DiagnosticsServer.h"
#include "ReactorEventLoop.h"
#include "Error.h"

int main(void){

    /* Create a server and enter an eternal event loop, watching
       the Reactor do the rest. */

    const unsigned int serverPort = SERV_PORT;
    DiagnosticsServerPtr server = createServer(serverPort);

    if(NULL == server) {
        error("Failed to create the server");
    }

    /* Enter the eternal reactive event loop. */
    for(;;){
        HandleEvents();
    }
}