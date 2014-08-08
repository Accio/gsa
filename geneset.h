#ifndef GENESET_H
#define GENESET_H

#include "gsa.h"
#include "genestat.h"

typedef std::set<std::string> StringSet;

class GeneSet {
  std::string _name;
  std::string _desc;
  StringSet _genes; 
  std::vector<int> _indices;
  double _es;
  double _nes;
  double _p;
  double _fdr;

 public:
  GeneSet(const std::string& name,
	  const std::string& desc,
	  const StringVec::const_iterator begin,
	  const StringVec::const_iterator end):_name(name),_desc(desc), _genes(begin, end) {};
  const std::string name() {return _name;}
  const int size() const {return(_genes.size());}
  void setIndices(const StringVec& accs);

  void print();
};


typedef std::vector<GeneSet> GeneSets;
void setIndices(GeneSets& gss, const GeneStats& stats);

#endif
