/* PSO Encryption Library 
 * 
 * All included source written by Fuzziqer Software except where otherwise
 * indicated.  copyright 2004 
 */

#include "spp.h"

#define ZeroMemory(d,s) memset(d, 0, s)

bool CRYPT_CreateKeys(CRYPT_SETUP* cs,void* key,unsigned char type)
{
    ZeroMemory(cs,sizeof(CRYPT_SETUP));
    cs->type = type;
    switch (cs->type)
    {
      case CRYPT_PC:
        CRYPT_PC_CreateKeys(cs,*(unsigned long*)key);
        break;
      case CRYPT_GAMECUBE:
        CRYPT_GC_CreateKeys(cs,*(unsigned long*)key);
        break;
      case CRYPT_BLUEBURST:
        CRYPT_BB_CreateKeys(cs,key);
        break;
      default:
        return false;
    }
    return true;
}

bool CRYPT_CryptData(CRYPT_SETUP* cs,void* data,unsigned long size,bool encrypting)
{
    switch (cs->type)
    {
      case CRYPT_PC:
        CRYPT_PC_CryptData(cs,data,size);
        break;
      case CRYPT_GAMECUBE:
        CRYPT_GC_CryptData(cs,data,size);
        break;
      case CRYPT_BLUEBURST:
        if (encrypting) CRYPT_BB_Encrypt(cs,data,size);
        else CRYPT_BB_Decrypt(cs,data,size);
        break;
      default:
        return false;
    }
    return true;
}
