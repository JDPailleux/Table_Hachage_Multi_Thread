# HT_000
HT_000 is a hash table implementation written in C which supports multi-
threaded applications.  HT_000 supports atomic construction and destruction of
objects.  When a requesting thread obtains a hash object, HT_000 guarantees
that the object will not be freed until the thread has released this object.

The task here is to implement HT_000 as described below.  The implementation
must be done in C and must be accompanied with multi-threaded test program
which demonstrates the use of HT_000.

See README.todo.txt for more details.
