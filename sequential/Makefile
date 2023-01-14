CC= gcc
SRC= sources
BIN= bin


.PHONY: clean all 

run: main
	./$(BIN)/$<

main: $(SRC)/lib.c
	$(CC) -g -c $< -o $(BIN)/$@.o
	$(CC) -g -c $(SRC)/main.c -o $(BIN)/lib.o
	$(CC) -o $(BIN)/$@ ./$(BIN)/$@.o ./$(BIN)/lib.o  -lcmocka -lm


test:$(SRC)/test.c
	make main
	$(CC) -g -c $< -o $(BIN)/$@.o 
	$(CC) -o $(BIN)/$@ ./$(BIN)/$@.o ./$(BIN)/main.o -lcmocka -lm
	rm -f lib.o test.o

	time ./$(BIN)/$@

clean: 
	rm -f bin/*