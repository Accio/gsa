// a prototype C++ program of the GSEA-Prerank algorithm

#include <vector>
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime> // std::time
#include <algorithm> // std::random_shuffle
#include <cstdlib> 

#include "genestat.h"
#include "geneset.h"

using namespace std;

typedef std::vector<std::string> StringVec;
typedef StringVec GeneNames;
typedef std::vector<double> Stats;
typedef std::vector<GeneStat> GeneStats;
typedef std::vector<GeneSet> GeneSets;

bool issquote(const char &c) {
  return(isspace(c) || c=='"' || c=='\'');
}

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

    return buf;
}

void log(const std::string& log) {
  cout << "[" << currentDateTime() << "] " << log << "\n";
}

StringVec split(const string& s) {
  vector<string> ret;
  typedef string::size_type string_size;
  string_size i=0, j=0;

  while(i!=s.size()) {
    // ignore leading blanks
    while(i!=s.size() && issquote(s[i]))
      ++i;
    
    if(s[i]=='#') { // commented line
      break;
    }
    // find end of next word: characters in [i,j) is not space or quote
    j=i;
    while(j != s.size() && !issquote(s[j]))
      ++j;

    // if we found some non-whitespace/quote characters
    if(i!=j) {
      ret.push_back(s.substr(i, j-i));
      i=j;
    }
  }
  return(ret);
}

// split two columns and append to the data
void readLine(const string& s, GeneStats& gss) {
  double val;
  StringVec ret=split(s);
  if(ret.size()==2) {
    try {
      val=std::stod(ret[1]); // C++11 specific code
      GeneStat gs(ret[0], val);
      gss.push_back(gs);
    } catch(std::exception &e) {
      cout << "Ignored line: " << s << "\n";
    }
  } else {
    cout << "Ignored line: " << s << "\n";
  }
  return;
}

// read gene sets
void readGmt(const string& s, GeneSets& gss) {
  StringVec ret=split(s);

  if(ret.size()>3) {
    GeneSet gs(ret[0],
	       ret[1],
	       ret.begin()+2, ret.end());
    gss.push_back(gs);
  } else {
    cout << "Ignored line: " << s << "\n";
  }
  return;
}

int main(int argc, char** argv) {
  string statfile("simulated-diffstats.txt");
  string gmtfile("simulated-genesets.gmt");
  string line;
  int i;
  //const int NPERM=1000;

  GeneStats gss;
  GeneSets gsets;

  log("Reading genes");
  ifstream statf(statfile.c_str());
  if(statf.is_open()) {
    while(getline(statf, line)) {
      readLine(line, gss);
    }
    statf.close();
  } else {
    cout << "Unable to open file";
  }
  cout << "Read " <<  gss.size() << " genes" << "\n";

  log("Reading gene sets");
  ifstream gsetf(gmtfile.c_str());
  if(gsetf.is_open()) {
    while(getline(gsetf, line)) {
      readGmt(line, gsets);
    }
    gsetf.close();
  } else {
    cout << "Unable to open file";
  }
  cout << "Read " <<  gsets.size() << " gene sets" << "\n";
  //for(i=0;i<3;i++)
  //  gsets[i].print();

  // sorting
  log("Rank genes");
  std::sort(gss.begin(), gss.end(), descCompGeneStat);

  // random shuffle gene symbols
  // log("Gene permutation");
  // std::srand(unsigned (std::time(0)));
  // for(i=0;i<NPERM;++i)
  //  std::random_shuffle(gss.begin(), gss.end());
  
  log("Program quitting");
  return 0;
}
