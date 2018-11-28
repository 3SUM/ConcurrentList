#include "coarseList.h"
#include "fineList.h"
#include "optimisticList.h"
#include "lazyList.h"
#include <iostream>
#include <thread>
#include <sstream>

using namespace std;

coarseList<int> cList;
fineList<int> fList;
optimisticList<int> oList;
lazyList<int> lList;

void coarseFunction(int i) {
    for(int j = 0; j < 10000; j++)
        if(!cList.add(j+ i*10000))
            cout << "Coarse Grained add error." << endl;

    for(int j = 0; j < 10000; j++)
        if(!cList.contains(j+ i*10000))
            cout << "Coarse Grained contains error." << endl;

    for(int j = 0; j < 10000; j++)
        if(!cList.remove(j+ i*10000))
            cout << "Coarse Grained remove error." << endl;
}

void fineFunction(int i) {
    for(int j = 0; j < 10000; j++)
        if(!fList.add(j+ i*10000))
            cout << "Fine Grained add error." << endl;

    for(int j = 0; j < 10000; j++)
        if(!fList.contains(j+ i*10000))
            cout << "Fine Grained contains error." << endl;

    for(int j = 0; j < 10000; j++)
        if(!fList.remove(j+ i*10000))
            cout << "Fine Grained remove error." << endl;
}

void optimisticFunction(int i) {
    for(int j = 0; j < 10000; j++)
        if(!oList.add(j+ i*10000))
            cout << "Optimistic Synchronization add error." << endl;

    for(int j = 0; j < 10000; j++)
        if(!oList.contains(j+ i*10000))
            cout << "Optimistic Synchronization contains error." << endl;

    for(int j = 0; j < 10000; j++)
        if(!oList.remove(j+ i*10000))
            cout << "Optimistic Synchronization remove error." << endl;
}

void lazyFunction(int i) {
    for(int j = 0; j < 10000; j++)
        if(!lList.add(j+ i*10000))
            cout << "Lazy Synchronization add error." << endl;

    for(int j = 0; j < 10000; j++)
        if(!lList.contains(j+ i*10000))
            cout << "Lazy Synchronization contains error." << endl;

    for(int j = 0; j < 10000; j++)
        if(!lList.remove(j+ i*10000))
            cout << "Lazy Synchronization remove error." << endl;
}

int main(int argc, char *argv[])
{
// *****************************************************************
//  Data declarations...

    const char	*bold = "\033[1m";
    const char	*unbold = "\033[0m";
    string		stars = "";
    string      bars = "";
    int threadCount = 0;
    stringstream mySS;

    stars.append(65, '*');
    bars.append(60, '-');

// *****************************************************************
//  Get/verify command line arguments.
//	Error out if bad arguments...

    if(argc != 2) {
        cout << "Usage: ./linktest <threadCount>" << endl;
        exit(1);
    }

    // Get threadCount from argv
    mySS << argv[1];
    mySS >> threadCount;

    if(threadCount <= 0) {
        cout << "Error: Invalid thread count." << endl;
        exit(1);
    }

    // Dynamically allocate threadCount threads
    thread *t1 = new thread[threadCount];
    thread *t2 = new thread[threadCount];
    thread *t3 = new thread[threadCount];
    thread *t4 = new thread[threadCount];

// *****************************************************************
// Display header

	cout << stars << endl << bold << "Concurrent Linked List Tests" <<
	unbold << endl << endl;

// *****************************************************************
// Coarse Grained Linked List

    cout << bars << endl << bold << "Coarse Grained Linked List" << unbold <<
    unbold << endl;

    // Start time
    auto c1 = chrono::high_resolution_clock::now();

    // Start up threads...
    for(int i = 0; i < threadCount; i++)
        t1[i] = thread(&coarseFunction, i);

    // Wait for threads to finish...
    for(int i = 0; i < threadCount; i++)
        t1[i].join();

    // End time
    auto c2 = chrono::high_resolution_clock::now();

    cout << "   Run-time: " << chrono::duration_cast<chrono::milliseconds>(c2 - c1).count()
         << " milliseconds" << endl << endl;

// *****************************************************************
// Fine Grained Linked List

     cout << bars << endl << bold << "Fine Grained Linked List" << unbold <<
     unbold << endl;

     // Start time
     auto f1 = chrono::high_resolution_clock::now();

     // Start up threads...
     for(int i = 0; i < threadCount; i++)
         t2[i] = thread(&fineFunction, i);

     // Wait for threads to finish...
     for(int i = 0; i < threadCount; i++)
         t2[i].join();

     // End time
     auto f2 = chrono::high_resolution_clock::now();

     cout << "   Run-time: " << chrono::duration_cast<chrono::milliseconds>(f2 - f1).count()
          << " milliseconds" << endl << endl;

// *****************************************************************
// Optimistic Synchronization Linked List

    cout << bars << endl << bold << "Optimistic Synchronization Linked List" << unbold <<
    unbold << endl;

    // Start time
    auto o1 = chrono::high_resolution_clock::now();

    // Start up threads...
    for(int i = 0; i < threadCount; i++)
        t3[i] = thread(&optimisticFunction, i);

    // Wait for threads to finish...
    for(int i = 0; i < threadCount; i++)
        t3[i].join();

    // End time
    auto o2 = chrono::high_resolution_clock::now();

    cout << "   Run-time: " << chrono::duration_cast<chrono::milliseconds>(o2 - o1).count()
         << " milliseconds" << endl << endl;

// *****************************************************************
// Lazy Synchronization Linked List

   cout << bars << endl << bold << "Lazy Synchronization Linked List" << unbold <<
   unbold << endl;

   // Start time
   auto l1 = chrono::high_resolution_clock::now();

   // Start up threads...
   for(int i = 0; i < threadCount; i++)
        t4[i] = thread(&lazyFunction, i);

   // Wait for threads to finish...
   for(int i = 0; i < threadCount; i++)
        t4[i].join();

   // End time
   auto l2 = chrono::high_resolution_clock::now();

   cout << "   Run-time: " << chrono::duration_cast<chrono::milliseconds>(l2 - l1).count()
        << " milliseconds" << endl << endl;

// *****************************************************************
// All done...

    delete [] t1;
    delete [] t2;
    delete [] t3;
    delete [] t4;
}
