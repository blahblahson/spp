#include "spp.h"

int main(int argc, char *argv[])
{
    if(argc < 3) {
        printf("usage:\n\tspp <server IP> <port>\nspp will run on <port> and "
                "connect to the server on it, too\n");
        return 0;
    }

    int n;

    pthread_mutex_init(&net_mutex, NULL);

    crypt_active = 0;

    /* set up addr structs */
    memset(&proxyaddr, 0, sizeof(proxyaddr));
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&clientaddr, 0, sizeof(clientaddr));
    clientaddr_size = sizeof(clientaddr);

    /* set up sockets */
    servfd = socket(PF_INET, SOCK_STREAM, 0);
    net_assert(servfd, "socket");

    listenfd = socket(PF_INET, SOCK_STREAM, 0);
    net_assert(listenfd, "socket");

    /* set up listenfd */
    proxyaddr.sin_family = AF_INET;
    proxyaddr.sin_port = htons(PORT);
    proxyaddr.sin_addr.s_addr = INADDR_ANY;

    n = bind(listenfd, (struct sockaddr *) &proxyaddr, sizeof(proxyaddr));
    net_assert(n, "bind");

    n = listen(listenfd, BACKLOG);
    net_assert(n, "listen");

    /* initialise servaddr struct */
    struct hostent *h = gethostbyname(SERVER);
    /* uses h_error so net_assert won't work normally */
    if(!h) {
        errno = h_errno;
        net_assert(-1, "gethostbyname");
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr = (*((struct in_addr *) h->h_addr));

    /* accept new client */
    clientfd = accept(listenfd, (struct sockaddr *) &clientaddr,
            &clientaddr_size);
    net_assert(clientfd, "accept");

    /* connect to server */
    connect(servfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    net_assert(n, "connect");

    printf("%s connected\n", inet_ntoa(clientaddr.sin_addr));

    pthread_t servthread_id;
    pthread_create(&servthread_id, NULL, &servthread, NULL);

    void *recvbuf;

    while(1) {
        recvbuf = net_recv(clientfd, ENCRYPTION_CLIENT_IN);

        pthread_mutex_lock(&net_mutex);        

        /* in event of d/c */
        if(recvbuf == NULL) {
            printf("CLIENT CLOSING CONNECTION\n");
            n = close(servfd);
            net_assert(n, "close");
            close(clientfd);
            net_assert(n, "close");
            printf("SERVER CONNECTION CLOSED\n");

            exit(0);
        }

        printf("CLIENT -> SERVER\n");
        pkt_dump(recvbuf, pkt_sizeof(recvbuf));
        printf("\n\n");

        net_send(servfd, recvbuf, ENCRYPTION_CLIENT_IN);

        free(recvbuf);

        pthread_mutex_unlock(&net_mutex);
    }

    return 0;
}
