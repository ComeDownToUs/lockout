#include <stdio.h>
#include <string.h>

#define CBC 1

#include "aes.h"
#include "sha256.h"
#include "debugging_tools.h"
#include "lockout.h"

static void menu(void);

//password string strength
int main(int argc, char** argv){
  uint8_t strength;//argv
  uint8_t copypass[16];//argv
  uint8_t plaintext[TEXTSIZE];//argv
  uint8_t ciphertext[TEXTSIZE];//
  uint8_t hashed[32];//[SHA256_BLOCK_SIZE];//password hashed
  uint8_t output[TEXTSIZE];//the returned value following brute force
  char stringer[15];
  int seed = time(NULL);
  srand(seed);

  //copying values to fixed unsigned char variables
  strcpy(copypass, argv[1]);
  strcpy(plaintext, argv[1]);
  strcat(plaintext, argv[2]);//salting plaintext with verifier
  strength = atoi(argv[3]);

  sha256_gen(copypass, hashed);

  lockout(hashed, plaintext, ciphertext, strength);

    printf("input cipher\n");
    print_array(ciphertext, TEXTSIZE);
  //just to hold place
    scanf("%s", &stringer);

  breakin(hashed, ciphertext, plaintext, output, copypass);//brute force application

  //printing test
    printf("original plaintext\n");
    print_array(plaintext, TEXTSIZE);
    printf("output cipher\n");
    print_array(ciphertext, TEXTSIZE);
    printf("output\n");
    print_array(output, TEXTSIZE);
  for(int i=0; i<argc; i++){
    printf("\n%s\n", argv[i]);
  }
}

static void menu(void){  
  char option[] = "choice";
  while( strcmp(option, "exit")!=0){ 
    printf("Please select an option\n");
    printf("1. encrypt\n2. decrypt\nSelect: ");
    scanf("%s", &option);
    if(strcmp(option, "1")==0){
      printf("Encryption\n");
    }
    else if(strcmp(option, "2")==0){
      printf("Decryption\n");
    }
  }
}



