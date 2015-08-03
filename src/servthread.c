#include "spp.h"

void *servthread(void *nothing)
{
    void *recvbuf;
    int n;

    while(1) {
        recvbuf = net_recv(servfd, ENCRYPTION_SERV_IN);

        pthread_mutex_lock(&net_mutex);

        /* in event of d/c */
        if(recvbuf == NULL) {
            printf("SERVER CLOSING CONNECTION\n");
            n = close(servfd);
            net_assert(n, "close");
            close(clientfd);
            net_assert(n, "close");
            printf("CLIENT CONNECTION CLOSED\n");

            exit(0);
        }
    
        printf("SERVER -> CLIENT\n");
        pkt_dump(recvbuf, pkt_sizeof(recvbuf));
        printf("\n\n");

        /* CONNECTION HIJACK LOL */
        if(pkt_cmd(recvbuf) == 0x19) { /* redirect packet */
            printf("TELEPORTATION ENGAGE! HOLD ON TIGHT\n");
            /* disconnect from the server as the client would do so */
            n = close(servfd);
            net_assert(n, "close");

            /* set up servaddr struct for connection to new location */
            memcpy(&servaddr.sin_addr.s_addr, recvbuf + 4, 4); /* IP */
            memcpy(&servaddr.sin_port, recvbuf + 8, 2); /* port */

            /* connect to new location */
            connect(servfd, (struct sockaddr *) &servaddr,
                    sizeof(servaddr));
            net_assert(n, "connect");

            /* set up information for the GC client (reconnect to proxy) */
            memcpy(recvbuf + 4, &proxyaddr.sin_addr.s_addr, 4); /* IP
            */
            memcpy(recvbuf + 8, &proxyaddr.sin_port, 2); /* port */

            /* tell the client to reconnect to proxy */
            net_send(clientfd, recvbuf, ENCRYPTION_SERV_OUT);

            /* wait for new connection */
            int tmpfd = accept(listenfd, (struct sockaddr *)
                    &clientaddr, &clientaddr_size);
            net_assert(tmpfd, "accept");

            /* now close old connection */
            n = close(clientfd);
            net_assert(n, "close");

            /* and set clientfd to the new socket */
            clientfd = tmpfd;
        }
        /* encryption stuff */
        else if(pkt_cmd(recvbuf) == 0x02 || pkt_cmd(recvbuf) == 0x17) {
            printf("SETTING ENCRYPTION\n");
            /* get encryption keys and set them to crypt_serv and
             * crypt_client */
            CRYPT_CreateKeys(&crypt_serv_in, recvbuf + 0x44,
                    CRYPT_GAMECUBE);
            CRYPT_CreateKeys(&crypt_client_out, recvbuf + 0x44,
                    CRYPT_GAMECUBE);
            CRYPT_CreateKeys(&crypt_client_in, recvbuf + 0x48,
                    CRYPT_GAMECUBE);
            CRYPT_CreateKeys(&crypt_serv_out, recvbuf + 0x48,
                    CRYPT_GAMECUBE);
            net_send(clientfd, recvbuf, ENCRYPTION_SERV_OUT);
            free(recvbuf);
            crypt_active = 1;
        }
        else {
            net_send(clientfd, recvbuf, ENCRYPTION_SERV_OUT);
        }

        free(recvbuf);

        pthread_mutex_unlock(&net_mutex);
    }

    return NULL;
}

