#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <sys/resource.h>

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
    result += alphabet[random() % (alphabet.length() - 1)];
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

//Handles the running of all the different versions of the sorts given the appropriate arrays (A-L, and checks), size,  and the 
//  (choice) of which block(s) to run
//No return, but arrays will be modified by reference depending on choice input
//choice: 1 = original partition, 2 = tri-region partition, 3 = two-region partition, 4 = all partition methods
void runSorts (string * A, string * B, string * C, string * D, string * E, string * F, string * G,
               string * H, string * I, string * J, string * K, string * L, string * check, string * check2,
               const int choice, const int size) {

  double t[30];

  if (choice == 1 || choice == 4) {
    t[0] = getCPUTime();
    quicksort(A, 0, size - 1, origPart, firMed);
    t[1] = getCPUTime();
    cout << "\nOriginal Partition Method with first element pivot finished sorting in " << t[1] - t[0] << " seconds.\n";
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
    cout << "Original Partition Method with median of three pivot finished sorting in " << t[7] - t[6] << " seconds.\n\n";
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
    cout << "Tri-Region Partition Method with median of three pivot finished sorting in " << t[15] - t[14] << " seconds.\n\n";
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
    cout << "Two-Region Partition Method with median of three pivot finished sorting in " << t[23] - t[22] << " seconds.\n\n";
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

int main() {
  sranddev();

//User input for size of arrays, string length, and sort choice
  int length, size;
  input(size, length);
  const int choice = sortChoice();

//Array memory allocation
  string * A = new string[size], * B = new string[size], * C = new string[size], * D = new string[size],
         * E = new string[size], * F = new string[size], * G = new string[size], * H = new string[size],
         * I = new string[size], * J = new string[size], * K = new string[size], * L = new string[size];

  string * check = new string[size], * check2 = new string[size];
  
//Assigning the same strings to all of the arrays for each element
  for (int i = 0; i < size; i++) {
    A[i] = B[i] = C[i] = D[i] = E[i] = F[i] = G[i] = H[i] = I[i] = J[i] = K[i] = L[i] = check[i] = check2[i] = randomString(length);
  }

//Running and checking the sorts
  runSorts(A, B, C, D, E, F, G, H, I, J, K, L, check, check2, choice, size);
  checkSorts(A, B, C, D, E, F, G, H, I, J, K, L, check, check2, choice, size);

//Should deallocate memory for the above arrays, but program ends here so whatever

  return 0;
}
