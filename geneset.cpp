#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include "geneset.h"

void GeneSet::print() {
  std::cout << _name << "(" << _genes.size() << " genes)" << "\n";
}

void GeneSet::setIndices(const StringVec& accs) {
  StringSet::const_iterator it;
  StringVec::const_iterator git;
  
  for(it=_genes.begin();it!=_genes.end();++it) {
    git=find(accs.begin(), accs.end(), *it);
    if(git!=accs.end()) {
      _indices.push_back(git-accs.begin());
    } else {
      _indices.push_back(-1);
    }
  } 
}

void setIndices(GeneSets& gss, const GeneStats& stats) {
  const StringVec gaccs=accs(stats);
  GeneSets::iterator it;
  for(it=gss.begin();it<gss.end();++it) {
    it->setIndices(gaccs);
  }
} 
