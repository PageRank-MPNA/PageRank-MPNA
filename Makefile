all: compil

sequential:
	cd $@ && $(MAKE)

parallel:
	cd $@ && $(MAKE)

compil:
	g++ -O2 -march=native madj_parser.cpp -o parse

parse: compil
	./parse
test_matrix:
	gcc -o3 create_test_matrix.c -o test_matrix

.PHONY: clean all parallel sequential compil parse

clean:
	rm -f parse test_matrix