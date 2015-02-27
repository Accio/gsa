#include <math.h>

#include "rnk_file.h"

//#ifdef __USE_ISOC99
#define isvalid(x) (isfinite((x)) & !isnan((x)))
//#else
//#define isvalid(x) (!isnan((x)))
//#endif


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

void printNVPairArray(FILE* file,
		      const NVPairArray pairArray) {
  int i;
  NVPair p;
  for(i=0;i<arrayMax(pairArray);i++) {
    p=array(pairArray, i, NVPair);
    fprintf(file, "%s\t%f\n", p->id, p->value);
  }
}

void printInvalidLines(FILE* file,
		       const Texta invalid) {
  int i;
  if(arrayMax(invalid)>0 & file!=NULL) {
    for(i=0; i<arrayMax(invalid); ++i) {
      fprintf(file,
	      "[Discarded invalid line] %s\n",
	      textItem(invalid, i));
    }
  }
}


int sortIDandAbsValue(const NVPair *p1, const NVPair *p2) {
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

int sortValue(const NVPair *p1, const NVPair *p2) {
  return((*p1)->value < (*p2)->value);
}

void sortAndCollapse(NVPairArray pairArray) {
  char* lastID;
  int i;
  NVPair p;

  arraySort(pairArray, (ARRAYORDERF)sortIDandAbsValue);

  lastID=arru(pairArray, 0, NVPair)->id;
  
  for(i=1;i<arrayMax(pairArray); i++) {
    p=array(pairArray, i, NVPair);
    if(strcmp(p->id, lastID)==0) {
      arrayRemoveD(pairArray, i--);
    } else {
      lastID=p->id;
    }
  }

  arraySort(pairArray, (ARRAYORDERF)sortValue);
}

void destroyNVPairArray(NVPairArray pairArray) {
  int i;
  
  for(i=0;i<arrayMax(pairArray);i++) {
    destroyNVPair(array(pairArray, i, NVPair));
  }

  arrayDestroy(pairArray);
}

void parseRnkStream(const LineStream ls, 
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
  parseRnkStream(ls, pairArray, invalid);
  ls_destroy(ls);
}
