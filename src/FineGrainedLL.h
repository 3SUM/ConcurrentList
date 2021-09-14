/*************************************************************************
 * Luis Maya
 * Fine-Grained Synchronization Linked List
 * We can improve concurrency by locking individual nodes, rather than
 * locking the list as a whole. Instead of placing a lock on the entire list
 * let us add a lock to each node. As a thread traverses the list, it locks
 * each node when it first visits, and sometime later releases it. Such
 * fine-grained locking permits concurrent threads to traverse the list
 * together in a pipelined fashion.
 * **********************************************************************/
#include <iostream>
#include <mutex>
#include <cstdlib>

using namespace std;

template <class T>
class FineGrainedLL
{
public:
    FineGrainedLL();
    ~FineGrainedLL();
    bool contains(T);
    bool add(T);
    bool remove(T);
    void printList();
    void deleteList();
private:
    struct Node {
        T key;
        Node *next;
        mutex lock;
    };
    Node *head;
    Node *tail;
};

/*************************************************************************
 * Initialize class variables
 * Head and tail will be used as a sentinel nodes
 * **********************************************************************/
template <class T>
FineGrainedLL<T> :: FineGrainedLL()
{
    head = new Node;
    head->key = {};

    tail = new Node;
    tail->key = {};
    tail->next = NULL;

    head->next = tail;
}

/*************************************************************************
 * Deallocate linked list memory
 * **********************************************************************/
template <class T>
FineGrainedLL<T> :: ~FineGrainedLL()
{
    deleteList();

    delete head;
    delete tail;
}

/*************************************************************************
 * Uses fine grained locking to check if the given parameter is in
 * the linked list. If found, return true, else return false.
 * **********************************************************************/
template <class T>
bool FineGrainedLL<T> :: contains(T key)
{
    // Acquire init(head) pred node lock
    head->lock.lock();
    Node *pred = head;

    // Acquire curr node lock
    pred->next->lock.lock();
    Node *curr = head->next;

    // While not at the of the linked list
    while(curr != tail) {
        // If current key is >= key, break out of traversal
        if(curr->key >= key)
            break;

        // Release pred lock and update pred to curr
        pred->lock.unlock();
        pred = curr;

        // Acquire next node lock and set curr to next node
        curr->next->lock.lock();
        curr = curr->next;
    }

    // If key was not found, release pred and curr locks, and return false
    if(curr != tail && curr->key != key) {
        pred->lock.unlock();
        curr->lock.unlock();
        return false;
    }

    // If key was found, release pred and curr locks, and return true
    pred->lock.unlock();
    curr->lock.unlock();

    return true;
}

/*************************************************************************
 * Uses fine grained locking to attempt to add the given parameter.
 * If the parameter is already in the linked list, do not add again and
 * return false. If parameter is not already in the linked list, add node
 * and return true.
 * **********************************************************************/
template <class T>
bool FineGrainedLL<T> :: add(T key)
{
    // Acquire init(head) pred node lock
    head->lock.lock();
    Node *pred = head;

    // Acquire curr node lock
    pred->next->lock.lock();
    Node *curr = head->next;

    // While not at the of the linked list
    while(curr != tail) {
        // If current key is >= key, break out of traversal
        if(curr->key >= key)
            break;

        // Release pred lock and update pred to curr
        pred->lock.unlock();
        pred = curr;

        // Acquire next node lock and set curr to next node
        curr->next->lock.lock();
        curr = curr->next;
    }

    // If key is already inserted, release pred and curr locks, and return false
    if(curr != tail && curr->key == key) {
        pred->lock.unlock();
        curr->lock.unlock();
        return false;
    }

    // If key is not in the list, add node, release locks and return true
    Node *node = new Node;
    node->key = key;
    node->next = curr;
    pred->next = node;

    pred->lock.unlock();
    curr->lock.unlock();

    return true;
}

/*************************************************************************
 * Uses fine grained locking to attempt to remove the give parameter.
 * If the parameter is not found in the linked list, return false. If the
 * parameter is found in the linked list, remove it and return true.
 * **********************************************************************/
template <class T>
bool FineGrainedLL<T> :: remove(T key)
{
    // Acquire init(head) pred node lock
    head->lock.lock();
    Node *pred = head;

    // Acquire curr node lock
    pred->next->lock.lock();
    Node *curr = head->next;

    // While not at the of the linked list
    while(curr != tail) {
        // If current key is >= key, break out of traversal
        if(curr->key >= key)
            break;

        // Release pred lock and update pred to curr
        pred->lock.unlock();
        pred = curr;

        // Acquire next node lock and set curr to next node
        curr->next->lock.lock();
        curr = curr->next;
    }

    // If key was not found, release pred and curr locks, and return false
    if(curr != tail && curr->key != key) {
        pred->lock.unlock();
        curr->lock.unlock();
        return false;
    }

    // If key was found, remove it, release locks, and return true
    //Node *temp;
    //temp = curr;
    pred->next = curr->next;
    //delete temp;

    pred->lock.unlock();
    curr->lock.unlock();

    return true;
}

/*************************************************************************
 * Display contents of linked list
 * **********************************************************************/
template <class T>
void FineGrainedLL<T> :: printList()
{
    // Acquire head lock
    head->lock.lock();

    // Set curr to head->next since head is a sentinel node
    Node *curr = head->next;

    // Traverse linked list and display contents
    while(curr != tail) {
        cout << curr->key << " ";
        curr = curr->next;
    }

    // Release head lock
    head->lock.unlock();
}

/*************************************************************************
 * Delete contents of linked list
 * **********************************************************************/
template <class T>
void FineGrainedLL<T> :: deleteList()
{
    Node *temp;

    while(head->next != tail) {
        temp = head->next;
        head->next = temp->next;
        delete temp;
    }

    head = new Node;
    head->key = {};

    tail = new Node;
    tail->key = {};
    tail->next = NULL;

    head->next = tail;
}
