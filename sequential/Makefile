CC= gcc
SRC= sources
BIN= bin


.PHONY: clean all 

run: main
	./$(BIN)/$<


bench: $(SRC)/bench2.c
	make main
	$(CC) -g -c $< -o $(BIN)/$@.o 
	$(CC) -g -o $(BIN)/$@ ./$(BIN)/$@.o ./$(BIN)/main.o -lm -fopenmp
	rm -f lib.o test.o
	./$(BIN)/$@ /gpfs/users/m2chps-17/PageRank-MPNA/sequential/data

main: $(SRC)/lib.c
	$(CC) -g -c $< -o $(BIN)/$@.o
	$(CC) -g -c $(SRC)/main.c -o $(BIN)/lib.o -fopenmp
	$(CC) -o $(BIN)/$@ ./$(BIN)/$@.o ./$(BIN)/lib.o -lm -fopenmp

test:$(SRC)/test.c
	make main
	$(CC) -g -c $< -o $(BIN)/$@.o 
	$(CC) -o $(BIN)/$@ ./$(BIN)/$@.o ./$(BIN)/main.o -lcmocka -lm -fopenmp
	rm -f lib.o test.o

	time ./$(BIN)/$@

clean: 
	rm -f bin/*
