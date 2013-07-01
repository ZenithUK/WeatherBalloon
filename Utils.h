#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <assert.h>
#include <sys/time.h>
 
typedef struct {
  char *array;
  size_t used;
  size_t size;
} ByteArray;

uint64_t epochMilli, epochMicro;

unsigned int millis (void);

void initByteArray(ByteArray *a, size_t initialSize);

void insertByteArray(ByteArray *a, char element);

void appendByteArray(ByteArray *a, char** string);

int byteArrayToChar(ByteArray *a, char** data, int startOffset, int endOffset);

int readArrayData(ByteArray *a, char** output, int offset, char split);

void freeByteArray(ByteArray *a);

#endif
