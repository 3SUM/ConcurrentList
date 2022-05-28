/*************************************************************************
 * Luis Maya Aranda
 *
 * Fine-Grained Synchronization Linked List
 * We can improve concurrency by locking individual nodes, rather than
 * locking the list as a whole. Instead of placing a lock on the entire list
 * let us add a lock to each node. As a thread traverses the list, it locks
 * each node when it first visits, and sometime later releases it. Such
 * fine-grained locking permits concurrent threads to traverse the list
 * together in a pipelined fashion.
 *
 * **********************************************************************/
#pragma once

#include <atomic>
#include <iostream>
#include <mutex>

template <class T>
class FineGrainedList {
   public:
    FineGrainedList();
    ~FineGrainedList();
    T front() const;
    T back() const;
    bool empty() const;
    int size() const;
    void push_back(const T &);
    void pop_back();
    void print_forwards();
    void print_backwards();

   private:
    struct Node {
        T key;
        Node *prev;
        Node *next;
        mutable std::mutex lock;

        Node() : key(T()), prev(nullptr), next(nullptr) {}
        Node(T key) : key(key), prev(nullptr), next(nullptr) {}
        Node(T key, Node *prev, Node *next) : key(key), prev(prev), next(next) {}
    };
    atomic<int> list_size;
    Node *head;
    Node *tail;
    void delete_list();
};

template <class T>
FineGrainedList<T>::FineGrainedList() {
    head = new Node();
    tail = new Node();
    head->next = tail;
    tail->prev = head;
}

template <class T>
FineGrainedList<T>::~FineGrainedList() {
    deleteList();
}

template <class T>
T FineGrainedList<T>::front() const {
    if (head->next) {
        std::lock_guard<std::mutex> guard(head->next);
        return head->next->key;
    }
    return T();
}

template <class T>
T FineGrainedList<T>::back() const {
    if (tail) {
        std::lock_guard<std::mutex> guard(tail);
        return tail->key;
    }
    return T();
}

template <class T>
bool FineGrainedList<T>::empty() const {
    return list_size == 0;
}

template <class T>
int FineGrainedList<T>::size() const {
    return list_size;
}


template <class T>
void FineGrainedList<T>::push_back(const T &key) {
    Node *node = new Node(key);
    std::scoped_lock<std::mutex> lock(head, tail);

    if (head->next == nullptr) {
        head->next = node;
        node->prev = head;
        tail->prev = node;
        node->next = tail;
    } else {
        tail->prev->next = node;
        node->prev = tail->prev;
        tail->prev = node;
        node->next = tail;
    }
    list_size++;
}

template <class T>
void FineGrainedList<T>::pop_back() {
    std::scoped_lock<std::mutex> lock(head, tail);

    if (head->next && tail->prev) {
        if (head->next == tail->prev) {
            delete head->next;
            head->next = nullptr;
            tail- = nullptr;
        } else {
            Node *itr = tail;
            tail = tail->prev;
            tail->next = nullptr;
            delete itr;
        }
        list_size--;
    }
}

template <class T>
void FineGrainedList<T>::print_forwards() {
    Node *itr = head;
    while (itr) {
        std::cout << itr->key << " ";
        itr = itr->next;
    }
    std::cout << std::endl;
}

template <class T>
void FineGrainedList<T>::print_backwards() {
    Node *itr = tail;
    while (itr) {
        std::cout << itr->key << " ";
        itr = itr->prev;
    }
    std::cout << std::endl;
}

template <class T>
void FineGrainedList<T>::delete_list() {
    Node *itr = head;
    while (itr) {
        head = head->next;
        head->prev = nullptr;
        delete itr;
        list_size--;
        itr = head;
    }
    head = nullptr;
    tail = nullptr;
}
