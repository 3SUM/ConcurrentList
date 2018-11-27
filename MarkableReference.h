#include <iostream>
#include <cstdlib>

using namespace std;

template <class T>
class MarkableReference
{
public:
    MarkableReference(T*, bool);
    T* getRef();
    bool getMark();
private:
    uintptr_t val;
    static const uintptr_t mask = 1;
};

template <class T>
MarkableReference<T> :: MarkableReference(T* ref = NULL, bool mark = false)
{
    val = ((uintptr_t)ref & ~mask) | (mark ? 1 : 0);
}

template <class T>
T* MarkableReference<T> :: getRef()
{
    return (T*)(val & ~mask);
}

template <class T>
bool MarkableReference<T> :: getMark()
{
    return (val & mask);
}
