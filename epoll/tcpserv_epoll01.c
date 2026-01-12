/*  this is my own adaption of tcpserv_poll01.c replacing poll with epoll */

#include	"unp.h"
#include <sys/epoll.h>
#define MAX_CLIENTS 1024
#define MAX_EVENTS 16
#define NOTDEF  1


static int clients[MAX_CLIENTS];
static int client_count = 0;
static int listenfd = -1;
static int epoll_fd = -1;
static volatile sig_atomic_t running = 1;

void cleanup() {
    printf("\nCleaning up...\n");

    // Close all client connections
    for (int i = 0; i < client_count; i++) {
        if (clients[i] >= 0) {
            printf("Closing client fd=%d\n", clients[i]);
            close(clients[i]);
        }
    }

    // Close listener and epoll
    if (listenfd >= 0) close(listenfd);
    if (epoll_fd >= 0) close(epoll_fd);

    printf("Server shutdown complete\n");
}

void sigint_handler(int sig) {
    running = 0;
}

void add_client(int fd) {
    if (client_count < MAX_CLIENTS) {
        clients[client_count++] = fd;
    }
}

void remove_client(int fd) {
    for (int i = 0; i < client_count; i++) {
        if (clients[i] == fd) {
            // Shift remaining clients down
            for (int j = i; j < client_count - 1; j++) {
                clients[j] = clients[j + 1];
            }
            client_count--;
            break;
        }
    }
}


int
main(int argc, char **argv)
{
	int					i, connfd, sockfd;
	int					nready;
	ssize_t				n;
	char				line[MAXLINE];
	socklen_t			clilen;
    struct epoll_event events[MAX_EVENTS];
	struct sockaddr_in	cliaddr, servaddr;

    // Setup cleanup on exit
    atexit(cleanup);
    signal(SIGINT, sigint_handler);
    signal(SIGTERM, sigint_handler);

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    int optval = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        perror("setsockopt");
        return 1;
    }

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

    // Create epoll instance
    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        perror("epoll_create1");
        return 1;
    }

    // Add listenfd to epoll
    struct epoll_event ev = {0};
    ev.events = EPOLLIN;
    ev.data.fd = listenfd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listenfd, &ev) < 0) {
        perror("epoll_ctl");
        return 1;
    }

    while (running) {
		nready = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nready < 0) {
            if (errno == EINTR) continue;
            perror("epoll_wait");
            break;
        }

        for (i = 0; i < nready; i++) {
            // New connection
            if (events[i].data.fd == listenfd) {
                clilen = sizeof(cliaddr);
                connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
#ifdef	NOTDEF
                printf("new client: %s\n", Sock_ntop((SA *) &cliaddr, clilen));
#endif

                if (client_count >= MAX_CLIENTS) {
                    printf("Max clients reached, rejecting connection\n");
                    Close(connfd);
                    continue;
                }

                printf("New connection: fd=%d (total: %d)\n", connfd, client_count + 1);

                add_client(connfd);
                ev.events = EPOLLIN;
                ev.data.fd = connfd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connfd, &ev) < 0) {
                    perror("epoll_ctl: client");
                    remove_client(connfd);
                    Close(connfd);
                }
            } else {
                // Data from client
                sockfd = events[i].data.fd;
                if ( (n = Readline(sockfd, line, MAXLINE)) <= 0) {
                    // Connection closed or error
                    if (n < 0) perror("readline");
                    printf("Connection closed: fd=%d (remaining: %d)\n", sockfd, client_count - 1);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, sockfd, NULL);
                    Close(sockfd);
                    remove_client(sockfd);
                } else
                    Writen(sockfd, line, n);
            }
		}
	}
    return 0;
}
