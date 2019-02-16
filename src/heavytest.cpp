#include "coarseList.h"
#include "fineList.h"
#include "optimisticList.h"
#include "lazyList.h"
#include "lockFreeList.h"
#include <iostream>
#include <thread>
#include <atomic>

using namespace std;

atomic<int> items;
int STEP  = 10;
int LIMIT = 100;
int THREAD_COUNT = 9;
int THREAD_MULT = 1;

coarseList<int> cList;
fineList<int> fList;
optimisticList<int> oList;
lazyList<int> lList;
lockFreeList<int> xList;

// ****************************************************************************

void cinit(int start) {
    while(start <= LIMIT) {
        if(!cList.add(start)) {
            cout << "add(" << start << ") error." << endl;
        }
        items++;
        start += STEP;
    }
}

void cdeleteFunction(int start) {
    while(start <= LIMIT) {
        if(!cList.remove(start)){
            cout << "remove(" << start << ") error." << endl;
        }
        start += STEP;
    }
}

void csearchFunction(int start) {
    while(start <= LIMIT) {
        if(!cList.contains(start)) {
            cout << "contains(" << start << ") error." << endl;
        }
        start += STEP;
    }
}

void cinsertFunction(int start) {
    while(start <= LIMIT) {
        if(!cList.add(start)) {
            cout << "add(" << start << ") error." << endl;
        }
        start += STEP;
    }
}

// ****************************************************************************

void finit(int start) {
    while(start <= LIMIT) {
        if(!fList.add(start)) {
            cout << "add(" << start << ") error." << endl;
        }
        items++;
        start += STEP;
    }
}

void fdeleteFunction(int start) {
    while(start <= LIMIT) {
        if(!fList.remove(start)){
            cout << "remove(" << start << ") error." << endl;
        }
        start += STEP;
    }
}

void fsearchFunction(int start) {
    while(start <= LIMIT) {
        if(!fList.contains(start)) {
            cout << "contains(" << start << ") error." << endl;
        }
        start += STEP;
    }
}

void finsertFunction(int start) {
    while(start <= LIMIT) {
        if(!fList.add(start)) {
            cout << "add(" << start << ") error." << endl;
        }
        start += STEP;
    }
}

// ****************************************************************************

void oinit(int start) {
    while(start <= LIMIT) {
        if(!oList.add(start)) {
            cout << "add(" << start << ") error." << endl;
        }
        items++;
        start += STEP;
    }
}

void odeleteFunction(int start) {
    while(start <= LIMIT) {
        if(!oList.remove(start)){
            cout << "remove(" << start << ") error." << endl;
        }
        start += STEP;
    }
}

void osearchFunction(int start) {
    while(start <= LIMIT) {
        if(!oList.contains(start)) {
            cout << "contains(" << start << ") error." << endl;
        }
        start += STEP;
    }
}

void oinsertFunction(int start) {
    while(start <= LIMIT) {
        if(!oList.add(start)) {
            cout << "add(" << start << ") error." << endl;
        }
        start += STEP;
    }
}

// ****************************************************************************

void linit(int start) {
    while(start <= LIMIT) {
        if(!lList.add(start)) {
            cout << "add(" << start << ") error." << endl;
        }
        items++;
        start += STEP;
    }
}

void ldeleteFunction(int start) {
    while(start <= LIMIT) {
        if(!lList.remove(start)){
            cout << "remove(" << start << ") error." << endl;
        }
        start += STEP;
    }
}

void lsearchFunction(int start) {
    while(start <= LIMIT) {
        if(!lList.contains(start)) {
            cout << "contains(" << start << ") error." << endl;
        }
        start += STEP;
    }
}

void linsertFunction(int start) {
    while(start <= LIMIT) {
        if(!lList.add(start)) {
            cout << "add(" << start << ") error." << endl;
        }
        start += STEP;
    }
}

// ****************************************************************************

void xinit(int start) {
    while(start <= LIMIT) {
        if(!xList.add(start)) {
            cout << "add(" << start << ") error." << endl;
        }
        items++;
        start += STEP;
    }
}

void xdeleteFunction(int start) {
    while(start <= LIMIT) {
        if(!xList.remove(start)){
            cout << "remove(" << start << ") error." << endl;
        }
        start += STEP;
    }
}

void xsearchFunction(int start) {
    while(start <= LIMIT) {
        if(!xList.contains(start)) {
            cout << "contains(" << start << ") error." << endl;
        }
        start += STEP;
    }
}

void xinsertFunction(int start) {
    while(start <= LIMIT) {
        if(!xList.add(start)) {
            cout << "add(" << start << ") error." << endl;
        }
        start += STEP;
    }
}

// ****************************************************************************
int main(int argc, char *argv[])
{
// *****************************************************************
//  Data declarations...

    const char	*bold = "\033[1m";
    const char	*unbold = "\033[0m";
    string		stars = "";
    string      bars = "";
    char        userOpt;
    bool        keepProcessing = true;

    stars.append(65, '*');
    bars.append(60, '-');

// *****************************************************************
//  Get/verify command line arguments.
//	Error out if bad arguments...

    if(argc != 1) {
        cout << "Usage: ./heavytest" << endl;
        exit(1);
    }

    auto s1 = chrono::high_resolution_clock::now();
    auto s2 = chrono::high_resolution_clock::now();

    unsigned long hwthd = thread::hardware_concurrency();

    thread *t0 = NULL;
    thread *t1 = NULL;

// *****************************************************************
// Display header

	cout << stars << endl << bold << "Concurrent Linked List Tests" <<
	unbold << endl << endl;

// *****************************************************************
// Main...

    while(keepProcessing) {
        cout << bars << endl;
        cout << "Select Option:" << endl;
        cout << "\t'c' - Coarse Grained Synchronization" << endl;
        cout << "\t'f' - Fine Grained Synchronization" << endl;
        cout << "\t'o' - Optimistic Synchronization" << endl;
        cout << "\t'l' - Lazy Synchronization" << endl;
        cout << "\t'x' - Lock Free" << endl;
        cout << "\t'd' - Display machine stats" << endl;
        cout << "\t'n' - Change limit" << endl;
        cout << "\t't' - Change thread count" << endl;
        cout << "\t'r' - Clear screen" << endl;
        cout << "\t'q' - quit" << endl;
        cout << "> ";
        cin >> userOpt;

        switch(userOpt) {
// *****************************************************************
// Coarse Grained Linked List
            case 'c':
                cout << bars << endl << bold << "Coarse Grained Linked List"
                << unbold << unbold << endl;

                // Allocate threads to run
                t0 = new thread[6*THREAD_MULT];
                t1 = new thread[THREAD_COUNT];

                items = 0;
                for(int j = 0; j < THREAD_MULT; j++){
                    for(int i = 0; i < 6; i++)
                        t0[i+j*6] = thread(&cinit, i+STEP+j*9);
                }
                for(int j = 0; j < THREAD_MULT; j++){
                    for(int i = 0; i < 6; i++)
                        t0[i+6*j].join();
                }
                cout << "Item count: " << items << endl;

                // Start time
                s1 = chrono::high_resolution_clock::now();

                // Start up threads...
                for(int i = 0; i < THREAD_COUNT; i++) {
                    if((i/3)%3 == 0)
                        t1[i] = thread(&cdeleteFunction, i+STEP);
                    else if((i/3)%3 == 1)
                        t1[i] = thread(&csearchFunction, i+STEP);
                    else if((i/3)%3 == 2)
                        t1[i] = thread(&cinsertFunction, i+STEP);
                }

                // Wait for threads to finish...
                for(int i = 0; i < THREAD_COUNT; i++)
                    t1[i].join();

                // End time
                s2 = chrono::high_resolution_clock::now();

                cout << "   Run-time: " << chrono::duration_cast<chrono::milliseconds>(s2 - s1).count()
                     << " milliseconds" << endl << endl;

                cList.deleteList();

                delete [] t0;
                delete [] t1;

                break;

// *****************************************************************
// Fine Grained Linked List
            case 'f':
                cout << bars << endl << bold << "Fine Grained Linked List"
                << unbold << unbold << endl;

                // Allocate threads to run
                t0 = new thread[6*THREAD_MULT];
                t1 = new thread[THREAD_COUNT];

                items = 0;
                for(int j = 0; j < THREAD_MULT; j++){
                    for(int i = 0; i < 6; i++)
                        t0[i+j*6] = thread(&finit, i+STEP+j*9);
                }
                for(int j = 0; j < THREAD_MULT; j++){
                    for(int i = 0; i < 6; i++)
                        t0[i+6*j].join();
                }

                cout << "Item count: " << items << endl;

                // Start time
                s1 = chrono::high_resolution_clock::now();

                // Start up threads...
                for(int i = 0; i < THREAD_COUNT; i++) {
                    if((i/3)%3 == 0)
                        t1[i] = thread(&fdeleteFunction, i+STEP);
                    else if((i/3)%3 == 1)
                        t1[i] = thread(&fsearchFunction, i+STEP);
                    else if((i/3)%3 == 2)
                        t1[i] = thread(&finsertFunction, i+STEP);
                }

                // Wait for threads to finish...
                for(int i = 0; i < THREAD_COUNT; i++)
                    t1[i].join();

                // End time
                s2 = chrono::high_resolution_clock::now();

                cout << "   Run-time: " << chrono::duration_cast<chrono::milliseconds>(s2 - s1).count()
                     << " milliseconds" << endl << endl;

                fList.deleteList();

                delete [] t0;
                delete [] t1;

                break;

// *****************************************************************
// Optimistic Synchronization Linked List
            case 'o':
                cout << bars << endl << bold << "Optimistic Synchronization Linked List"
                << unbold << unbold << endl;

                // Allocate threads to run
                t0 = new thread[6*THREAD_MULT];
                t1 = new thread[THREAD_COUNT];

                items = 0;
                for(int j = 0; j < THREAD_MULT; j++){
                    for(int i = 0; i < 6; i++)
                        t0[i+j*6] = thread(&oinit, i+STEP+j*9);
                }
                for(int j = 0; j < THREAD_MULT; j++){
                    for(int i = 0; i < 6; i++)
                        t0[i+6*j].join();
                }

                cout << "Item count: " << items << endl;

                // Start time
                s1 = chrono::high_resolution_clock::now();

                // Start up threads...
                for(int i = 0; i < THREAD_COUNT; i++) {
                    if((i/3)%3 == 0)
                        t1[i] = thread(&odeleteFunction, i+STEP);
                    else if((i/3)%3 == 1)
                        t1[i] = thread(&osearchFunction, i+STEP);
                    else if((i/3)%3 == 2)
                        t1[i] = thread(&oinsertFunction, i+STEP);
                }

                // Wait for threads to finish...
                for(int i = 0; i < THREAD_COUNT; i++)
                    t1[i].join();

                // End time
                s2 = chrono::high_resolution_clock::now();

                cout << "   Run-time: " << chrono::duration_cast<chrono::milliseconds>(s2 - s1).count()
                     << " milliseconds" << endl << endl;

                oList.deleteList();

                delete [] t0;
                delete [] t1;

                break;

// *****************************************************************
// Lazy Synchronization Linked List
            case 'l':
                cout << bars << endl << bold << "Lazy Synchronization Linked List"
                << unbold << unbold << endl;

                // Allocate threads to run
                t0 = new thread[6*THREAD_MULT];
                t1 = new thread[THREAD_COUNT];

                items = 0;
                for(int j = 0; j < THREAD_MULT; j++){
                    for(int i = 0; i < 6; i++)
                        t0[i+j*6] = thread(&linit, i+STEP+j*9);
                }
                for(int j = 0; j < THREAD_MULT; j++){
                    for(int i = 0; i < 6; i++)
                        t0[i+6*j].join();
                }

                cout << "Item count: " << items << endl;

                // Start time
                s1 = chrono::high_resolution_clock::now();

                // Start up threads...
                for(int i = 0; i < THREAD_COUNT; i++) {
                    if((i/3)%3 == 0)
                        t1[i] = thread(&ldeleteFunction, i+STEP);
                    else if((i/3)%3 == 1)
                        t1[i] = thread(&lsearchFunction, i+STEP);
                    else if((i/3)%3 == 2)
                        t1[i] = thread(&linsertFunction, i+STEP);
                }

                // Wait for threads to finish...
                for(int i = 0; i < THREAD_COUNT; i++)
                    t1[i].join();

                // End time
                s2 = chrono::high_resolution_clock::now();

                cout << "   Run-time: " << chrono::duration_cast<chrono::milliseconds>(s2 - s1).count()
                     << " milliseconds" << endl << endl;

                lList.deleteList();

                delete [] t0;
                delete [] t1;

                break;

// *****************************************************************
// Lock Free Linked List
            case 'x':
                cout << bars << endl << bold << "Lock Free Linked List"
                << unbold << unbold << endl;

                // Allocate threads to run
                t0 = new thread[6*THREAD_MULT];
                t1 = new thread[THREAD_COUNT];

                items = 0;
                for(int j = 0; j < THREAD_MULT; j++){
                    for(int i = 0; i < 6; i++)
                        t0[i+j*6] = thread(&xinit, i+STEP+j*9);
                }
                for(int j = 0; j < THREAD_MULT; j++){
                    for(int i = 0; i < 6; i++)
                        t0[i+6*j].join();
                }

                cout << "Item count: " << items << endl;

                // Start time
                s1 = chrono::high_resolution_clock::now();

                // Start up threads...
                for(int i = 0; i < THREAD_COUNT; i++) {
                    if((i/3)%3 == 0)
                        t1[i] = thread(&xdeleteFunction, i+STEP);
                    else if((i/3)%3 == 1)
                        t1[i] = thread(&xsearchFunction, i+STEP);
                    else if((i/3)%3 == 2)
                        t1[i] = thread(&xinsertFunction, i+STEP);
                }

                // Wait for threads to finish...
                for(int i = 0; i < THREAD_COUNT; i++)
                    t1[i].join();

                // End time
                s2 = chrono::high_resolution_clock::now();

                cout << "   Run-time: " << chrono::duration_cast<chrono::milliseconds>(s2 - s1).count()
                     << " milliseconds" << endl << endl;

                xList.deleteList();

                delete [] t0;
                delete [] t1;

                break;

// *****************************************************************
// quit, clear and default cases
            case 'd':
                cout << bars << endl << bold << "Machine Statistics"
                << unbold << unbold << endl;

                cout << "\tHardware Cores: " << hwthd << endl;
                cout << "\tThread Count: " << THREAD_COUNT << endl;
                cout << "\tLimit: " << LIMIT << endl;
                break;
            case 'n':
                cout << bars << endl << bold << "Change limit"
                << unbold << unbold << endl;

                cout << "\tCurrent limit: " << LIMIT << endl
                     << "\tEnter new limit > ";
                cin >> LIMIT;
                cout << "\tNew limit: " << LIMIT << endl;
                break;
            case 't':
                cout << bars << endl << bold << "Change thread count"
                << unbold << unbold << endl;

                cout << "\tCurrent thread count: " << THREAD_COUNT << endl
                     << "\tEnter thread count > ";
                cin >> THREAD_MULT;
                THREAD_COUNT = 9*THREAD_MULT;
                STEP = THREAD_COUNT + 1;
                cout << "\tNew thread count (x9): " << THREAD_COUNT << endl;
                break;
            case 'q':
                keepProcessing = false;
                cout << endl;
                break;
            case 'r':
                cout << "\033[2J\033[1;1H";
                break;
            default:
                cout << "Error, invalid selection. "
                     << "Please retry" << endl;
                break;
        }
    }

// *****************************************************************
// All done...

    return 0;
}
