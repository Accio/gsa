#include "format.h"
#include "linestream.h"
#include "math.h"

#define isvalid(x) (isfinite((x)) & !isnan((x)))

int main(int argc, char* argv[]) {
  char* rnkFile = "withinvalid-diffstats.txt";
  char* line;
  
  char* statstr;
  char* nptr;
  double stat;

  LineStream ls = ls_createFromFile(rnkFile);
  Texta it;

  while(line=ls_nextLine(ls)) {
    it = textStrtok(line, "\t");
    if(arrayMax(it)==2) {
      statstr=textItem(it, 1);
      double stat = strtod(statstr, &nptr);
      if(isvalid(stat) & statstr != nptr)
	printf("%s\t%f\n", textItem(it, 0), stat);
    }
  }

  ls_destroy(ls);
  textDestroy(it);
  return 0;
}
