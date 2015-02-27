#ifndef RNK_FILE_H
#define RNK_FILE_H

#include "array.h"
#include "format.h"
#include "linestream.h"

typedef struct {
  char* id;
  double value;
} NVPairStruct, *NVPair;

typedef Array NVPairArray;

NVPair newNVPair(const char *id, double value);
void destroyNVPair(NVPair pair);

void printNVPairArray(FILE *file,
		      const NVPairArray pairArray);
void printInvalidLines(FILE *file,
		       const Texta invalid);

int sortIDandAbsValue(const NVPair *p1, const NVPair *p2);
int sortValue(const NVPair *p1, const NVPair *p2);

void sortAndCollapse(NVPairArray pairArray);
void destroyNVPairArray(NVPairArray pairArray);

void parseRnkFile(const char* filename,
		  NVPairArray pairArray,
		  Texta invalid);


#endif

