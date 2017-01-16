#include <stdio.h>
#include <string.h>


#define CBC 1

#define ARGS_MAXLEN 20

#include "aes.h"
#include "sha256.h"
#include "debugging_tools.h"
#include "lockout.h"
#include "docopt.h"

static int strength_check(char* algorithm, int strength);
static void file_io(char* filename, char* text, int filesize, char readorwrite);
static int validator(DocoptArgs args, char process);


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
  char argvalidator = 0;

  DocoptArgs args = docopt(argc, argv, /* help */ 1, /* version */ "2.0rc2");

  printf("\n\ncipher %s \n strength %d \n decode %d \n encode %d \n write %s \n salt %s \n \
    password %s\n read %s \n inpstr %s\n cmd %d \n",
    args.ciphertextdir, args.strength, args.decode, args.encode, args.write, args.salt, 
    args.password, args.read, args.inputstring, args.commandline
    );

  if (!strcmp(argv[2], "test")){
    return 0;
    // do the test stuff with prints and all
  } 
  else if (args.encode == 1){
  // //basic argument validator process
    // //currently not working
    // if(!args.password)
    //   argvalidator+=1;
    // if(!args.salt)
    //   argvalidator+=1;
    // if(!args.write)
    //   argvalidator+=1;
    // if(!args.read)
    //   argvalidator+=1;

    // if(argc > (4 + argvalidator )){
    //   printf("Unspecified additional arguments listed\n");
    //   return 1;
    // }

    if( validator(args, 'e') == 0){
      printf("Succesful process");
      return 0;
    }else{
      printf("Unsuccessful");
      return 1;
    }
  } 
  else if (args.decode == 1){

    if( validator(args, 'd') == 0){
      printf("Succesful process");
      return 0;
    }else{
      printf("Unsuccessful");
      return 1;
    }
  } 
  else if ( args.commandline == 1 ){
    printf("--commandline will likely be removed");
    //implement later
    return 1;
  }

//
  // //copying values to fixed unsigned char variables
  // strcpy(copypass, argv[1]);
  // strcpy(plaintext, argv[1]);
  // strcat(plaintext, argv[2]);//salting plaintext with verifier
  // strength = atoi(argv[3]);

  // if(strength_check("AES128", strength)){
  //   return 0;
  // }

  // sha256_gen(copypass, hashed);

  // lockout(hashed, plaintext, ciphertext, strength);

  //   printf("input cipher\n");
  //   print_array(ciphertext, TEXTSIZE);
  // //just to hold place
  //   scanf("%s", &stringer);

  // breakin(hashed, ciphertext, plaintext, output, copypass);//brute force application

  // //printing test
  //   printf("original plaintext\n");
  //   print_array(plaintext, TEXTSIZE);
  //   printf("output cipher\n");
  //   print_array(ciphertext, TEXTSIZE);
  //   printf("output\n");
  //   print_array(output, TEXTSIZE);
  //   file_io("cipher.txt", ciphertext, TEXTSIZE, 'w');
  //   file_io("output.txt", output, TEXTSIZE, 'w');
  // for(int i=0; i<argc; i++){
  //   printf("\n%s\n", argv[i]);
  // }
  return 0;

}


//validator 
static int validator(DocoptArgs args, char process){
  char option;
  char plaintext[TEXTSIZE];
  char ciphertext[TEXTSIZE];
  char* out;
  char pass1[ARGS_MAXLEN];
  char pass2[ARGS_MAXLEN];
  char* hashed;

  uint8_t _password[ARGS_MAXLEN];
  uint8_t _salt[ARGS_MAXLEN];
  uint8_t _cipher[ARGS_MAXLEN];
  uint8_t _plain[ARGS_MAXLEN];
  uint8_t _hash[ARGS_MAXLEN];

  //if no strength, check for value,
  if(args.strength<125 || args.strength > 127){
    // verify strength
    printf("%d Please enter a value for strength: ", args.strength);
    scanf("%d", &(args.strength) );
    // if encoding, verify again
  }
  printf("1");
  if(args.salt == NULL){
    args.salt = malloc(sizeof(char)* ARGS_MAXLEN);
    if(process == 'e'){
      printf("Do you want to use a custom salt (if not your password will be used)? (y/n)");
      scanf("%c", &option);
      fflush(stdin);

      if(toupper(option) == 'Y'){
        printf("Please enter the salt? (y/n)");
        scanf("%s", _salt);
        fflush(stdin);
        //verify
      }
    }
    else if(process == 'd'){
      printf("Was a custom salter used when encoding? (y/n)");
      scanf(" %c", &option);
      fflush(stdin);

      if(toupper(option) == 'Y'){
        printf("Please enter the salt? (y/n)");
        scanf("%s", _salt);
        fflush(stdin);
        //verify
      }
    }
  }
  printf("2");
  if(args.read == NULL){
    //if no input, check for value,
    args.read =  malloc(sizeof(char)* ARGS_MAXLEN);

    // if encoding, offer inline?
    if(process == 'e'){
      printf("Would you like to enter the data to be encrypted here instead of providing a filename ? (y/n)");
      scanf(" %c", &option);
      if(toupper(option) == 'Y'){
        printf("Please enter what you want to be encrypted: ");
        scanf("%s", _plain);
      }
    }
    if ( process == 'd' || (process == 'e' && (toupper(option) == 'N')) ){
      // enter filename
      printf("Please enter a filename:");
      scanf("%s", args.read);
      fflush(stdin);
      //- check if valid
      //- read from file
      //- get text
    }  
  }
  printf("3");
  if(args.write == NULL){
    //if no output, check for value,
    args.write =  malloc(sizeof(char)* ARGS_MAXLEN);

    // if encoding, offer inline?
    printf("Do you want the output to be written to a file? (y/n)");
    scanf(" %c", &option);
    fflush(stdin);

    if(toupper(option) == 'Y'){
      printf("Please enter a filename:");
      printf("3.1");
      scanf("%s", args.write);
      printf("3.2");
      fflush(stdin);
      printf("3.3");
    }
  }
  printf("4");
  //if no password, check for value, verify
  if(args.password == NULL){
    args.password =  malloc(sizeof(char)* ARGS_MAXLEN);
    do{
      printf("Please enter a password:");
      scanf("%s", pass1);
      printf("Please verify password:");
      scanf("%s", pass2);

      if(strcmp(pass1, pass2))
        printf("passwords do not match");
    }while(strcmp(pass1, pass2));

    strcpy(args.password, pass1);
  }
  printf("5\n");
  //list all selections
  //read in cipher

  printf("6\n");
  char_to_uint8t(args.password, _password, strlen(args.password));

  printf("7\n");
  if(process == 'e'){
    sha256_gen(_password, hashed);
    printf("7.2\n");
    if (lockout(hashed, /*include salt in pt*/ plaintext, ciphertext, args.strength) == 0)
      return 0;
    else
      return 1;
  }
  //elif decode
  if(process == 'd'){
    sha256_gen(args.password, hashed);
    if( breakin(hashed, ciphertext, plaintext, out, args.salt, args.strength) == 0 )
      return 0;
    else
      return 1;
  }
  return 1;
}


static void file_io(char* filename, char* text, int filesize, char readorwrite){
  FILE* fp;
  if(readorwrite == 'r' || readorwrite == 'w'){
    if(readorwrite=='r'){
      fp = fopen(filename, "r");
      fread(text, sizeof(char), filesize, fp);
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
