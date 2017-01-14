#include <stdio.h>
#include <string.h>

#define CBC 1

#include "aes.h"
#include "sha256.h"
#include "debugging_tools.h"
#include "lockout.h"
#include "docopt.h"

static void menu(void);
static int strength_check(char* algorithm, int strength);
static void file_io(char* filename, uint8_t* text, int filesize, char readorwrite);


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

  DocoptArgs args = docopt(argc, argv, /* help */ 1, /* version */ "2.0rc2");

  printf("\n\ncipher %s \n strength %d \n decode %d \n encode %d \n write %s \n salt %s \n password %s\n read %s \n inpstr %s\n cmd %d \n",
    args.ciphertextdir, args.strength, args.decode, args.encode, args.write, args.salt, args.password, args.read, args.inputstring, args.commandline
    );

  //copying values to fixed unsigned char variables
  strcpy(copypass, argv[1]);
  strcpy(plaintext, argv[1]);
  strcat(plaintext, argv[2]);//salting plaintext with verifier
  strength = atoi(argv[3]);

  if(strength_check("AES128", strength)){
    return 0;
  }

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
    file_io("cipher.txt", ciphertext, TEXTSIZE, 'w');
    file_io("output.txt", output, TEXTSIZE, 'w');
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

static void file_io(char* filename, uint8_t* text, int filesize, char readorwrite){
  FILE* fp;
  if(readorwrite == 'r' || readorwrite == 'w'){
    if(readorwrite=='r'){
      fp = fopen(filename, "r");
      fread(text, sizeof(uint8_t), filesize, fp);
    }else if(readorwrite=='w'){ 
      fp = fopen(filename, "w");
      fprintf(fp, "%s", text);//fprintf removes junk at end
    }
    fclose(fp);
  }else{
    printf("invalid file process\n");
  }
}

static int strength_check(char* algorithm, int strength){
  int alg_str = 0;
  printf("(does it run)\n");
  if(strcmp(algorithm, "AES128")==0){
    alg_str = 128;
  }

  if (strength > (alg_str-1) || strength < 1){
    printf(
      "this program uses %s, therefore the value must be between 1 and %d (we suggest %d)",
      algorithm, alg_str-1, alg_str-2);
    return 1;
  }

  return 0;
}
