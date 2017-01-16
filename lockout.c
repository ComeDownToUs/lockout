#include <stdio.h>
#include <string.h>
#include <math.h>

#include "aes.h"
#include "sha256.h"
#include "lockout.h"
#include "debugging_tools.h"


int lockout(uint8_t* hash, uint8_t* plaintext, uint8_t* ciphertext, int strength){
  uint8_t* key[16];
  uint8_t* plaincopy[TEXTSIZE];
  uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };//
  password_gen(hash, (char*)key, 16, strength);

  memcpy(plaincopy, plaintext, TEXTSIZE);

  AES128_CBC_encrypt_buffer( (uint8_t*) ciphertext, (uint8_t*) plaincopy, TEXTSIZE, (uint8_t*) key, (uint8_t*) iv);//
  printf("PostEncrypt: \n");
  print_charray((uint8_t*) ciphertext, TEXTSIZE);
  return 0;
}

int breakin(uint8_t* hash, uint8_t* ciphertext, uint8_t* plaintext, uint8_t* output, uint8_t* append_val, int strength){
  if(brute_force(plaintext, ciphertext, hash, 16, 0, output, append_val)==15){
    printf("SUCCESS\n");
    return 0;
  }
  return 1;
}

int brute_force(uint8_t* plaintext, uint8_t* ciphertext, uint8_t* arr, char size, char current, uint8_t* output, uint8_t* append_val){
  uint8_t* buffer[TEXTSIZE];
  char result=0;
  uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };//
  
  char deets[10];//<<FLAG>> LENGTH NEEDS TO BE DYNAMIC

  for(int i=0; i<=255;i++){
    AES128_CBC_decrypt_buffer( (uint8_t*) buffer, (uint8_t*) ciphertext, TEXTSIZE, (uint8_t*) arr, iv);
    printf("\n");
    memcpy(deets, buffer, 9);
    deets[8]='\0';
    if(strcmp(deets, (char*) append_val)==0){
      printf("WOWZER\n");
      strcpy((char*) output, (char*) buffer);
      printf("%10s | ", deets);
      print_array(arr, size);
      return 15;
    }
    if(current!=size-1){
      if(brute_force(plaintext, ciphertext, arr, size, current+1, output, append_val)==15){
        return 15;
      }
    }
    printf("%10s | ", deets);
    print_array(arr, size);
    arr[current]=i;
  } 
  arr[current-1]=0;
  return 0;
}

void sha256_gen(uint8_t* input, uint8_t* output){
  SHA256_CTX ctx;
  sha256_init(&ctx);
  sha256_update(&ctx, (uint8_t*) input, strlen((uint8_t*)input));
  sha256_final(&ctx, (uint8_t*) output);
  //buffer is hash
}

void password_gen(uint8_t* hash, uint8_t* password1, int length, char strength){
  char strength_in_bytes = (char)strength/8;
  uint8_t* password;
  char remainder_range = (strength ==0) ? 0 : 8 - strength%8;
  printf("strength_in_bytes: %d\n", strength_in_bytes);
  password = (uint8_t*)password1;

  printf("remainder range: %d\n", remainder_range);
  printf("max value of index %d should be %d\n", strength_in_bytes+1, ((int)pow(2, remainder_range )-1));

  for(int i=length-1; i>=0; i--){
    if(i>strength_in_bytes)
      password[i]=rand()%256;
    else if(i<strength_in_bytes)
      password[i]=hash[i];
    else 
      password[i]=rand()%( (int)pow(2,remainder_range) );
  }
  print_array(hash, length);
  print_array(password, length);
}

// means to convert chars to unsigned fixed integers for encryption
// without having to change types across huge portions of code both ways
void char_to_uint8t(char* input, uint8_t* output, int length){
  for(int i=0; i<length; i++){
    output[i]=input[1];
    printf("%c%d => %c%d\n", input[i], input[i], output[i], output[i]);
  }
}
