#ifndef LOCKOUT_H
#define LOCKOUT_H

#include <stdio.h>
#include <string.h>

#include "aes.h"
#include "sha256.h"
#include "debugging_tools.h"

#ifndef TEXTSIZE
#define TEXTSIZE 32 //512 is limit
#endif

int lockout(uint8_t* hash, uint8_t* plaintext, uint8_t* ciphertext, int strength);
int breakin(uint8_t* hash, uint8_t* ciphertext, uint8_t* plaintext, uint8_t* output, uint8_t* append_val, int strength);
int brute_force(uint8_t* plaintext, uint8_t* ciphertext, uint8_t* arr, char size, char current, uint8_t* output, uint8_t* append_val);
void password_gen(uint8_t* hash, uint8_t* key, int length, char strength);
void sha256_gen(uint8_t* input, uint8_t* output);
void char_to_uint8t(char* input, uint8_t* output, int length);

#endif