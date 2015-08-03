#ifndef _ENCRYPTION_H_
#define _ENCRYPTION_H_

/* PSO Encryption Library 
 * 
 * All included source written by Fuzziqer Software except where otherwise
 * indicated.  copyright 2004 
 */

#include "spp.h"

#define CRYPT_SETUP_SIZE (sizeof(CRYPT_SETUP))

// Supported encryption types 
#define CRYPT_GAMECUBE   0 // 521-key encryption used in PSOGC and PSOX 
#define CRYPT_BLUEBURST  1 // 1042-key encryption used in PSOBB 
#define CRYPT_PC         2 // 56-key encryption used in PSODC and PSOPC 

// Encryption data struct 
typedef struct {
    unsigned long type; // what kind of encryption is this? 
    unsigned long keys[1042]; // encryption stream 
    unsigned long pc_posn; // PSOPC crypt position 
    unsigned long* gc_block_ptr; // PSOGC crypt position 
    unsigned long* gc_block_end_ptr; // PSOGC key end pointer 
    unsigned long gc_seed; // PSOGC seed used 
    unsigned long bb_posn; // BB position (not used) 
    unsigned long bb_seed[12]; // BB seed used 
} CRYPT_SETUP;

// Internal functions (don't call these) 
unsigned long CRYPT_PC_GetNextKey(CRYPT_SETUP*);
void CRYPT_PC_MixKeys(CRYPT_SETUP*);
void CRYPT_PC_CreateKeys(CRYPT_SETUP*,unsigned long);
void CRYPT_PC_CryptData(CRYPT_SETUP*,void*,unsigned long);

unsigned long CRYPT_GC_GetNextKey(CRYPT_SETUP*);
void CRYPT_GC_MixKeys(CRYPT_SETUP*);
void CRYPT_GC_CreateKeys(CRYPT_SETUP*,unsigned long);
void CRYPT_GC_CryptData(CRYPT_SETUP*,void*,unsigned long);

void CRYPT_BB_Decrypt(CRYPT_SETUP*,void*,unsigned long);
void CRYPT_BB_Encrypt(CRYPT_SETUP*,void*,unsigned long);
void CRYPT_BB_CreateKeys(CRYPT_SETUP*,void*);

// External functions 
bool CRYPT_CreateKeys(CRYPT_SETUP* cs,void* key,unsigned char type);
bool CRYPT_CryptData(CRYPT_SETUP* cs,void* data,unsigned long size,bool encrypting);

//void CRYPT_PrintData(void* ds,unsigned long data_size);

/* void CRYPT_CreateKeys(CRYPT_SETUP* cs,void* key,unsigned char type)
 * 
 *   Initalizes a CRYPT_SETUP to be used to encrypt and decrypt data. 
 * 
 *   Arguments: 
 * 
 *     CRYPT_SETUP* cs 
 *         Pointer to the CRYPT_SETUP structure to prepare. 
 * 
 *     void* key 
 *         Pointer to the encryption key. For CRYPT_PC and CRYPT_GAMECUBE, this
 *         should point to a single 32-bit value (an unsigned long). For 
 *         CRYPT_BLUEBURST, this should point to a 48-byte array to be used as
 *         the key. 
 * 
 *     unsigned char type 
 *         Defines the type of encryption to use. Valid types: CRYPT_GAMECUBE, 
 *         CRYPT_BLUEBURST, and CRYPT_GAMECUBE. 
 * 
 *   Return value:
 *     The function returns TRUE if the operation succeeded, or FALSE if an
 *     invalid encryption type was given. 
 */

/* bool CRYPT_CryptData(CRYPT_SETUP* cs,void* data,unsigned long size,
 *                      bool encrypting)
 * 
 *   Encrypts or decrypts data. 
 * 
 *   Arguments: 
 * 
 *     CRYPT_SETUP* cs 
 *         Pointer to the CRYPT_SETUP structure to use. 
 * 
 *     void* data 
 *         Pointer to the data to be processed. 
 * 
 *     unsigned long size 
 *         Size of the data to be processed. 
 * 
 *     bool encrypting 
 *         TRUE if the data is to be encrypted, FALSE if it is to be decrypted. 
 *         Ignored unless the type of the given CRYPT_SETUP is CRYPT_BLUEBURST. 
 * 
 *   Return value:
 *     The function returns TRUE if the operation succeeded, or FALSE if an
 *     invalid encryption type was given. 
 */

/* void CRYPT_PrintData(void* ds,unsigned long data_size)
 * 
 *   Prints a segment of raw data to the console usinf printf, both as
 *   hexadecimal and ASCII. 
 * 
 *   Arguments: 
 * 
 *     void* ds 
 *         Pointer to the data to be printed. 
 * 
 *     unsigned long data_size 
 *         Size of the data to be printed. 
 * 
 *   Return value: none 
 */

#endif /* _ENCRYPTION_H_ */
