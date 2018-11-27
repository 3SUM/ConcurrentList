# Concurrent Linked List

Concurrent Linked List Implementations in C++

## Contents

#### Synchronization Methods

- [Coarse-Grained Synchronization](src/coarseList.h)
- [Fine-Grained Synchronization](src/fineList.h)
- [Optimistic Synchronization](src/optimisticList.h)
- [Lazy Synchronization](src/lazyList.h)
- Lock-Free (TODO)

## How to Test

#### Testing of all synchronization methods is provided by [linktest](src/linktest.cpp).
```
make
./linktest <threadCount>
```

## Contact

email: mayafluis@gmail.com
