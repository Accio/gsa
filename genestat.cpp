#include <iostream>
#include "genestat.h"
using namespace std;

bool descCompGeneStat(const GeneStat& i,const GeneStat& j) { 
  return (i.stat()>j.stat());
}

void GeneStat::print() {
  cout << _id << ":" << _stat << "\n";
}
/*
int main() {
  GeneStat gs=GeneStat("ABC", 3.25);
  gs.setAcc("g_ABC");
  cout << gs.id() << "(" << gs.acc() << ") stat:" << gs.stat() << "\n";
  return 0;
}
*/
