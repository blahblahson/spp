#ifndef _NET_H_
#define _NET_H_

#include "spp.h"

#define PKT_HEADER_SIZE 4

void *net_recv(int, CRYPT_SETUP *);
int net_send(int, void *, CRYPT_SETUP *);
void net_assert(int, char *);

#endif /* _NET_H_ */
