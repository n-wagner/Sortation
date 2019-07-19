# Sortation
Various sortation algorithms and comparisons of their speeds.

## QS.cpp
Contains code from a previous assignment which runs Quick Sort
with various configurations on randomly generated strings.
Results are compared against qsort(...) and sort(...) from
C's stdlib.h and C++'s algorithm, respectively.

### 	std::async
As it stands, there seems to be an issue with getting proper
timings for the variations of quicksort run under multi-
threading. Not sure why that is, but different (thread-based)
timing functions still do not produce the same (or even close) 
output to that of the sequential process-wide timer for the 
same sorts. Due to this fact, it's probably not going to be 
something I pursue for the time being as the potential for 
time saved running everything asynchronously is rather moot 
if proper timings cannot be gathered...

### System
Really old FreeBSD Unix system. I think the last time it was
updated was 2006 or so. This may be an issue with running it,
but going forward I hope to make it compatible on Cygwin.

## Future Plans
I want to include hopefully all of the major algorithms and 
some of the more advanced hybrid ones later on. Also <strike>
I want to incorporate multithreading to speed up the test 
runs and</strike> maybe use functors in place of the function 
pointers I have currently.

### Architecture
The plan is to break all of the sort implementations into
separate code files and just have a central file with the timing
functions which imports and invokes everything. 
