all: compil

sequential:
	cd $@ && $(MAKE)

parallel:
	cd $@ && $(MAKE)

compil:
	g++ -O2 -march=native madj_parser.cpp -o parse

parse: 
	./parse

.PHONY: clean all parallel sequential compil parse

clean:
	rm parse