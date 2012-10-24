#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "persistence.h"

#define MAX_SIZE 1024

int readfile(char * filename, char ** str){
  FILE * f = fopen(filename, "rb");
  long filesize;
  long result;
  struct stat st;

  fstat(fileno(f), &st);
  filesize = st.st_size;

  *str = (char *) malloc(sizeof(char) * filesize);
  if (str == NULL){
    perror("Malloc");
    return -1;
  }

  result = fread(*str, sizeof(char), filesize, f);
  if (result != filesize){
    perror("Fread error");
    return -1;
  }
  fclose(f);
}

int writefile(char * filename, char * buf, int len){
  FILE * f = fopen(filename, "wb");
  int result;
  result = fwrite(buf, sizeof(char), len, f);
  if (result < len){
    perror("Fwrite error");
    return -1;
  }
  fclose(f);
}

int main_test(){
  char * res;
  writefile("test.bin", "This is a \n string", strlen("This is a \n string"));
  readfile("test.bin", &res);
  printf("STRING \n%s", res);
  free(res);
  return 0;
}
