#include <stdio.h>
#include <stdint.h>
#include "debugging_tools.h"

void print_array(uint8_t* arr, int size){
  for(int i = 0; i<size; i++){
    printf("%3d,", arr[i]);
  }
  printf("\n");
}
void print_charray(char* arr, int size){
  for(int i = 0; i<size; i++){
    printf("%c", arr[i]);
  }
  printf("\n");
}