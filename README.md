# PageRank-MPNA

This projet is divided in two sections: sequential and parallel.

There is a main Makefile that can call the two sub-makefiles of each section.

Here each makes and sub makes that can be done at each folder:

- make sequential 
  - make main
  - make run
  - make test
  - make clean 

- make parallel 
  - make main
  - make run
  - make test
  - make bench
  - make clean


./bin/main ./data/<file> -> if you want to test any custom CSR file

- make parse -> For the creation of the Amazon matrix