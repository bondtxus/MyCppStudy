#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm> // for sort
#include <chrono>
using namespace std;
using namespace std::chrono;

mutex m; //Object for exclusive control
vector<int> primes; //vector for storing prime numbers

//Determine if argument is prime
bool isPrime(int n) {
  if (n < 2) return false; //If it's less than 2, it's not prime
  if (n == 2) return true; //2 is a prime number

  //If it is divisible by a number greater than or equal to 2 and less than n, it is not a prime number.
  for (int j = 2; j < n; ++j) {
    if (n % j == 0) return false; // not prime number
  }
  return true; // prime number
}

// Use two thread to accelerate process to check all number up to entered.
// The number of 2 and 3 is prime
// Multiples of 3 are not prime numbers (3,6,9...)
// One thread search from the number which multiple of 3 + 1 (4,7,10...)
// Second thread search from the number which multiple of 3 + 2 (5,8,11...)
void threadFunc(int N, int start) {
  for (int n = start; n <= N; n += 3) {
    if (isPrime(n)) {
      unique_lock<mutex> lock(m); // Get mutex
      primes.push_back(n);
    } // mutex unlock
  }
}

//Display the first and last "num" elements
template<typename T>
void report(T first, T last) {
  const int num = 5;
  if (last - first < num) { //Display all if less than num
    cout << "First " << num << " elements : ";
    for (T i = first; i != last; ++i) cout << *i << ", ";
  }
  else {
    cout << "First " << num << " elements : ";
    for (T i = first; i != first + num; ++i) cout << *i << ", ";
    cout << endl;
    cout << "Last " << num << " elements : ";
    for (T i = last - num; i != last; ++i) cout << *i << ", ";
  }
  cout << endl;
}



int main() {

  const int NMAX = 500000; // Max number to enter
  int entry; // The number user entered
  
  cout << "Calculate the number of prime numbers up to entered number." << endl;

  do {
    cout << "Please enter a number " << NMAX << " or less." << endl;
    cin >> entry;
  } while (entry > NMAX);

  // measure the duration of process with high resolution
  auto t0 = high_resolution_clock::now(); // Start time 

  primes.push_back(2); // Put "2" to vector of primes 
  primes.push_back(3); // Put "3" to vector of primes

  thread threadA(threadFunc, entry, 4); // Start prime check from 4 
  thread threadB(threadFunc, entry, 5); // Start prime check from 5

  //wait for thread to finish
  threadA.join();
  threadB.join();

  cout << endl << "The number of prime numbers: " << primes.size() << endl;

  sort(primes.begin(), primes.end());
  report(primes.cbegin(), primes.cend()); // Display first 5, last 5 elementsuse (cbegin(),cend() return const_iterator)

  auto t1 = high_resolution_clock::now(); // End time
  cout << "Calculated time : " << duration_cast<milliseconds>(t1 - t0).count() / 1000. << " sec" << endl << endl;


  ////////////////////// single thread check ///////////////////////
  cout << "Enter 1 to check with single thread otherwise close" << endl;
  int x;
  cin >> x;
  
  if (x != 1) return 0;

  /////////////////////////////////

  primes.clear();

  t0 = high_resolution_clock::now();

  for (int n = 2; n <= entry; ++n) {
    if (isPrime(n)) { 
      primes.push_back(n);
    }
  }

  cout << endl << "The number of prime numbers: " << primes.size() << endl;
  report(primes.cbegin(), primes.cend());

  t1 = high_resolution_clock::now();
  cout << "Calculated time with single thread: " << duration_cast<milliseconds>(t1 - t0).count() / 1000. << " sec" << endl << endl;
  
  // Before close window
  cout << "Enter any key to close window" << endl;
  cin >> x;

  return 0;
}
