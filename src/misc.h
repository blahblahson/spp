#ifndef _MISC_H_
#define _MISC_H_

#include "spp.h"

void *_malloc(size_t);

void *_realloc(void *, size_t);

uint16_t pkt_sizeof(void *);

uint8_t pkt_cmd(void *);

#endif /* _MISC_H_ */
