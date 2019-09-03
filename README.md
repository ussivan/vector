# Vector with SO and CoW

Vector on c++ with 2 optimizations:

### Small Object (SO)

Small object optimization is a technique which is used within low level data structures. 
It's meant to use stack space as a buffer instead of some allocated memory in 
case the content is small enough to fit within the reserved space.

### Copy on write (CoW)

Modified object is copied on writing it somewhere, not on modifying.
