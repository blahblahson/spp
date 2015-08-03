#include "spp.h"

/* PSO Encryption Library (PC Encryption)
 * 
 * All included source written by Fuzziqer Software except where otherwise
 * indicated.  copyright 2004 
 */

/* notes:
 * - Kohle's encryption doesn't work without all the source files in the
 *   library; lobbylurk may in future support PC but it's unlikely
 */

void CRYPT_PC_MixKeys(CRYPT_SETUP* pc)
{
    unsigned long esi,edi,eax,ebp,edx;
    edi = 1;
    edx = 0x18;
    eax = edi;
    while (edx > 0)
    {
        esi = pc->keys[eax + 0x1F];
        ebp = pc->keys[eax];
        ebp = ebp - esi;
        pc->keys[eax] = ebp;
        eax++;
        edx--;
    }
    edi = 0x19;
    edx = 0x1F;
    eax = edi;
    while (edx > 0)
    {
        esi = pc->keys[eax - 0x18];
        ebp = pc->keys[eax];
        ebp = ebp - esi;
        pc->keys[eax] = ebp;
        eax++;
        edx--;
    }
}

void CRYPT_PC_CreateKeys(CRYPT_SETUP* pc,unsigned long val)
{
    unsigned long esi,ebx,edi,eax,edx,var1;
    esi = 1;
    ebx = val;
    edi = 0x15;
    pc->keys[56] = ebx;
    pc->keys[55] = ebx;
    while (edi <= 0x46E)
    {
        eax = edi;
        var1 = eax / 55;
        edx = eax - (var1 * 55);
        ebx = ebx - esi;
        edi = edi + 0x15;
        pc->keys[edx] = esi;
        esi = ebx;
        ebx = pc->keys[edx];
    }
    CRYPT_PC_MixKeys(pc);
    CRYPT_PC_MixKeys(pc);
    CRYPT_PC_MixKeys(pc);
    CRYPT_PC_MixKeys(pc);
    pc->pc_posn = 56;
}

unsigned long CRYPT_PC_GetNextKey(CRYPT_SETUP* pc)
{    
    unsigned long re;
    if (pc->pc_posn == 56)
    {
        CRYPT_PC_MixKeys(pc);
        pc->pc_posn = 1;
    }
    re = pc->keys[pc->pc_posn];
    pc->pc_posn++;
    return re;
}

void CRYPT_PC_CryptData(CRYPT_SETUP* pc,void* data,unsigned long size)
{
    unsigned long x;
    for (x = 0; x < size; x += 4) *(unsigned long*)((unsigned long)data + x) ^= CRYPT_PC_GetNextKey(pc);
}
