#ifndef _SPP_H_
#define _SPP_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "encryption.h"
#include "printpacket.h"
#include "net.h"
#include "misc.h"

#define SERVER argv[1]
#define PORT (atoi(argv[2]))
#define BACKLOG 2

/* LOL GLOBAL VARIABLES */
pthread_mutex_t net_mutex;
struct sockaddr_in proxyaddr;
struct sockaddr_in servaddr;
struct sockaddr_in clientaddr;
size_t clientaddr_size;
int servfd;
int clientfd;
int listenfd;
CRYPT_SETUP crypt_serv_in;
CRYPT_SETUP crypt_serv_out;
CRYPT_SETUP crypt_client_in;
CRYPT_SETUP crypt_client_out;
int crypt_active;

void *servthread(void *);

#define ENCRYPTION_SERV_IN (crypt_active ? &crypt_serv_in : NULL)
#define ENCRYPTION_SERV_OUT (crypt_active ? &crypt_serv_out : NULL)
#define ENCRYPTION_CLIENT_IN (crypt_active ? &crypt_client_in : NULL)
#define ENCRYPTION_CLIENT_OUT (crypt_active ? &crypt_client_out : NULL)

#endif /* _SPP_H_ */
