/*  this is my own adaption of tcpserv_poll01.c replacing poll with epoll */

#include	"unp.h"
#include <sys/epoll.h>
#define MAX_EVENTS 16
#define NOTDEF  1

int
main(int argc, char **argv)
{
	int					i, listenfd, connfd;
	int					nready, epoll_fd;
	ssize_t				n;
	char				line[MAXLINE];
	socklen_t			clilen;
    struct epoll_event events[MAX_EVENTS];
	struct sockaddr_in	cliaddr, servaddr;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

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
        close(listenfd);
        return 1;
    }

    // Add listenfd to epoll
    struct epoll_event ev = {0};
    ev.events = EPOLLIN;
    ev.data.fd = listenfd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listenfd, &ev) < 0) {
        perror("epoll_ctl");
        close(listenfd);
        close(epoll_fd);
        return 1;
    }

    for ( ; ; ) {
		nready = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nready < 0) {
            perror("epoll_wait");
            break;
        }

        for (i = 0; i < nready; i++) {
            if (events[i].data.fd == listenfd) {
                // New connection
                connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
                int client = accept(listenfd, NULL, NULL);
                if (client < 0) {
                    perror("accept");
                    continue;
                }

                printf("New connection: fd=%d\n", client);

                ev.events = EPOLLIN;
                ev.data.fd = client;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client, &ev) < 0) {
                    perror("epoll_ctl: client");
                    close(client);
                }
            }


		if (client[0].revents & POLLRDNORM) {	/* new client connection */
			clilen = sizeof(cliaddr);
			connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
#ifdef	NOTDEF
			printf("new client: %s\n", Sock_ntop((SA *) &cliaddr, clilen));
#endif

			for (i = 1; i < OPEN_MAX; i++)
				if (client[i].fd < 0) {
					client[i].fd = connfd;	/* save descriptor */
					break;
				}
			if (i == OPEN_MAX)
				err_quit("too many clients");

			client[i].events = POLLRDNORM;
			if (i > maxi)
				maxi = i;				/* max index in client[] array */

			if (--nready <= 0)
				continue;				/* no more readable descriptors */
		}

		for (i = 1; i <= maxi; i++) {	/* check all clients for data */
			if ( (sockfd = client[i].fd) < 0)
				continue;
			if (client[i].revents & (POLLRDNORM | POLLERR)) {
				if ( (n = readline(sockfd, line, MAXLINE)) < 0) {
					if (errno == ECONNRESET) {
							/*4connection reset by client */
#ifdef	NOTDEF
						printf("client[%d] aborted connection\n", i);
#endif
						Close(sockfd);
						client[i].fd = -1;
					} else
						err_sys("readline error");
				} else if (n == 0) {
						/*4connection closed by client */
#ifdef	NOTDEF
					printf("client[%d] closed connection\n", i);
#endif
					Close(sockfd);
					client[i].fd = -1;
				} else
					Writen(sockfd, line, n);

				if (--nready <= 0)
					break;				/* no more readable descriptors */
			}
		}
	}
}
/* end fig02 */
