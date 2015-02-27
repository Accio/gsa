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

rnk_file.o: rnk_file.h rnk_file.c $(PRPILIB) $(APPCONF_H)
	$(CC) -std=gnu99 $(DEBUG) $(CFLAGS) $(APPINC) $(PRPIINC) -c rnk_file.c -o rnk_file.o

collapse_rnk: rnk_file.o collapse_rnk.c
	$(CC) $(CFLAGS) $(APPINC) $(PRPIINC) collapse_rnk.c $O/rnk_file.o -o collapse_rnk $(PRPILNK) -lm

test-collapse_rnk:tests/withinvalid-diffstats.txt $O/collapse_rnk
	$O/collapse_rnk -in tests/withinvalid-diffstats.txt
	$O/collapse_rnk -in tests/withinvalid-diffstats.txt -out tests/collapse_rnk.out
	$O/collapse_rnk -in tests/withinvalid-diffstats.txt -out tests/collapse_rnk.out -invalid tests/collapse_rnk_invalid.out
	$O/collapse_rnk -in tests/withinvalid-diffstats.txt -out - -invalid tests/collapse_rnk_invalid.out
	$O/collapse_rnk -in tests/withinvalid-diffstats.txt -out - -invalid -
	diff tests/collapse_rnk.out tests/withinvalid-diffstats-expected.out
	diff tests/collapse_rnk_invalid.out tests/withinvalid-diffstats-expected-invalid.out

filter_rnk:filter_rnk.c
	$(CC) $(BIOSINC)  filter_rnk.c -o filter_rnk $(BIOSKERNLNK)

run-filter_rnk:filter_rnk
	./filter_rnk

filter_and_collapse:filter_rnk withinvalid-diffstats.txt
	./filter_rnk | sort -r -k 2 -n | sort -u -k 1,1 | sort -r -k 2 -n ## tak the largest statistics (not the largest absolute value)