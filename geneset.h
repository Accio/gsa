typedef std::vector<std::string> Accs;

class GeneSet {
  std::string _name;
  std::string _desc;
  std::set<std::string> _genes; 

 public:
  GeneSet(const std::string& name,
	  const std::string& desc,
	  const Accs::const_iterator begin,
	  const Accs::const_iterator end):_name(name),_desc(desc), _genes(begin, end) {};
  const std::string name() {return _name;}
  const int size() const {return(_genes.size());}
  
  void print();
};
