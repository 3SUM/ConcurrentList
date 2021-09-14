/*************************************************************************
 * Luis Maya
 * Lazy Synchronization Linked List
 * The next step is to refine the Optimistic Synchronization algorithm so
 * that contains() calls are wait-free, and add() and remove() methods,
 * while still blocking, traverse the list only once(in the absence of
 * contention). We add to each node a Boolean marked field indicating
 * whether that node is in the set. Now, traversals do not need to lock
 * the target node, and there is no need to validate that the node is reachable
 * by retraversing the whole list. Instead, the algorithm maintains the
 * invariant that every unmarked node is reachable. If a traversing
 * thread does not find a node, or finds it marked, then the item is not
 * in the set.
 * **********************************************************************/
#include <iostream>
#include <mutex>
#include <cstdlib>

using namespace std;


template <class T>
class LazyLL
{
public:
    LazyLL();
    ~LazyLL();
    bool contains(T);
    bool add(T);
    bool remove(T);
    void printList();
    void deleteList();
private:
    struct Node {
        T key;
        bool marked;
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
LazyLL<T> :: LazyLL()
{
    head = new Node;
    head->key = {};
    head->marked = false;

    tail = new Node;
    tail->key = {};
    tail->marked = false;
    tail->next = NULL;

    head->next = tail;
}

/*************************************************************************
 * Deallocate linked list memory
 * **********************************************************************/
template <class T>
LazyLL<T> :: ~LazyLL()
{
    deleteList();

    delete head;
    delete tail;
}

/*************************************************************************
 * Uses Lazy Synchronization to check if the given parameter is in
 * the linked list. If found, return true, else return false.
 * **********************************************************************/
template <class T>
bool LazyLL<T> :: contains(T key)
{
    // Set curr to head node
    Node *curr = head;

    // While not at the of the linked list
    while(curr != tail) {
        // If current key is >= key, break out of traversal
        if(curr->key >= key)
            break;

        // Set curr to next node
        curr = curr->next;
    }

    // If key is found and curr is not marked, return true
    return (curr->key == key && !curr->marked);
}

/*************************************************************************
 * Uses Lazy Synchronization to attempt to add the given parameter.
 * If the parameter is already in the linked list, do not add again and
 * return false. If parameter is not already in the linked list, add node
 * and return true.
 * **********************************************************************/
template <class T>
bool LazyLL<T> :: add(T key)
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
                node->marked = false;
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
 * Uses Lazy Synchronization to attempt to remove the give parameter.
 * If the parameter is not found in the linked list, return false. If the
 * parameter is found in the linked list, remove it and return true.
 * **********************************************************************/
template <class T>
bool LazyLL<T> :: remove(T key)
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

                // Logical removal
                curr->marked = true;

                // Physical removal
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
 void LazyLL<T> :: printList()
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
 * Validation checks that neither the pred nor curr nodes have been logically
 * deleted, and that pred points to curr.
 * **********************************************************************/
template <class T>
bool LazyLL<T> :: validate(Node *pred, Node *curr)
{
    return  (!pred->marked && !curr->marked && pred->next == curr);
}

/*************************************************************************
 * Delete contents of linked list
 * **********************************************************************/
template <class T>
void LazyLL<T> :: deleteList()
{
    Node *temp;

    while(head->next != tail) {
        temp = head->next;
        head->next = temp->next;
        delete temp;
    }

    head = new Node;
    head->key = {};
    head->marked = false;

    tail = new Node;
    tail->key = {};
    tail->marked = false;
    tail->next = NULL;

    head->next = tail;
}
