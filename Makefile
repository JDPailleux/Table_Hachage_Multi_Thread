
LIB=./lib/libHashMap.so
SRC= $(wildcard src/*.c)
OBJ= $(subst src/, lib/,  $(SRC:.c=.o))
C_FLAGS= -fPIC -Wall -O3 -std=c99
LD_FLAGS= -shared -fPIC -lpthread
CC=gcc

#Flags used to compile the tests
TEST_FLAGS= -L./lib/ -lHashMap  -I./src/ -lpthread 

$(LIB) : $(OBJ) Makefile 
	$(CC) -o $(LIB) $(OBJ) $(LD_FLAGS)
	
lib/%.o :  src/%.c Makefile
	$(CC) $(C_FLAGS) -c $< -o $@

test : tests/test1.c tests/test2.c tests/impl_string_hashmap.c Makefile  
	$(CC) ./tests/test1.c ./tests/impl_string_hashmap.c -o ./tests/test1 $(TEST_FLAGS)
	./tests/test1
	$(CC) ./tests/test2.c -g ./tests/impl_string_hashmap.c -o ./tests/test2 $(TEST_FLAGS) 
	./tests/test2

clean:
	rm $(LIB) $(OBJ) 
