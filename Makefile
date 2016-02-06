APP=ltalk
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
CC=clang 
CFLAGS=--std=gnu99 -g3 -Wall --pedantic
LDFLAGS=-lpthread
all: $(APP)

$(APP): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $(APP)
%.o: %.c 
	$(CC) -c $(CFLAGS) $< -o $@

test: all
	@echo "build test"
	@$(MAKE) $(MAKEFLAGS) -C tests
	
clean:
	rm -f $(APP) *.o tests/*.o 