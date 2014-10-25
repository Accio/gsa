clean:
	rm -f core.* *~

test-gseap: gseap-prototype simulated-diffstats.txt simulated-genesets.txt
	./gseap-prototype

gseap-prototype:gseap-prototype.cpp genestat.cpp genestat.h geneset.h
	$(CXX) $(CXXFLAGS) -Wall gseap-prototype.cpp genestat.cpp geneset.cpp -o gseap-prototype 


simulated-diffstats.txt simulated-genesets.txt: generate-data

generate-data:generate-data.R
	R -f generate-data.R
