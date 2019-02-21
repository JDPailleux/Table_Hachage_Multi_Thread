%% Compilation 
Dependency of the project: pthread library 
The project has been compiled and test ran with gcc v7.3.0
**make** compiles the library in the lib dir
Sources are in the src dir 

%% Testing 
The files relevant for testing are contained in the ./test dir. 
**make test** builds and runs the 2 test programs
You may need to export the variable LD_LIBRARY_PATH containing the ./lib dir in 
order to run the tests
For testing purposes, a hash table implementation that uses string objects is 
defined in ./test/impl_string_hashmap* files 

%%%% test1
This test uses two threads that manipulate the same string_t object and perform 
an equal number of get and put operations to validate the concurrency implementation 
To succeed, the test should plot an empty hash table at the end 

%%%% test2
This test uses 26 threads. Each thread has its string object and 
manipulate it through the table. To succeed, the test should plot
a table containing 26 string_t objects, with 100 ref_counts per object


%% Concurrency handling:
The way the thread safety is handled right now is a simple global lock for the whole table. 
It can be further optimized if needed in many ways such as adopting a finer lock system, 
for each bucket for example with a thread-safe linked list implementation. Combined with
a hashing function that dispatch data uniformly enough across the hash table should be 
able to handle most of the cases

%% Portability
The code is portable except that the **off_t** type that isn't defined by the C standard. It's part 
of the POSIX standard and defined in sys/type.h file therefore the code is not portable to
 other OS than LINUX so far.

