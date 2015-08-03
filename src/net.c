#include "spp.h"

/* receive data and decrypt if needed, then return */
void *net_recv(int fd, CRYPT_SETUP *encryption)
{
    void *recvbuf;
    void *tmp_headerbuf;
    uint16_t recvsize;
    int n;

    recvbuf = _malloc(PKT_HEADER_SIZE);
    tmp_headerbuf = _malloc(PKT_HEADER_SIZE);

    /* first get the header */
    n = recv(fd, recvbuf, PKT_HEADER_SIZE, 0);

    /* 0 = orderly shutdown */
    if(!n) {
        free(recvbuf);
        return NULL;
    }
    /* -1 = error */
    else if(n == -1)
        net_assert(n, "recv");

    printf("%x - %d\n", encryption, crypt_active);

    /* if there's no encryption set */
    if(!encryption) {
        printf("NO ENCRYPTION\n");
        recvsize = pkt_sizeof(recvbuf);

        memcpy(tmp_headerbuf, recvbuf, PKT_HEADER_SIZE);

        /* reallocate more space for the packet */
        recvbuf = _realloc(recvbuf, recvsize);

        memcpy(recvbuf, tmp_headerbuf, PKT_HEADER_SIZE);

        free(tmp_headerbuf);

        /* just get the data, no encryption stuff */
        n = recv(fd, recvbuf + PKT_HEADER_SIZE, recvsize - PKT_HEADER_SIZE, 0);

        /* 0 = orderly shutdown */
        if(!n) {
            free(recvbuf);
            return NULL;
        }
        /* -1 = error */
        else if(n == -1)
            net_assert(n, "recv");

        return recvbuf;
    }

    printf("USING ENCRYPTION\n");

    /* otherwise there's an encryption... */
    /* decrypt the header */
    CRYPT_CryptData(encryption, recvbuf, PKT_HEADER_SIZE, false);
    printf("header = \n");
    pkt_dump(recvbuf, pkt_sizeof(recvbuf));
    printf("\n\n");

    recvsize = pkt_sizeof(recvbuf);

    memcpy(tmp_headerbuf, recvbuf, PKT_HEADER_SIZE);

    /* reallocate more space for the packet */
    recvbuf = _realloc(recvbuf, recvsize);

    memcpy(recvbuf, tmp_headerbuf, PKT_HEADER_SIZE);

    free(tmp_headerbuf);

    /* get the new data now that the size is revealed */
    n = recv(fd, recvbuf + PKT_HEADER_SIZE, recvsize - PKT_HEADER_SIZE, 0);

    /* error check */
    net_assert(n, "recv");

    /* 0 = orderly shutdown */
    if(!n) {
        free(tmp_headerbuf);
        free(recvbuf);
        return NULL;
    }

    /* decrypt new data */
    CRYPT_CryptData(encryption, recvbuf + PKT_HEADER_SIZE, recvsize -
            PKT_HEADER_SIZE, false);

    /* return full packet (including header) */
    return recvbuf;
}

/* encrypt (if needed) and send data */
int net_send(int fd, void *data, CRYPT_SETUP *encryption)
{
    void *sendbuf;
    uint16_t sendsize;
    int n;

    /* if there's no encryption */
    if(!encryption)
        sendsize = pkt_sizeof(data);
    else {
        sendbuf = _malloc(PKT_HEADER_SIZE);
        memcpy(sendbuf, data, 4);
        sendsize = pkt_sizeof(data);
        free(sendbuf);
    }

    sendbuf = _malloc(sendsize);
    memcpy(sendbuf, data, sendsize);

    /* if there's no encryption set */
    if(!encryption) {
            printf("sending data:\n");
    pkt_dump(sendbuf, pkt_sizeof(sendbuf));

        /* send data as is */
        n = send(fd, sendbuf, sendsize, 0);

        /* error check */
        net_assert(n, "send");

        free(sendbuf);

        return n;
    }

    /* otherwise there's an encryption... */
    /* encrypt data */
    CRYPT_CryptData(encryption, sendbuf, sendsize, true);

    printf("sending data:\n");
    pkt_dump(sendbuf, pkt_sizeof(sendbuf));

    /* send encrypted data */
    n = send(fd, sendbuf, sendsize, 0);

    /* error check */
    net_assert(n, "send");

    free(sendbuf);

    return n;
}

/* error checking */
void net_assert(int n, char *func)
{
    /* error */
    if(n == -1) {
        printf("error: %s failed: %s\n", func, strerror(errno));
        exit(1);
    }
}
