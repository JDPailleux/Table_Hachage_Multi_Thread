
LIB=./lib/libHashMap.so
SRC= $(wildcard src/*.c)
OBJ= $(subst src/, lib/,  $(SRC:.c=.o))
C_FLAGS= -fPIC -Wall -O3 -Ofast -std=c99
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

#CC = gcc
# CFLAGS = -O2

# FF = g77
# FFLAGS = -O2

# all: stream_f.exe stream_c.exe

# stream_f.exe: stream.f mysecond.o
# 	$(CC) $(CFLAGS) -c mysecond.c
# 	$(FF) $(FFLAGS) -c stream.f
# 	$(FF) $(FFLAGS) stream.o mysecond.o -o stream_f.exe

# stream_c.exe: stream.c
# 	$(CC) $(CFLAGS) stream.c -o stream_c.exe

# clean:
# 	rm -f stream_f.exe stream_c.exe *.o

# # an example of a more complex build line for the Intel icc compiler
# stream.icc: stream.c
# 	icc -O3 -xCORE-AVX2 -ffreestanding -qopenmp -DSTREAM_ARRAY_SIZE=80000000 -DNTIMES=20 stream.c -o stream.omp.AVX2.80M.20x.icc