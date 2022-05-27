/*************************************************************************
 * Luis Maya Aranda
 * Coarse-Grained Synchronization List
 *
 * The list itself has a single lock which every method call must acquire.
 * The principle advantage of this algorithm, is that it is thread safe.
 * All methods act on the list only while holding the lock, so the execution
 * is essentially sequential.
 *
 * **********************************************************************/
#pragma once

#include <iostream>
#include <mutex>

template <class T>
class CoarseGrainedList {
   public:
    CoarseGrainedList();
    ~CoarseGrainedList();
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

        Node() : key(T()), prev(nullptr), next(nullptr) {}
        Node(T key) : key(key), prev(nullptr), next(nullptr) {}
        Node(T key, Node *prev, Node *next) : key(key), prev(prev), next(next) {}
    };
    int list_size;
    Node *head;
    Node *tail;
    mutable std::mutex lock;
    void delete_list();
};

template <class T>
CoarseGrainedList<T>::CoarseGrainedList() {
    list_size = 0;
    head = nullptr;
    tail = nullptr;
}

template <class T>
CoarseGrainedList<T>::~CoarseGrainedList() {
    delete_list();
}

template <class T>
T CoarseGrainedList<T>::front() const {
    std::lock_guard<std::mutex> guard(lock);
    return head != nullptr ? head->key : T();
}

template <class T>
T CoarseGrainedList<T>::back() const {
    std::lock_guard<std::mutex> guard(lock);
    return tail != nullptr ? tail->key : T();
}

template <class T>
bool CoarseGrainedList<T>::empty() const {
    std::lock_guard<std::mutex> guard(lock);
    return head == nullptr && tail == nullptr;
}

template <class T>
int CoarseGrainedList<T>::size() const {
    std::lock_guard<std::mutex> guard(lock);
    return list_size;
}

template <class T>
void CoarseGrainedList<T>::push_back(const T &key) {
    std::lock_guard<std::mutex> guard(lock);

    Node *node = new Node(key);
    if (head == nullptr) {
        head = node;
        tail = node;
    } else {
        tail->next = node;
        node->prev = tail;
        tail = node;
    }
    list_size++;
}

template <class T>
void CoarseGrainedList<T>::pop_back() {
    std::lock_guard<std::mutex> guard(lock);

    if (head && tail) {
        if (head == tail) {
            delete head;
            head = nullptr;
            tail = nullptr;
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
void CoarseGrainedList<T>::print_forwards() {
    std::lock_guard<std::mutex> guard(lock);

    Node *itr = head;
    while (itr) {
        std::cout << itr->key << " ";
        itr = itr->next;
    }
    std::cout << std::endl;
}

template <class T>
void CoarseGrainedList<T>::print_backwards() {
    std::lock_guard<std::mutex> guard(lock);

    Node *itr = tail;
    while (itr) {
        std::cout << itr->key << " ";
        itr = itr->prev;
    }
    std::cout << std::endl;
}

template <class T>
void CoarseGrainedList<T>::delete_list() {
    std::lock_guard<std::mutex> guard(lock);

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
