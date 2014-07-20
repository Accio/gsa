#include <iostream>
using namespace std;

class GeneStat {
  std::string _id; // input identifier
  std::string _acc; // accession identifier
  double _stat;

public:
  GeneStat(const string& id, double stat):_id(id),_stat(stat){};
  void setAcc(const string& acc) {_acc=acc;}
  const std::string id() {return _id;}
  const std::string acc() {return _acc;}
  double stat() {return _stat;}
};

int main() {
  GeneStat gs=GeneStat("ABC", 3.25);
  gs.setAcc("g_ABC");
  cout << gs.id() << "(" << gs.acc() << ") stat:" << gs.stat() << "\n";
  return 0;
}
