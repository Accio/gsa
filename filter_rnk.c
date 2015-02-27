#include "format.h"
#include "linestream.h"
#include "math.h"

#define isvalid(x) (isfinite((x)) & !isnan((x)))

typedef struct {
  char* id;
  double value;
} PairStruct, *Pair;
typedef Array PairArray;

Pair newPair(const char* id, double value) {
  Pair p = (Pair)malloc(sizeof(PairStruct));
  p->id = hlr_strdup((char*)id);
  p->value=value;
  return(p);
}

void destroyPair(Pair pair) {
  if(pair) {
    pair->id=NULL;
    free(pair);
  }
}

void printPairList(const PairArray pairArray) {
  int i;
  Pair p;
  for(i=0;i<arrayMax(pairArray);i++) {
    p=array(pairArray, i, Pair);
    printf("%s\t%f\n", p->id, p->value);
  }
}

static int sortIDandAbsval(Pair *p1, Pair *p2) {
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

void sortAndCollapse(PairArray pairArray) {
  char* lastID;
  int i;
  Pair p;

  arraySort(pairArray, (ARRAYORDERF)sortIDandAbsval);

  lastID=arru(pairArray, 0, Pair)->id;
  
  for(i=1;i<arrayMax(pairArray); i++) {
    p=array(pairArray, i, Pair);
    if(strcmp(p->id, lastID)==0) {
      arrayRemoveD(pairArray, i--);
    } else {
      lastID=p->id;
    }
  }
}

void destroyPairArray(PairArray pairArray) {
  int i;
  Pair p;
  
  for(i=0;i<arrayMax(pairArray);i++) {
    destroyPair(array(pairArray, i, Pair));
  }

  arrayDestroy(pairArray);
}

int main(int argc, char* argv[]) {
  char* rnkFile = "withinvalid-diffstats.txt";
  char* line;
  
  char* statstr;
  char* nptr;
  double stat;

  LineStream ls = ls_createFromFile(rnkFile);
  Texta it;

  Array stats = arrayCreate(128, Pair);
  Array outstats=arrayCreate(128, Pair);
  Pair p;

  int i;
  
  while(line=ls_nextLine(ls)) {
    it = textStrtokP(line, "\t ");
    if(arrayMax(it)==2) {
      statstr=textItem(it, 1);
      double stat = strtod(statstr, &nptr);
      if(isvalid(stat) & statstr != nptr) {
	p = newPair(textItem(it, 0), stat);
	array(stats, arrayMax(stats), Pair)=p;
      } else {
	fprintf(stderr, "[Discarding line] %s\n", line);
      }
    }
  }

  if(arrayMax(stats)==0)
    die("No genes left");

  sortAndCollapse(stats);

  printPairList(stats);

  destroyPairArray(stats);
  ls_destroy(ls);
  textDestroy(it);
  return 0;
}
