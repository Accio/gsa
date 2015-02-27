#include "format.h"
#include "linestream.h"
#include "math.h"

#define isvalid(x) (isfinite((x)) & !isnan((x)))

typedef struct {
  char* id;
  double value;
} NVPairStruct, *NVPair;
typedef Array NVPairArray;

NVPair newNVPair(const char* id, double value) {
  NVPair p = (NVPair)malloc(sizeof(NVPairStruct));
  p->id = hlr_strdup((char*)id);
  p->value=value;
  return(p);
}

void destroyNVPair(NVPair pair) {
  if(pair) {
    pair->id=NULL;
    free(pair);
  }
}

void printNVPairArray(const NVPairArray pairArray) {
  int i;
  NVPair p;
  for(i=0;i<arrayMax(pairArray);i++) {
    p=array(pairArray, i, NVPair);
    printf("%s\t%f\n", p->id, p->value);
  }
}

static int sortIDandAbsval(NVPair *p1, NVPair *p2) {
  int compid;
  int compabsval;
  
  compid=strcmp((*p1)->id, (*p2)->id);
  if(compid!=0) {
    return(compid);
  } else {
    compabsval = abs((*p1)->value)<abs((*p2)->value);
    return(compabsval);
  }
}

void sortAndCollapse(NVPairArray pairArray) {
  char* lastID;
  int i;
  NVPair p;

  arraySort(pairArray, (ARRAYORDERF)sortIDandAbsval);

  lastID=arru(pairArray, 0, NVPair)->id;
  
  for(i=1;i<arrayMax(pairArray); i++) {
    p=array(pairArray, i, NVPair);
    if(strcmp(p->id, lastID)==0) {
      arrayRemoveD(pairArray, i--);
    } else {
      lastID=p->id;
    }
  }
}

void destroyNVPairArray(NVPairArray pairArray) {
  int i;
  NVPair p;
  
  for(i=0;i<arrayMax(pairArray);i++) {
    destroyNVPair(array(pairArray, i, NVPair));
  }

  arrayDestroy(pairArray);
}

void parseRnk(const LineStream ls, 
	      NVPairArray pairArray,
	      Texta invalid) {
  char* line;
  Texta it;
  char* statstr;
  double stat;
  char* nptr;
  NVPair p;

  while(line=ls_nextLine(ls)) {
    it = textStrtokP(line, "\t ");
    if(arrayMax(it)==2) {
      statstr=textItem(it, 1);
      stat = strtod(statstr, &nptr);
      if(isvalid(stat) & statstr != nptr) {
	p = newNVPair(textItem(it, 0), stat);
	array(pairArray, arrayMax(pairArray), NVPair)=p;
      } else {
	if(invalid!=NULL) {
	  textAdd(invalid, line);
	}
      }
    }
  }

  textDestroy(it);
}

void parseRnkFile(const char* filename,
		  NVPairArray pairArray,
		  Texta invalid) {
  LineStream ls;

  ls = ls_createFromFile((char*)filename);
  parseRnk(ls, pairArray, invalid);
  ls_destroy(ls);
}

int main(int argc, char* argv[]) {
  char* rnkFile = "withinvalid-diffstats.txt";
  Array stats = arrayCreate(128, NVPair);
  Texta invalid = textCreate(128);

  parseRnkFile(rnkFile, stats, invalid);

  if(arrayMax(invalid)>0) {
    int i=0;
    for(i=0;i<arrayMax(invalid);++i)
      fprintf(stderr, "[Discarded invalid line]:%s\n", textItem(invalid, i));
  }
  
  if(arrayMax(stats)==0)
    die("No genes left");
  
  sortAndCollapse(stats);

  printNVPairArray(stats);

  destroyNVPairArray(stats);
  
  textDestroy(invalid);
  return 0;
}
