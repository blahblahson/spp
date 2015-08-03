#include "spp.h"

void *_malloc(size_t size)
{
    void *ret = malloc(size);

    if(!ret) {
        printf("error: call to malloc(%d) failed: %s\n",
                size, strerror(errno));
        exit(1);
    }

    memset(ret, 0, size); /* clear memory */

    return ret;
}

void *_realloc(void *ptr, size_t size)
{
    void *ret = realloc(ptr, size);

    /* error */
    if(!ptr) {
        printf("error: call to realloc(%x, %d) failed: %s\n", (unsigned int)
                ptr, size, strerror(errno));
        exit(1);
    }
    
    memset(ret, 0, size); /* clear memory */

    return ret;
}

uint16_t pkt_sizeof(void *packet)
{
    unsigned short i;
    memcpy(&i, packet + 2, 2);

    return i;
}

uint8_t pkt_cmd(void *packet)
{
    uint8_t i;
    memcpy(&i, packet, 1);

    return i;
}
