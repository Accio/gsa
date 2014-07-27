#include <iostream>
#include <vector>
#include <set>
#include "geneset.h"

void GeneSet::print() {
  std::cout << _name << "(" << _genes.size() << " genes)" << "\n";
}
