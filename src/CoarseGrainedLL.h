/*************************************************************************
 * Luis Maya
 * Coarse-Grained Synchronization Linked List
 * The list itself has a single lock which every method call must acquire.
 * The principle advantage of this algorithm, is that it is thread safe.
 * All methods act on the list only while holding the lock, so the execution
 * is essentially sequential.
 * **********************************************************************/
#include <iostream>
#include <mutex>
#include <cstdlib>

using namespace std;


template <class T>
class CoarseGrainedLL
{
public:
    CoarseGrainedLL();
    ~CoarseGrainedLL();
    bool contains(T);
    bool add(T);
    bool remove(T);
    void printList();
    void deleteList();
private:
    struct Node {
        T key;
        Node *next;
    };
    Node *head;
    mutex lock;
};

/*************************************************************************
 * Initialize class variables
 * Head will be used as a sentinel node
 * **********************************************************************/
template <class T>
CoarseGrainedLL<T> :: CoarseGrainedLL()
{
    head = new Node;
    head->key = {};
    head->next = NULL;
}

/*************************************************************************
 * Deallocate linked list memory
 * **********************************************************************/
template <class T>
CoarseGrainedLL<T> :: ~CoarseGrainedLL()
{
    deleteList();

    delete head;
}

/*************************************************************************
 * Uses coarse grained locking to check if the given parameter is in
 * the linked list. If found, return true, else return false.
 * **********************************************************************/
template <class T>
bool CoarseGrainedLL<T> :: contains(T key)
{
    // Acquire lock
    lock_guard<mutex>guard(lock);

    //Node *pred = head;
    Node *curr = head->next;

    // While not at the of the linked list
    while(curr != NULL) {
        // If current key is >= key, break out of traversal
        if(curr->key >= key)
            break;

        // Set pred to curr node
        //pred = curr;

        // Set curr to next node
        curr = curr->next;
    }

    // If key was not found, return false
    if(curr != NULL && curr->key != key)
        return false;

    // If key was found, return true
    return true;
}

/*************************************************************************
 * Uses coarse grained locking to attempt to add the given parameter.
 * If the parameter is already in the linked list, do not add again and
 * return false. If parameter is not already in the linked list, add node
 * and return true.
 * **********************************************************************/
template <class T>
bool CoarseGrainedLL<T> :: add(T key)
{
    // Acquire lock
    lock_guard<mutex>guard(lock);

    Node *pred = head;
    Node *curr = head->next;

    // While not at the of the linked list
    while(curr != NULL) {
        // If current key is >= key, break out of traversal
        if(curr->key >= key)
            break;

        // Set pred to curr node
        pred = curr;

        // Set curr to next node
        curr = curr->next;
    }

    // If key is already in the list, don't add and return false
    if(curr != NULL && curr->key == key)
        return false;

    // Add key to list and return true
    Node *node = new Node;
    node->key = key;
    node->next = curr;
    pred->next = node;

    return true;
}

/*************************************************************************
 * Uses coarse grained locking to attempt to remove the give parameter.
 * If the parameter is not found in the linked list, return false. If the
 * parameter is found in the linked list, remove it and return true.
 * **********************************************************************/
template <class T>
bool CoarseGrainedLL<T> :: remove(T key)
{
    // Acquire lock
    lock_guard<mutex>guard(lock);

    Node *pred = head;
    Node *curr = head->next;

    // While not at the of the linked list
    while(curr != NULL) {
        // If current key is >= key, break out of traversal
        if(curr->key >= key)
            break;

        // Set pred to curr node
        pred = curr;

        // Set curr to next node
        curr = curr->next;
    }

    // If key was not found, return false
    if(curr != NULL && curr->key != key)
        return false;

    // Remove key and return true
    Node *temp;
    temp = curr;
    pred->next = curr->next;
    delete temp;
    return true;
}

/*************************************************************************
 * Display contents of linked list
 * **********************************************************************/
template <class T>
void CoarseGrainedLL<T> :: printList()
{
    // Acquire lock
    lock_guard<mutex>guard(lock);

    // Set curr to head->next since head is a sentinel node
    Node *curr = head->next;

    // Traverse linked list and display contents
    while(curr != NULL) {
        cout << curr->key << " ";
        curr = curr->next;
    }
}

/*************************************************************************
 * Delete contents of linked list
 * **********************************************************************/
template <class T>
void CoarseGrainedLL<T> :: deleteList()
{
    Node *temp;

    while(head->next != NULL) {
        temp = head->next;
        head->next = temp->next;
        delete temp;
    }

    head = new Node;
    head->key = {};
    head->next = NULL;
}
