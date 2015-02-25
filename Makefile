include $(BIOINFOCONFDIR)/biosdefs.make

clean:
	rm -f core.* *~

test-gseap: gseap-prototype simulated-diffstats.txt simulated-genesets.txt
	./gseap-prototype

gseap-prototype:gseap-prototype.cpp genestat.cpp genestat.h geneset.h
	$(CXX) $(CXXFLAGS) -Wall gseap-prototype.cpp genestat.cpp geneset.cpp -o gseap-prototype 


simulated-diffstats.txt simulated-genesets.txt: generate-data

generate-data:generate-data.R
	R -f generate-data.R

filter-invalid:withinvalid-diffstats.txt
	sort -r -k 2 -n  withinvalid-diffstats.txt  | awk '$$2 ~ /^-?[0-9]+(\.[0-9]+)?((e|E)(+|-)[0-9]+){0,1}$$/' | sort -u -k 1,1 | sort -r -k 2 -n > filtered-diffstats.txt
	diff filtered-diffstats.txt valid-diffstats.txt

filter_rnk:filter_rnk.c
	$(CC) $(BIOSINC)  filter_rnk.c -o filter_rnk $(BIOSKERNLNK)
