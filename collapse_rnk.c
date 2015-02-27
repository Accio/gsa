#include "rnk_file.h"

#include "log.h"
#include "arg.h"
#include "rofutil.h"

static void printUsage(int level){
  /*short manual on gsea2tab usage*/
  printf("Usage: %s [-out FILE/-] [-invalid INVALID_FILE/-] -in RNKFILE\n"
	 "Clean and collapse rnk files by the maximum-absolute-statistic\n"
	 "  -in: a rnk file: two-column file, with identifiers in the first and statistics in the second column\n"
	 "Optional parameters:\n"
	 "  -out: output file with collapsed name-value pairs. If set to '-', output will be writtin to stdout\n"
	 "  -invalid: output file with invalid lines. Written to stderr if set to '-'. Silent if not set.\n"
	 "\n"
	 "Contact: zhangj83\n",
	 arg_getProgName());

}

int main(int argc, char* argv[]) {
  char* inFileName;
  FILE* outFile=stdout;
  FILE* invalidFile=NULL;
  Texta invalidLines;
  
  arg_init(argc, argv, "out,1 invalid,1", "in", &printUsage);

  inFileName = hlr_strdup(arg_get("in"));
  if(arg_present("out")) {
    outFile=hlr_fopenWrite(hlr_strdup(arg_get("out")));
  }

  if(arg_present("invalid")) {
    char* invalidFileName=hlr_strdup(arg_get("invalid"));
    if(strcmp(invalidFileName, "-")!=0) {
      invalidFile=hlr_fopenWrite(invalidFileName);
    } else {
      invalidFile=stderr;
    }
  }

  NVPairArray stats = arrayCreate(128, NVPair);
  invalidLines = textCreate(128);

  parseRnkFile(inFileName, stats, invalidLines);

 
  if(arrayMax(stats)==0)
    die("No genes left");
  
  sortAndCollapse(stats);

  printNVPairArray(outFile, stats);
  printInvalidLines(invalidFile, invalidLines);

  destroyNVPairArray(stats);
  textDestroy(invalidLines);
  return 0;
}
