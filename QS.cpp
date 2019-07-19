#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <sys/resource.h>

#include <vector>
#include <future>
#include <chrono>

/* Coming soon...
#include "heap.hpp"
#include "tree.hpp"
*/

using namespace std;


//Given a pointer to an array of strings (A), swaps the entries at A[a] and A[b], no return (array modified by reference)
void swap (string * A, const int a, const int b) {
  if (a != b) {
    const string temp = A[a];
    A[a] = A[b];
    A[b] = temp;
  }
}

//3 way swap in given pointer to an array of strings (A) - (c->a->b->c), no return (array modified by reference)
void triSwap (string * A, const int a, const int b, const int c) {
  const string temp = A[c];
  A[c] = A[b];
  A[b] = A[a];
  A[a] = temp;
}

//Returns the position of the "median" as the first element in string array A
int firMed (const string * A, const int a, const int b) {
  return a;
}

//Returns the position of the "median" as the middle element in string array A
int midMed (const string * A, const int a, const int b) {
  return ((a + b) / 2);
}

//Returns the position of the "median" as the last element in string array A
int endMed (const string * A, const int a, const int b) {
  return b;
}

//Returns the position of the "median" as the "median of three" in string array A (middle between the first, middle, and last elements)
int medOfThree (const string * A, const int a, const int b) {
  int beg = a, mid = ((a + b) / 2), end = b;
  string first = A[beg], middle = A[mid], last = A[end];

  if (first <= middle && first <= last) {		//1, 2, 3 or 1, 3, 2
    if (middle <= last) {
      return mid;
    } else {
      return end;
    }
  } else if (middle <= first && middle <= last) {	//2, 1, 3 or 2, 3, 1
    if (first <= last) {
      return beg;
    } else {
      return end;
    }
  } else {						//3, 1, 2 or 3, 2, 1
    if (first <= middle) {
      return beg;
    } else {
      return mid;
    }
  }
}

/**
 * Slightly more slimmed down version of the above
 * Maximum of 3 comparisons ever made (as compared to the other's 5)
 */
int medianOfThree (const string * A, const int a, const int b) {
  int beginning = a, middle = (a + b) / 2, end = b;
  string first = A[beginning], second = A[middle], third = A[end];

  if (first < second) {          //1, 2, 3 or 1, 3, 2 or 3, 1, 2
    if (second < third) {
      return middle;
    } else if (first < third) {
      return end;
    } else {
      return beginning;
    }
  } else {                      //2, 1, 3 or 2, 3, 1 or 3, 2, 1
    if (first < third) {
      return beginning;
    } else if (second < third) {
      return end;
    } else {
      return middle;
    }
  }
}

//Original quicksort partitioning method (unexplored region in the center of the array as pointers decrement/increment from either side)
//Takes string array A, bounds of the array (a, b), references to m & n which are used as the bounds in subsequent
//  iterations of the recursive quicksort, and pointer to a median function (of the form of one of the four above)
//  no return, but A is modified by reference
void origPart(string * A, const int a, const int b, int & m, int & n, int (* med) (const string *, const int, const int)) {
  const int pivotPos = med(A, a, b);
  const string pivotVal = A[pivotPos];
  swap(A, pivotPos, b);

  int p =  a;
  int q = b - 1;

  while (true) {
    while (A[p] < pivotVal && p < b) {
      p++;
    }
    while (A[q] >= pivotVal && q > p) {
      q--;
    }
    if (p < q) {
      swap(A, p, q);
      p++;
      q--;
    } else {
      break;
    }
  }
  swap(A, p, b);
  m = q;
  n = p + 1;
}

//Triple region quicksort partitioning method (characterized by having less than, equal to, and greator than regions)
//Takes string array A, bounds of the array (a, b), references to m & n which are used as the bounds in subsequent
//  iterations of the recursive quicksort, and pointer to a median function (of the form of one of the four above)
//  no return, but A is modified by reference
void triRegionPart (string * A, const int a, const int b, int & m, int & n, int (* med) (const string *, const int, const int)) {
  const int pivotPos = med(A, a, b);
  const string pivotVal = A[pivotPos];
  swap(A, a, pivotPos);

  int eql = a;
  int grtr = a + 1;
  int unk = a + 1;

  while (unk <= b) {
    if (A[unk] < pivotVal) {
      triSwap(A, eql, grtr, unk);
      eql++;
      grtr++;
    } else if (A[unk] == pivotVal) {
      swap(A, grtr, unk);
      grtr++;
    } else {
    //Do the below
    }
//Incremented in all of the above cases so brought out of the if block for simplicity
    unk++;
  }
  m = eql - 1;
  n = grtr;
}

//Double region quicksort partitioning method (characterized by having a less than and a greatorthan/equal to region - essentially a faster
//  version of the above triple region quicksort)
//Takes string array A, bounds of the array (a, b), references to m & n which are used as the bounds in subsequent
//  iterations of the recursive quicksort, and pointer to a median function (of the form of one of the four above)
//  no return, but A is modified by reference
void twoRegionPart (string * A, const int a, const int b, int & m, int & n, int (* med) (const string *, const int, const int)) {
  const int pivotPos = med(A, a, b);
  const string pivotVal = A[pivotPos];
  swap(A, b, pivotPos);

  int grtr = a;
  int unk = a;

  while (unk < b) {
    if (A[unk] < pivotVal) {
      swap(A, grtr, unk);
      grtr++;
    } else {
    //Do the below
    }
//Incremented in both of the cases so brought out of the if block for simplicity
    unk++;
  }
  swap(A, b, grtr);
  m = grtr - 1;
  n = grtr + 1;
}

//General quicksort algorithmn, takes pointer to string array A, bounds a to b (initial call 0 and N - 1, respectively),
//  pointer to partitioning method (of the form of one of the above 3), and pointer to a median function (of the form of those
//  above the partitioning functions above - see the partitioning functions for more details)
//No return, but A is modified by reference
void quicksort (string * A, const int a, const int b, 
                void (* part) (string *, const int, const int, int &, int &, int (*) (const string *, const int, const int)),
                int (* med) (const string *, const int, const int)) {
  if (a >= b) {
    return;
  }
  int m, n;
  part(A, a, b, m, n, med);
  quicksort(A, a, m, part, med);
  quicksort(A, n, b, part, med);
}

//Generates and returns random string of given length (len) that contains only alphabetic characters
string randomString (const int len) {
  const string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopkrstuvwxyz";
  string result = "";
  for (int i = 0; i < len; i++) {
//    result += alphabet[random() % (alphabet.length() - 1)];			Not compatible with Cygwin?
    result += alphabet[rand() % (alphabet.length() - 1)];
  }
  return result;
}

//Compare function to be used with C standard function "qsort"
//  compares two strings (a, b) and returns -1 if a < b, 0 if a == b, or 1 if a > b
int compare (const void * a, const void * b) {
  const string x = *(const string *)a;
  const string y = *(const string *)b;
  
  if (x < y) {
    return -1;
  } else if (x == y) {
    return 0;
  } else {
    return 1;
  }
}

//Returns CPU time as a double (Murrell's timing function)
double getCPUTime() {
  struct rusage ruse;
  getrusage(RUSAGE_SELF, &ruse);
  return ruse.ru_utime.tv_sec + ruse.ru_utime.tv_usec / 1000000.0 + 
         ruse.ru_stime.tv_sec + ruse.ru_stime.tv_usec / 1000000.0;
}

/**
 * Thread-specific timing function (takes CPU timing per thread)
 *  - ISSUE: Times seem to be longer than process wide CPU Timer
 *           for multi-threaded clock-ins using std::async?
 */
double getTime () {
  struct timespec time;
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &time);
//  cout << "Time is " << time.t
  return time.tv_sec + time.tv_nsec / 1000000000.0;
}

/**
 * Multithreaded "run" function, performs a singular variation of quicksort
 *  - ISSUE: Timings gathered by start/end do NOT coincide with the sequential
 *           implementation (as in oldRun(...))
 */
double handleSort (string * Master, string * Sorted, const int index, const int size,
                   void (* part) (string *, const int, const int, int &, int &, int (*) (const string *, const int, const int)),
                   int (* med) (const string *, const int, const int)) {
  string * copy = new string[size];
  for (int i = 0; i < size; i++) {
    copy[i] = Master[i];
  }
  double start = getTime();
//  double t_start = time(NULL);
//  auto c_start = chrono::steady_clock::now();

  quicksort(copy, 0, size - 1, part, med); 

  double end = getTime();
//  double t_end = time(NULL);
//  auto c_end = chrono::steady_clock::now();

//  chrono::duration<double> change = c_end - c_start;

//  double diff = difftime(t_end, t_start);

  cout << "Index " << index / 10 << "." << index % 10 << " time was " << end - start << " seconds.\n";

  for (int i = 0; i < size; i++) {
    if (copy[i] != Sorted[i]) {
      cout << "Mismatch at position: " << i << "for array with index: " << index / 10 << "." << index % 10 << endl;
      exit(1);
    }
  }
  delete copy;
  return end - start;
}

/**
 * Multi-threading implementation of all of the variations of quicksort
 *  - ISSUE: Timings do NOT line up with the sequential testing implementation
 *           Off (longer) by about 75% or so
 */
/*
void handleAll (string * Master, const int size) {
  void (* partArray[]) (string *, const int, const int, int &, int &, int (*) (const string *, const int, const int))
        = {origPart, triRegionPart, twoRegionPart};
  const int PART_ARRAY_SIZE = 3;
  int (* medArray[]) (const string *, const int, const int) = {firMed, midMed, endMed, medOfThree};
  const int MED_ARRAY_SIZE = 4;
  vector<future<double>> returns; 

  double start_all = getTime();

  string * QSorted = new string [size];
  string * AlgSorted = new string [size];

  for (int i = 0; i < size; i++) {
    QSorted[i] = AlgSorted[i] = Master[i];
  }

  double start = getCPUTime();
  sort(AlgSorted, AlgSorted + size);
  double end = getCPUTime();
  cout << "First check (sort(...) from <algorithm>) finished sorting in " << end - start << " seconds.\n";

  start = getCPUTime();
  qsort(QSorted, size, sizeof(string), compare);
  end = getCPUTime();
  cout << "Second check (qsort(...) from <stdlib.h>) finished sorting in " << end - start << " seconds.\n";

  for (int i = 0; i < size; i++) {
    if (QSorted[i] != AlgSorted[i]) {
      cout << "The checks don't match up...\n";
      exit(1);
    }
  }

  for (int i = 0; i < PART_ARRAY_SIZE; i++) {
    for (int j = 0; j < MED_ARRAY_SIZE; j++) {
      returns.push_back(async(handleSort, Master, AlgSorted, (10 * i) + j, size, partArray[i], medArray[j]));
    }
  }
  
  cout << endl << endl;

  for (int i = 0; i < PART_ARRAY_SIZE; i++) {
    for (int j = 0; j < MED_ARRAY_SIZE; j++) {
      cout << "Sort index: " << i << "." << j << " finished in " << returns.at((i * MED_ARRAY_SIZE) + j).get() << " seconds.\n";
    }
    cout << endl;
  }
  double end_all = getTime();

  cout << "Entire new process takes " << end_all - start_all << " seconds.\n\n";

  delete QSorted;
  delete AlgSorted;
}
*/

//Handles the running of all the different versions of the sorts given the appropriate arrays (A-L, and checks), size,  and the 
//  (choice) of which block(s) to run
//No return, but arrays will be modified by reference depending on choice input
//choice: 1 = original partition, 2 = tri-region partition, 3 = two-region partition, 4 = all partition methods
void runSorts (string * A, string * B, string * C, string * D, string * E, string * F, string * G,
               string * H, string * I, string * J, string * K, string * L, string * check, string * check2,
               string * X, string * Y, string * Z,
               const int choice, const int size) {

  double t[30], start, stop;

  if (choice == 1 || choice == 4) {
    t[0] = getCPUTime();
    double t1 = getTime();
    quicksort(A, 0, size - 1, origPart, firMed);
    double t2 = getTime();
    t[1] = getCPUTime();
    cout << "\nOriginal Partition Method with first element pivot finished sorting in " << t[1] - t[0] << " (" << t2 - t1 << ") seconds.\n";
    t[2] = getCPUTime();
    quicksort(B, 0, size - 1, origPart, midMed);
    t[3] = getCPUTime();
    cout << "Original Partition Method with middle element pivot finished sorting in " << t[3] - t[2] << " seconds.\n";
    t[4] = getCPUTime();
    quicksort(C, 0, size - 1, origPart, endMed);
    t[5] = getCPUTime();
    cout << "Original Partition Method with last element pivot finished sorting in " << t[5] - t[4] << " seconds.\n";
    t[6] = getCPUTime();
    quicksort(D, 0, size - 1, origPart, medOfThree);
    t[7] = getCPUTime();
    cout << "Original Partition Method with old median of three pivot finished sorting in " << t[7] - t[6] << " seconds.\n";
    start = getCPUTime();
    quicksort(X, 0, size - 1, origPart, medianOfThree);
    stop = getCPUTime();
    cout << "Original Partition Method with new median of three pivot finished sorting in " << stop - start << " seconds.\n\n";
  }
  if (choice == 2 || choice == 4) {
    t[8] = getCPUTime();
    quicksort(E, 0, size - 1, triRegionPart, firMed);
    t[9] = getCPUTime();
    cout << "Tri-Region Partition Method with first element pivot finished sorting in " << t[9] - t[8] << " seconds.\n";
    t[10] = getCPUTime();
    quicksort(F, 0, size - 1, triRegionPart, midMed);
    t[11] = getCPUTime();
    cout << "Tri-Region Partition Method with middle element pivot finished sorting in " << t[11] - t[10] << " seconds.\n";
    t[12] = getCPUTime();
    quicksort(G, 0, size - 1, triRegionPart, endMed);
    t[13] = getCPUTime();
    cout << "Tri-Region Partition Method with last element pivot finished sorting in " << t[13] - t[12] << " seconds.\n";
    t[14] = getCPUTime();
    quicksort(H, 0, size - 1, triRegionPart, medOfThree);
    t[15] = getCPUTime();
    cout << "Tri-Region Partition Method with old median of three pivot finished sorting in " << t[15] - t[14] << " seconds.\n";
    start = getCPUTime();
    quicksort(Y, 0, size - 1, triRegionPart, medianOfThree);
    stop = getCPUTime();
    cout << "Tri-Region Partition Method with new median of three pivot finished sorting in " << stop - start << " seconds.\n\n";
  }
  if (choice == 3 || choice == 4) {
    t[16] = getCPUTime();
    quicksort(I, 0, size - 1, twoRegionPart, firMed);
    t[17] = getCPUTime();
    cout << "Two-Region Partition Method with first element pivot finished sorting in " << t[17] - t[16] << " seconds.\n";
    t[18] = getCPUTime();
    quicksort(J, 0, size - 1, twoRegionPart, midMed);
    t[19] = getCPUTime();
    cout << "Two-Region Partition Method with middle element pivot finished sorting in " << t[19] - t[18] << " seconds.\n";
    t[20] = getCPUTime();
    quicksort(K, 0, size - 1, twoRegionPart, endMed);
    t[21] = getCPUTime();
    cout << "Two-Region Partition Method with last element pivot finished sorting in " << t[21] - t[20] << " seconds.\n";
    t[22] = getCPUTime();
    quicksort(L, 0, size - 1, twoRegionPart, medOfThree);
    t[23] = getCPUTime();
    cout << "Two-Region Partition Method with old median of three pivot finished sorting in " << t[23] - t[22] << " seconds.\n";
    start = getCPUTime();
    quicksort(Z, 0, size - 1, twoRegionPart, medOfThree);
    stop = getCPUTime();
    cout << "Two-Region Partition Method with new median of three pivot finished sorting in " << stop - start << " seconds.\n\n";
  }
  t[24] = getCPUTime();
  sort(check, check + size);
  t[25] = getCPUTime();
  cout << "First check (sort(...) from <algorithm>) finished sorting in " << t[25] - t[24] << " seconds.\n";
  t[26] = getCPUTime();
  qsort(check2, size, sizeof(string), compare);
  t[27] = getCPUTime();
  cout << "Second check (qsort(...) from <stdlib.h>) finished sorting in " << t[27] - t[26] << " seconds.\n";
}

//Handles the check of all the different versions of the sorts against the check arrays given the appropriate arrays (A-L), (size),
//   and the (choice) of which sorts were run
//No return, arrays will not be modified
//choice: 1 = original partition, 2 = tri-region partition, 3 = two-region partition, 4 = all partition methods
void checkSorts (string * A, string * B, string * C, string * D, string * E, string * F, string * G,
                 string * H, string * I, string * J, string * K, string * L, string * check, string * check2,
                 string * X, string * Y, string * Z,
                 const int choice, const int size) {
  cout << endl;
  bool equal = true;
  for (int i = 0; i < size; i++) {
    if (check[i] != check2[i]) {
      cout << "The checks don't match up...\n";
      exit(1);
    }
    if (choice == 1 || choice == 4) {
      if (A[i] != check[i] || A[i] != check2[i]) {
        equal = false;
        cout << "A ";
      }
      if (B[i] != check[i] || B[i] != check2[i]) {
        equal = false;
        cout << "B ";
      }
      if (C[i] != check[i] || C[i] != check2[i]) {
        equal = false;
        cout << "C ";
      }
      if (D[i] != check[i] || D[i] != check2[i]) {
        equal = false;
        cout << "D ";
      }
      if (X[i] != check[i] || X[i] != check2[i]) {
        equal = false;
        cout << "X ";
      }
    }
    if (choice == 2 || choice == 4) {
      if (E[i] != check[i] || E[i] != check2[i]) {
        equal = false;
        cout << "E ";
      }
      if (F[i] != check[i] || F[i] != check2[i]) {
        equal = false;
        cout << "F ";
      }
      if (G[i] != check[i] || G[i] != check2[i]) {
        equal = false;
        cout << "G ";
      }
      if (H[i] != check[i] || H[i] != check2[i]) {
        equal = false;
        cout << "H ";
      }
      if (Y[i] != check[i] || Y[i] != check2[i]) {
        equal = false;
        cout << "Y ";
      }
    }
    if (choice == 3 || choice == 4) {
      if (I[i] != check[i] || I[i] != check2[i]) {
        equal = false;
        cout << "I ";
      }
      if (J[i] != check[i] || J[i] != check2[i]) {
        equal = false;
        cout << "J ";
      }
      if (K[i] != check[i] || K[i] != check2[i]) {
        equal = false;
        cout << "K ";
      }
      if (L[i] != check[i] || L[i] != check2[i]) {
        equal = false;
        cout << "L ";
      }
      if (Z[i] != check[i] || Z[i] != check2[i]) {
        equal = false;
        cout << "Z ";
      }
    }
    if (!equal) {
      break;
    }
  }
  if (equal) {
    cout << "All methods of sort passed both of the checks.\n";
  } else {
    cout << "failed at least one of the checks.\n";
    exit(1);
  }
}

//Returns true if the input (s) is a positive integer and false if not
bool isNum (const string s) {
  for (int i = 0; i < s.length(); i++) {
    if (s[i] > '9' || s[i] < '0') {
      return false;
    }
  }
  return true;
}

//Handles the input for how many strings to fit the arrays with (size) and the (length) of each as well as the associated errors
//size and length modified by reference
void input (int & size, int & length) {
  string inp;
  cout << "Please enter a positive integer value for the number of strings to sort: ";
  cin >> inp;
  if (isNum(inp)) {
    size = atoi(inp.c_str());
    if (size == 0) {
      cout << "Invalid entry, need a positive number of strings.\n";
      return input(size, length);
    }
  } else {
    cout << "Invalid response.\n";
    return input(size, length);
  }

  cout << "\nPlease enter a positive integer value for the length of each string (keep this below 20): ";
  cin >> inp;
  if (isNum(inp)) {
    length = atoi(inp.c_str());
    if (length > 20 || length <= 0) {
      cout << "Invalid size.\n";
      return input(size, length);
    }
  } else {
    cout << "Invalid response.\n";
    return input(size, length);
  }  
}

//Handles user input for choosing which sort(s) should be performed and returns the proper code:
//choice: 1 = original partition, 2 = tri-region partition, 3 = two-region partition, 4 = all partition methods
int sortChoice () {
  cout << "\nWhich version(s) of quicksort would you like to see?\n  1. Original Partition\n  2. Tri-Region Partition\n"
       << "  3. Two-Region Partition\n  4. All of the above\n";
  string inp;
  cin >> inp;
  if (isNum(inp)) {
    int choice = atoi(inp.c_str());
    if (choice <= 4 && choice >= 1) {
      return choice;
    }
  }
  cout << "Invalid response, please try again.\n";
  return sortChoice();
}

/**
 * Handles all of the running all variations of quicksort SEQUENTIALLY
 *  - TODO: streamline into something more modular and perhaps perform
 *          sorts in batches (no bulk memory allocation as is done below)
 */
void oldRun (string * Master, const int size) {
  const int choice = sortChoice();

  double start = getCPUTime();
//Array memory allocation
  string * A = new string[size], * B = new string[size], * C = new string[size], * D = new string[size],
         * E = new string[size], * F = new string[size], * G = new string[size], * H = new string[size],
         * I = new string[size], * J = new string[size], * K = new string[size], * L = new string[size],
         * X = new string[size], * Y = new string[size], * Z = new string[size];

  string * check = new string[size], * check2 = new string[size];
  
//Assigning the same strings to all of the arrays for each element
  for (int i = 0; i < size; i++) {
    A[i] = B[i] = C[i] = D[i] = E[i] = F[i] = G[i] = H[i] = I[i] = J[i] = K[i] = L[i] = check[i] = check2[i] = X[i] = Y[i] = Z[i] = Master[i];
  }

//Running and checking the sorts
  runSorts(A, B, C, D, E, F, G, H, I, J, K, L, check, check2, X, Y, Z, choice, size);
  checkSorts(A, B, C, D, E, F, G, H, I, J, K, L, check, check2, X, Y, Z, choice, size);
  double end = getCPUTime();

  cout << "Entire old process takes " << end - start << " seconds.\n\n";

//deallocate everything
  delete A; delete B; delete C; delete D; delete E; delete F;
  delete G; delete H; delete I; delete J; delete K; delete L;
  delete X; delete Y; delete Z;
  delete check; delete check2;
}

/**
 * Merge sort on an array.
 *  - Time complexity = O(nlog(n))
 *  - Space complexity = O(1)
 *   - Achieved with continuous trading of elements between 2 arrays
 *  - TODO: Separate this into another code file
 */
void MergeSort (string *& master, const int size) {
  string * A = master;
  string * temp = new string[size];
//batch represents the offset between chunks to be compared
// since the array is treated as having been split down to singular
// elements logically and is "built up" from the beginning
  for (int batch = 1; batch < size; batch *= 2) {
    int section1Start = 0;
    int section1End = section1Start + batch - 1;
    int section2Start = section1End + 1;
    int section2End = section2Start + batch - 1 >= size ? size - 1 : section2Start + batch - 1; //Check <= size - 1
    int tempi = 0;
    while (section1Start < size) {
      while (true) {
//Copy over remainder of the other subsection of the array
        if (section1Start > section1End) {
          while(section2Start <= section2End) {
            temp[tempi] = A[section2Start];
            tempi++;
            section2Start++;
          }
          break;
        } else if (section2Start > section2End) {
          while (section1Start <= section1End) {
            temp[tempi] = A[section1Start];
            tempi++;
            section1Start++;
          }
          break;
        }
//Compare the two subsections and move the lower one forward
        if (A[section1Start] <= A[section2Start]) {
          temp[tempi] = A[section1Start];
          section1Start++;
        } else {
          temp[tempi] = A[section2Start];
          section2Start++;
        }
        tempi++;
      }
//Move on to the next subsections in the same batch
      section1Start = section2End + 1;
      if (section1Start + batch - 1 >= size) {
        section1End = size - 1;   //check <= size - 1
        section2Start = size;
        section2End = size - 1;
      } else {
        section1End = section1Start + batch - 1;
        section2Start = section1End + 1;
        section2End = section2Start + batch - 1 >= size ? size - 1 : section2Start + batch - 1;   //check <= size - 1
      }
    }
/* Printing for debugging
    cout << "A: " << A << " master: " << master << " temp: " << temp << " batch: " << batch << endl;
    for (int i = 0; i < size; i++) {
      cout << "A[" << i << "]: " << A[i] << " temp[" << i << "]: " << temp[i] << endl;
    }
*/
//Swap the arrays so the next round copies into the other array (improves space complexity)
    string * swap = temp;
    temp = A;
    A = swap;
  }
//  cout << "A: " << A << " master: " << master << " temp: " << temp << endl;
//Make sure the right array is returned (this is why master is a reference pointer,
// otherwise we would have to copy the entire contents of the array here)
  delete[] temp;
  if (A != master) {
    master = A;
  }
}

int main() {
//  sranddev();		Not compatible with Cygwin?
  srand(time(NULL));

//User input for size of arrays, string length, and sort choice
  int length, size;
  input(size, length);

  string * Master = new string[size];
  for (int i = 0; i < size; i++) {
    Master[i] = randomString(length);
  }

  oldRun(Master, size);

// Coming soon...
//  heap<string, Heap::MAX> h(size);
//  tree<string> t;
  string * check = new string[size], * merge_data = new string[size];
  string * * Master_Copy = new string * [size];
  for (int i = 0; i < size; i++) {
    check[i] = merge_data[i] = Master[i];
    Master_Copy[i] = new string(Master[i]);
  }
  sort(check, check + size);
  double t1, t2, t3;
  bool flag;
/* TODO: Separate code files for the lot of these
  t1 = getCPUTime();
  for (int i = 0; i < size; i++) {
    h.push(Master[i]);
  }
  t2 = getCPUTime();
  string * data = h.sort();
  t3 = getCPUTime();

  cout << "\nHeap sort total time: " << t3 - t1 << " seconds, sort time: " << t3 - t2 << " seconds, load time: " << t2 - t1 << " seconds.\n";
  flag = true;
  for (int i = 0; i < size; i++) {
    if (check[i] != data[i]) {
      flag = false;
      break;
    }
  }
  if (flag == true) {
    cout << "Heap sort passed the check\n\n";
  }

  t1 = getCPUTime();
  for (int i = 0; i < size; i++) {
    t.push(Master_Copy[i]);
  }
  t2 = getCPUTime();
  vector<string *> tree_data = t.sort();
  t3 = getCPUTime();

  cout << "Tree sort total time: " << t3 - t1 << " seconds, sort time: " << t3 - t2 << " seconds, load time: " << t2 - t1 << " seconds.\n";
  flag = true;
  for (int i = 0; i < size; i++) {
    if (check[i] != *(tree_data[i])) {
      flag = false;
      break;
    }
  }
  if (flag == true) {
    cout << "Tree sort passed the check\n\n";
  }
*/
  t1 = getCPUTime();
  MergeSort(merge_data, size);
  t2 = getCPUTime();

  cout << "Merge sort on array finished in " << t2 - t1 << " seconds.\n";
  flag = true;
  for (int i = 0; i < size; i++) {
    if (check[i] != merge_data[i]) {
      flag = false;
      cout << i << endl;
    }
  }
  if (flag == true) {
    cout << "Merge sort passed the check\n\n";
  } else {
    cout << "Merge sort failed the check\n\n";
  }
//  handleAll(Master, size);    //async implementation needs to be fixed

  delete Master;
  delete Master_Copy;
  delete check;

  return 0;
}
