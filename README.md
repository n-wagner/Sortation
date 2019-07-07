# Sortation
Various sortation algorithms and comparisons of their speeds.

## QS.cpp
Contains code from a previous assignment which runs Quick Sort
with various configurations on randomly generated strings.
Results are compared against qsort(...) and sort(...) from
C's stdlib.h and C++'s algorithm, respectively.

### System
Really old FreeBSD Unix system. I think the last time it was
updated was 2006 or so. This may be an issue with running it,
but going forward I hope to make it compatible on Cygwin.

## Future Plans
I want to include hopefully all of the major algorithms and 
some of the more advanced hybrid ones later on. Also I want 
to incorporate multithreading to speed up the test runs and 
maybe use functors in place of the function pointers I have 
currently.
