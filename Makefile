all: compil

sequential:
	cd $@ && $(MAKE)

parallel:
	cd $@ && $(MAKE)

compil:
	gcc -O2 -march=native madj_parser.c -o parse

parse: 
	./parse

.PHONY: clean all parallel sequential compil parse

clean:
	rm parse