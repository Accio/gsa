gseap:gseap-prototype.cpp genestat.cpp genestat.h geneset.h
	$(CXX) -std=c++11 -Wall gseap-prototype.cpp genestat.cpp geneset.cpp -o gseap-prototype 
generate-data:generate-data.R
	R -f generate-data.R