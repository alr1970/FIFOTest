# FIFOTest
FIFO implementation for coding test

To build:
Uses Cmake 3.1 or greater
from the folder containing CMakeLists.txt run:
> cmake .
then
> make

To run unit tests:
>./FifoTest
This uses the Catch2 lightweight test framework, from the Catch2/catch.hpp file. Originally from
https://github.com/catchorg/Catch2

To run threaded test program:
>./FifoThreadTest
This puts 500 ints into a Fifo<int> and uses 2 threads to pick them out. It hangs at the end but proves the queue is
working.

Implementation:
Fifo wraps an STL std::deque, using a std::mutex to protect the deque in the push and pop methods.
The move constructor and operator are provided but the copy constructor and operator have been deleted because they
could result in duplication of any items in the deque if used.
The destructor just clears the deque once it has safe access to it. This means that if any items in the deque contain
dynamically allocated data, that could be leaked. Careful design of the objects can avoid this, for example by using
smart pointers.
