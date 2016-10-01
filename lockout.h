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

void lockout(uint8_t* hash, uint8_t* plaintext, uint8_t* ciphertext, uint8_t strength);
void breakin(uint8_t* hash, uint8_t* ciphertext, uint8_t* plaintext, uint8_t* output, uint8_t* append_val);
int brute_force(uint8_t* plaintext, uint8_t* ciphertext, uint8_t* arr, uint8_t size, uint8_t current, uint8_t* output, uint8_t* append_val);
void password_gen(uint8_t* hash, uint8_t* key, uint8_t length, uint8_t strength);
void sha256_gen(uint8_t* input, uint8_t* output);

#endif