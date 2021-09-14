/*
 * @author	Luis Maya Aranda
 * @date	02/16/19
 * @AtomicMarkableReference implementation in C++
 */

#include <atomic>
#include <iostream>

using namespace std;

/*
 * Struct for atomic markable reference
 */
template <class T>
struct MarkableReference {
    T* next;
    bool marked;

    MarkableReference() {
        next = NULL;
        marked = false;
    }

    MarkableReference(T* node, bool mark) {
        next = node;
        marked = mark;
    }

    bool operator==(const MarkableReference<T>& other) {
        return (next == other.next && marked == other.marked);
    }
};

/*
 * The AtomicMarkableReference class
 */
template <class T>
class AtomicMarkableReference {
   private:
    /*
		 * Since a pointer is always a POD type, even if T is of non-pod type
		 * atomicity will be maintained
		 */
    std::atomic<MarkableReference<T>*> markedNext;

   public:
    AtomicMarkableReference() {
        markedNext.store(new MarkableReference<T>(NULL, false));
    }

    AtomicMarkableReference(T* nextNode, bool mark) {
        /* Atomically store the values*/
        markedNext.store(new MarkableReference<T>(nextNode, mark));
    }

    /*
		 * Returns the reference. load() is atomic and hence that will be the linearization point
		 */
    T* getReference() {
        return markedNext.load()->next;
    }

    /*
		 * Returns the reference and update bool in the reference passed as the argument
		 * load() is atomic and hence that will be the linearization point.
		 */
    T* get(bool* mark) {
        MarkableReference<T>* temp = markedNext.load();
        *mark = temp->marked;
        return temp->next;
    }

    /*
		 * Set the variables unconditionally. load() is atomic and hence that will be the linearization point
		 */
    void set(T* newRef, bool newMark) {
        MarkableReference<T>* curr = markedNext.load();

        if (newRef != curr->next || newMark != curr->marked) {
            markedNext.store(new MarkableReference<T>(newRef, newMark));
        }
    }

    /*
		 * Tests an expected reference value and if the test succeeds,
		 * replaces it with a new mark value
		 */
    bool attemptMark(T* expected, bool newMark) {
        MarkableReference<T>* curr = markedNext.load();

        if (expected == curr->next) {
            markedNext.store(new MarkableReference<T>(expected, newMark));
            return true;
        }
        return false;
    }

    /*
		 * CAS with reference and the marked field. load() is atomic and hence that will be the linearization point.
		 * I take advantage of the fact that C++ has short-circuiting hence
		 * if one of the first 2 conditions is false the atomic_compare_exchange_strong will not happen
		 */

    bool CAS(T* expected, T* newValue, bool expectedBool, bool newBool) {
        MarkableReference<T>* curr = markedNext.load();
        return (expected == curr->next && expectedBool == curr->marked &&
                ((newValue == curr->next && newBool == curr->marked) ||
                 markedNext.compare_exchange_strong(curr, new MarkableReference<T>(newValue, newBool))));
    }
};
