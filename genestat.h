class GeneStat {
  std::string _id; // input identifier
  std::string _acc; // accession identifier
  double _stat;
  
 public:
  GeneStat(){};
  GeneStat(const std::string& id, double stat):_id(id),_stat(stat){};
  void setID(const std::string& id) {_id=id;}
  void setAcc(const std::string& acc) {_acc=acc;}
  void setStat(double stat) {_stat=stat;}
  const std::string id() {return _id;}
  const std::string acc() {return _acc;}
  const double stat() const {return _stat;}

  void print();
};

bool descCompGeneStat(const GeneStat& i, const GeneStat& j);
