BIN=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))/bin/
OBJ=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))/obj/
SRC=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))/

CFLAGS=-Wall `pkg-config --cflags gtk+-3.0`
LDFLAGS=`pkg-config --libs gtk+-3.0` -lgtksourceview-3.0

all: $(BIN) $(OBJ) $(BIN)main.elf

$(BIN):
	mkdir -p $(BIN)

$(OBJ):
	mkdir -p $(OBJ)
	
$(OBJ)gtkcellrenderermultiple.o: $(SRC)gtkcellrenderermultiple.c $(SRC)gtkcellrenderermultiple.h
	gcc $(CFLAGS) -I/usr/include/gtksourceview-3.0/ -c $(SRC)gtkcellrenderermultiple.c -o $(OBJ)/gtkcellrenderermultiple.o

$(OBJ)main.o: $(SRC)main.c $(SRC)gtkcellrenderermultiple.h
	gcc $(CFLAGS) -I/usr/include/gtksourceview-3.0/ -c $(SRC)main.c -o $(OBJ)/main.o

$(BIN)main.elf: $(OBJ)gtkcellrenderermultiple.o $(OBJ)main.o
	gcc $(OBJ)/gtkcellrenderermultiple.o $(OBJ)/main.o -o $(BIN)/main.elf $(LDFLAGS) 

clean:
	rm -f $(OBJ)*.o

mrproper: clean
	rm -f $(BIN)*
