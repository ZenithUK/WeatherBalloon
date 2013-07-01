#include "Utils.h"

unsigned int millis (void)
{
	struct timeval tv ;
	uint64_t now ;

	gettimeofday (&tv, NULL) ;
	now  = (uint64_t)tv.tv_sec * (uint64_t)1000 + (uint64_t)(tv.tv_usec / 1000) ;

	return (uint32_t)(now - epochMilli) ;
}

void initByteArray(ByteArray *a, size_t initialSize) {
  a->array = (int *)malloc(initialSize * sizeof(char));
  a->used = 0;
  a->size = initialSize;
}

void insertByteArray(ByteArray *a, char element) {
  if (a->used == a->size) {
    a->size += 1;
    a->array = (char *)realloc(a->array, a->size);
  }
  a->array[a->used++] = element;
}

void appendByteArray(ByteArray *a, char** string) {
	for (int i=0; i<strlen(*string); i++){
		insertByteArray(a, (*string)[i]);
	}
}

int byteArrayToChar(ByteArray *a, char** data, int startOffset, int endOffset) {

	//assert(data);
	//printf("f1\n");
	int size = a->used-endOffset-startOffset+1;
	//printf("f2\n");
	if (size > 0){
		//printf("f3\n");
    		*data = malloc(size);

		//segfault before
		//printf("f4\n");

		//printf("!\n");
		for (int i=0; i<a->used-endOffset-startOffset; i++){
			(*data)[i] = a->array[i+startOffset];
		}
		//printf("£\n");
		(*data)[a->used-endOffset-startOffset] = '\0';
		//printf("$\n");

		return 1;
	}else{
		return 0;
	}
}

int readArrayData(ByteArray *a, char** output, int offset, char split) {
	int i = offset;
	while(i+1 < a->used && a->array[i] != split){
		i++;
	}
	int size = i-offset;
	if (size  > 0 && i != offset){
		assert(output);
    		(*output) = malloc(size+1);

		//printf("Size %i \n", size);

		for (int p=0; p<size; p++){
			(*output)[p] = a->array[offset+p];
		}
		(*output)[size] = '\0';
		
		return offset+size+1;
	}else{
		(*output) = NULL;
		return offset+1;
	}
}

void freeByteArray(ByteArray *a) {
	if (a->array != NULL){
  		free(a->array);
  		a->array = NULL;
	}
  	a->used = a->size = 0;
}
