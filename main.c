#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <math.h>
#include "index.h"
#include <pthread.h>

// Global variables
pthread_mutex_t producerLock;
char ** producerBuffer;
// Keeps track of the number of items in the producerBuffer
// Useful for consumer to see if there are any available items
int producerItems = 0;

// Functions
void printErrorMessage();
void addToBuffer(char * listOfFileNames);
char* removeFromBuffer();
void indexer(char * filename);

// Main program where right now the producer is called
int main(int argc, char* argv []){
  // Check to make sure the number of args is right
  if (argc != 3){
    printErrorMessage();    
  } 
  //int numberOfThreads = (int)strtol(argv[1], NULL, 10);
  char* listOfFileNames = argv[2];
  // All of the following code is determine the number of filenames in the file
  FILE * counterFile;
  counterFile = fopen(listOfFileNames, "r");
  int numberOfLines = 0;
  char buffer[512];
  int buffer_len = 512;
  while (fgets(buffer, buffer_len, counterFile) != NULL){
    numberOfLines++;
  }
  fclose(counterFile);
  
  // Malloc the producerBuffer for future use
  producerBuffer = malloc(numberOfLines*sizeof(char*));
  addToBuffer(listOfFileNames);
  //indexer(blah);
  exit(0);
}

// This function adds a filename from the list of filenames to the 
// buffer of filenames that will be consumed by the indexer
void addToBuffer(char * listOfFileNames){
  // open file and initialize a buffer to read into
  FILE *fd;
  fd = fopen(listOfFileNames, "r");
  char buffer[512];
  // fgets into buffer until end of file is reached
  while (fgets(buffer, sizeof(buffer), fd) != NULL){
    int length = strlen(buffer);
    // replace the final character in the string with a null character
    // if it was not already
    if (buffer[length - 1] == '\n'){
      buffer[length - 1] = '\0';
    }
    // put the buffer into the producerBuffer
    producerBuffer[producerItems] = strdup(buffer);
    producerItems++;
  }
  fclose(fd);
}

// Producer
char* removeFromBuffer(){
  // Decrement number of producerItems
  producerItems--;
  // Return the filename that will be used by the indexer
  return producerBuffer[producerItems];
}

// Print an error message if something is wrong
void printErrorMessage(){
  fprintf(stderr, "Error");
  exit(1);  
}

// Consumer (using code from project description)
/*
void indexer(char * filename){
  FILE * file;
  file = fopen(filename, "r");
  char buffer[512];
  int buffer_len = 512;
  while(!feof(file)){
   int line_number = 0;
   char * word;
   char * saveptr;
   fgets(buffer, buffer_len, file);
   word = strtok_r(buffer, " \n\t", &saveptr);
   while (word != NULL){
    insert_into_index(word, filename, line_number);
    word = strtok_r(NULL, " \n\t", &saveptr);
   }
   line_number++;
  }
  fclose(file);
}
*/

