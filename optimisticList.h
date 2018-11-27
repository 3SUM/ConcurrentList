/*************************************************************************
 * Luis Maya
 * Optimistic Synchronization Linked List
 * Although fine-grained locking is an improvement over single, coarse-grained
 * lock, it still imposes a potentially long sequence of lock acquisitions
 * and releases. Moreover, threads accessing disjoint parts of the list may
 * still block one another. One way to reduce synchronization costs is to:
 * search without acquiring locks, lock the nodes found, then confirm that
 * the locked nodes are correct. If a synchronization conflict causes the wrong
 * nodes to be locked, then release the locks and start over. Normally this
 * kind of conflict is rare.
 * **********************************************************************/
#include <iostream>
#include <mutex>
#include <cstdlib>

using namespace std;


template <class T>
class optimisticList
{
public:
    optimisticList();
    ~optimisticList();

    bool contains(T);
    bool add(T);
    bool remove(T);
    void printList();
private:
    struct Node {
        T key;
        Node *next;
        mutex lock;
    };
    Node *head;
    Node *tail;
    bool validate(Node *, Node *);
};

/*************************************************************************
 * Initialize class variables
 * Head and tail will be used as a sentinel nodes
 * **********************************************************************/
template <class T>
optimisticList<T> :: optimisticList()
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
optimisticList<T> :: ~optimisticList()
{
    Node *temp;

    while(head->next != tail) {
        temp = head->next;
        head->next = temp->next;
        delete temp;
    }

    delete head;
    delete tail;
}

/*************************************************************************
 * Uses Optimistic Synchronization to check if the given parameter is in
 * the linked list. If found, return true, else return false.
 * **********************************************************************/
template <class T>
bool optimisticList<T> :: contains(T key)
{
    while(true) {
        Node *pred = head;
        Node *curr = head->next;

        // While not at the of the linked list
        while(curr != tail) {
            // If current key is >= key, break out of traversal
            if(curr->key >= key)
                break;

            // Set pred to curr node
            pred = curr;

            // Set curr to next node
            curr = curr->next;
        }

        // Acquire pred and curr locks
        pred->lock.lock();
        curr->lock.lock();

        // Validate we locked correct nodes
        if(validate(pred, curr)) {
            // If valid, release pred and curr locks
            pred->lock.unlock();
            curr->lock.unlock();

            // Return true if key was found
            return (curr->key == key);
        }
        // Validation failed, release locks and retry
        pred->lock.unlock();
        curr->lock.unlock();
    }
}

/*************************************************************************
 * Uses Optimistic Synchronization to attempt to add the given parameter.
 * If the parameter is already in the linked list, do not add again and
 * return false. If parameter is not already in the linked list, add node
 * and return true.
 * **********************************************************************/
template <class T>
bool optimisticList<T> :: add(T key)
{
    while(true) {
        Node *pred = head;
        Node *curr = head->next;

        // While not at the of the linked list
        while(curr != tail) {
            // If current key is >= key, break out of traversal
            if(curr->key >= key)
                break;

            // Set pred to curr node
            pred = curr;

            // Set curr to next node
            curr = curr->next;
        }

        // Acquire pred and curr locks
        pred->lock.lock();
        curr->lock.lock();

        // Validate we locked correct nodes
        if(validate(pred,curr)) {
            // If valid & key is found in list, release locks and return false
            if(curr != tail && curr->key == key) {
                pred->lock.unlock();
                curr->lock.unlock();
                return false;
            }
            // Else, add key to list, release locks and return true
            else {
                Node *node = new Node;
                node->key = key;
                node->next = curr;
                pred->next = node;

                pred->lock.unlock();
                curr->lock.unlock();

                return true;
            }
        }
        // Validation failed, release locks and retry
        pred->lock.unlock();
        curr->lock.unlock();
    }
}

/*************************************************************************
 * Uses Optimistic Synchronization to attempt to remove the give parameter.
 * If the parameter is not found in the linked list, return false. If the
 * parameter is found in the linked list, remove it and return true.
 * **********************************************************************/
template <class T>
bool optimisticList<T> :: remove(T key)
{
    while(true) {
        Node *pred = head;
        Node *curr = head->next;

        // While not at the of the linked list
        while(curr != tail) {
            // If current key is >= key, break out of traversal
            if(curr->key >= key)
                break;

            // Set pred to curr node
            pred = curr;

            // Set curr to next node
            curr = curr->next;
        }

        // Acquire pred and curr locks
        pred->lock.lock();
        curr->lock.lock();

        // Validate we locked correct nodes
        if(validate(pred, curr)) {
            // If valid & key is not found in list, release locks and return false
            if(curr != tail && curr->key != key) {
                pred->lock.unlock();
                curr->lock.unlock();
                return false;
            }
            // Else, remove key from list, release locks and return true
            else {
                //Node *temp = curr;
                pred->next = curr->next;
                //delete temp;

                pred->lock.unlock();
                curr->lock.unlock();
                return true;
            }
        }
        // Validation failed, release locks and retry
        pred->lock.unlock();
        curr->lock.unlock();
    }
}

/*************************************************************************
 * Display contents of linked list
 * **********************************************************************/
 template <class T>
 void optimisticList<T> :: printList()
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
 * Validation checks that pred points to curr and is reachable from head.
 * **********************************************************************/
template <class T>
bool optimisticList<T> :: validate(Node *pred, Node *curr)
{
    // Set node to head
    Node *node = head;

    // While not at the of the linked list
    while(node != tail) {
        // If node key > pred key, incorrect node, break and return false
        if(node->key > pred->key)
            break;
        // If pred is reachable from head
        if(node == pred)
            // Return true if pred points to curr, else false
            return (pred->next == curr);

        // Update node to next node in list
        node = node->next;
    }
    return false;
}
