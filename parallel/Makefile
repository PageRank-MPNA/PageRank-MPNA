CC= mpicc
SRC= sources/
BIN= bin/


.PHONY: clean all 

run: main
	OMP_NUM_THREADS=1 mpirun -np 2 ./$(BIN)$<

main: $(SRC)lib.c
## 	$(CC) $< -o $(BIN)$@ -lm
	$(CC) -g -c $< -o $(BIN)$@.o -g
	$(CC) -g -c $(SRC)main.c -o $(BIN)$lib.o  -g

	$(CC) -o $(BIN)$@ ./$(BIN)$@.o ./$(BIN)$lib.o  -g -lcmocka -lm -fopenmp


test:$(SRC)test.c
	make main
	$(CC) -g -c $< -o $(BIN)$@.o 
	$(CC) -o $(BIN)$@ ./$(BIN)$@.o ./$(BIN)main.o -lcmocka -lm -fopenmp
	rm -f lib.o test.o

	OMP_NUM_THREADS=2 mpirun -np 2 ./$(BIN)$@

clean: 
	rm -f bin/*