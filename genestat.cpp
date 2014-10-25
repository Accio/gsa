#include <iostream>
#include "genestat.h"
using namespace std;

bool descCompGeneStat(const GeneStat& i,const GeneStat& j) { 
  return (i.stat()>j.stat());
}

void GeneStat::print() {
  cout << _id << ":" << _stat << "\n";
}

StringVec accs(const GeneStats& gss) {
  StringVec res;
  GeneStats::const_iterator it;
  for(it=gss.begin();it<gss.end();++it) {
    res.push_back(it->acc());
  }
  return(res);
}
/*
int main() {
  GeneStat gs=GeneStat("ABC", 3.25);
  gs.setAcc("g_ABC");
  cout << gs.id() << "(" << gs.acc() << ") stat:" << gs.stat() << "\n";
  return 0;
}
*/
