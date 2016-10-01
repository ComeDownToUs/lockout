#include <stdio.h>
#include <string.h>

#define CBC 1
#define TEXTSIZE 1024 //512 is limit

#include "aes.h"
#include "sha256.h"

static void phex(uint8_t* str);
int brute_force(uint8_t* plaintext, uint8_t* ciphertext, uint8_t* arr, uint8_t size, uint8_t current, uint8_t* output);
static void print_array(uint8_t* arr, int size);
static void print_charray(char* arr, int size);
static void menu(void);
static void lockout(uint8_t* hash, uint8_t* plaintext, uint8_t* ciphertext, uint8_t strength);
static void breakin(uint8_t* hash, uint8_t* ciphertext, uint8_t* plaintext, uint8_t* output);
static void key_gen(uint8_t* hash, uint8_t* key, uint8_t length, uint8_t strength);
static void sha256_gen(uint8_t* input, uint8_t* output);

//password string strength
int main(int argc, char** argv){
  uint8_t strength;//argv
  uint8_t copypass[16];//argv
  uint8_t plaintext[TEXTSIZE];//argv
  uint8_t ciphertext[TEXTSIZE];//
  uint8_t hashed[SHA256_BLOCK_SIZE];//password hashed
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

  breakin(hashed, ciphertext, plaintext, output);//brute force application

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

static void sha256_gen(uint8_t* input, uint8_t* output){
  SHA256_CTX ctx;
  sha256_init(&ctx);
  sha256_update(&ctx, input, strlen(input));
  sha256_final(&ctx, output);
  //buffer is hash
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

static void password_gen(uint8_t* hash, uint8_t* password, uint8_t length, uint8_t strength){
  for(int i=length-1; i>=0; i--){
    if(i>=strength)
      password[i]=rand()%256;
    else
      password[i]=hash[i];
  }
  print_array(hash, length);
  print_array(password, length);
}

static void lockout(uint8_t* hash, uint8_t* plaintext, uint8_t* ciphertext, uint8_t strength){
  uint8_t* key[16];
  uint8_t* plaincopy[TEXTSIZE];
  uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };//
  password_gen(hash, key, 16, strength);

  memcpy(plaincopy, plaintext, TEXTSIZE);

  AES128_CBC_encrypt_buffer(ciphertext, plaincopy, TEXTSIZE, key, iv);//
  printf("PostEncrypt: \n");
  print_charray(ciphertext, TEXTSIZE);
}

static void breakin(uint8_t* hash, uint8_t* ciphertext, uint8_t* plaintext, uint8_t* output){
  if(brute_force(plaintext, ciphertext, hash, 16, 0, output)==15)
    printf("SUCCESS\n");
}

int brute_force(uint8_t* plaintext, uint8_t* ciphertext, uint8_t* arr, uint8_t size, uint8_t current, uint8_t* output){
  uint8_t* buffer[TEXTSIZE];
  uint8_t result=0;
  uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };//
  
  char deets[10];//<<FLAG>> LENGTH NEEDS TO BE DYNAMIC

  for(int i=0; i<=255;i++){
    AES128_CBC_decrypt_buffer(buffer, ciphertext, TEXTSIZE, arr, iv);
    printf("\n");
    memcpy(deets, buffer, 9);
    deets[8]='\0';
    printf("%s", deets);

    if(strcmp(deets, "password")==0){
      printf("WOWZER\n");
      strcpy(output, buffer);
      return 15;
    }
    if(current!=size-1){
      if(brute_force(plaintext, ciphertext, arr, size, current+1, output)==15){
        return 15;
      }
    }
    arr[current]=i;
    print_array(arr, size);
  } 
  arr[current-1]=0;
  return 0;
}


static void print_array(uint8_t* arr, int size){
  for(int i = 0; i<size; i++){
    printf("%3d,", arr[i]);
  }
  printf("\n");
}
static void print_charray(char* arr, int size){
  for(int i = 0; i<size; i++){
    printf("%c", arr[i]);
  }
  printf("\n");
}
// prints string as hex
static void phex(uint8_t* str)//
{
    unsigned char i;
    for(i = 0; i < 16; ++i)
        printf("%.2x", str[i]);
}


