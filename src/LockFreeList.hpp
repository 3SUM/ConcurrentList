/*************************************************************************
 * Luis Maya Aranda
 *
 * Lock-free Linked List
 *
 * **********************************************************************/
#pragma once

#include <climits>
#include <iostream>

#include "AtomicMarkableReference.hpp"

template <class T>
class LockFreeList {
   public:
    LockFreeList();
    ~LockFreeList();
    bool contains(T);
    bool add(T);
    bool remove(T);
    void printList();
    void deleteList();

   private:
    struct Node {
        T key;
        AtomicMarkableReference<Node> *next;
        Node() {
            key = {};
            next = new AtomicMarkableReference<Node>;
        }
        Node(T myKey) {
            key = myKey;
            next = new AtomicMarkableReference<Node>;
        }
    };

    struct Window {
        Node *pred;
        Node *curr;
        Window(Node *myPred, Node *myCurr) {
            pred = myPred;
            curr = myCurr;
        }

        /*
         * Creates a structure containing the nodes on either side of
         * the key. It removes marked nodes when it encounters them.
         */
        Window(Node *head, int key) {
            pred = NULL;
            curr = NULL;
            Node *succ = NULL;

            bool *marked = new bool;
            bool snip;

        RETRY:
            while (true) {
                pred = head;
                curr = pred->next->getReference();
                while (true) {
                    succ = curr->next->get(marked);
                    while (marked[0]) {
                        snip = pred->next->CAS(curr, succ, false, false);
                        if (!snip)
                            goto RETRY;
                        curr = succ;
                        succ = curr->next->get(marked);
                    }
                    if (curr->key >= key) {
                        Window(pred, curr);
                        return;
                    }
                    pred = curr;
                    curr = succ;
                }
            }
        }
    };
    Node *head;
    Node *tail;
};

/*
 * Initialize class variables
 * Head and tail will be used as a sentinel nodes
 */
template <class T>
LockFreeList<T>::LockFreeList() {
    head = new Node(INT_MIN);

    tail = new Node(INT_MAX);

    /* Set next of head to tail
     * and next of tail will be NULL, false due to default constructors */
    head->next->set(tail, false);
}

/*
 * Deallocate linked list memory
 */
template <class T>
LockFreeList<T>::~LockFreeList() {
    deleteList();

    delete head;
    delete tail;
}

/*
 * This wait-free contains method is almost the same as the Lazy
 * Synchronization method. It checks if the given parameter is in the linked
 * list. If found, return true, else return false. The only small differance
 * is it calls curr->next->get(marked) to test whether curr is marked.
 */
template <class T>
bool LockFreeList<T>::contains(T key) {
    bool *marked = new bool;

    Node *curr = head;
    while (curr->key < key) {
        curr = curr->next->getReference();
        curr->next->get(marked);
    }

    return (curr->key == key && !marked[0]);
}

/*
 * The add method creates a window to locate pred and curr. It adds a new
 * node only if pred is unmarked and refers to curr.
 */
template <class T>
bool LockFreeList<T>::add(T key) {
    while (true) {
        Window window(head, key);
        Node *pred = window.pred;
        Node *curr = window.curr;
        if (curr->key == key) {
            return false;
        } else {
            Node *node = new Node(key);
            node->next->set(curr, false);
            if (pred->next->CAS(curr, node, false, false)) {
                return true;
            }
        }
    }
}

/*
 * The remove method creates a window to locate pred and curr, and atomically
 * marks the node for removal.
 */
template <class T>
bool LockFreeList<T>::remove(T key) {
    bool snip = false;

    while (true) {
        Window window(head, key);
        Node *pred = window.pred;
        Node *curr = window.curr;
        if (curr->key != key) {
            return false;
        } else {
            Node *succ = curr->next->getReference();

            snip = curr->next->attemptMark(succ, true);
            if (!snip)
                continue;
            pred->next->CAS(curr, succ, false, false);
            return true;
        }
    }
}

/*
 * Display contents of linked list
 */
template <class T>
void LockFreeList<T>::printList() {
    // Set curr to head->next since head is a sentinel node
    Node *curr = head;

    // Traverse linked list and display contents
    while (curr != NULL) {
        cout << curr->key << " ";
        curr = curr->next->getReference();
    }
}

/*
 * Delete contents of linked list
 */
template <class T>
void LockFreeList<T>::deleteList() {
    Node *temp;

    while (head->next->getReference() != tail) {
        temp = head->next->getReference();
        head->next = temp->next;
        delete temp;
    }

    delete head;
    delete tail;

    head = new Node(INT_MIN);

    tail = new Node(INT_MAX);

    /* Set next of head to tail
     * and next of tail will be NULL, false due to default constructors */
    head->next->set(tail, false);
}
