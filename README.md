# Red-Black Tree Implementation

Implementation of Red-Black Tree and Binary Search Tree in C++ with performance comparison.

## Features

- Binary Search Tree (BST), implemented with parental connection, for easier copying of functionality into the RBT class
- Red-Black Tree (RBT) with full balancing
- Colored traversal of RBT
- Performance measurement
- Tree validation

## Building
```bash
g++ -c main.cpp
g++ main.o -o compiled.exe
```

## Usage
```bash
./compiled
```

### Commands:
- `i` - insert
- `d` - delete
- `s` - search
- `t` - traverse (colored for RBT)
- `b` - black height (RBT only)
- `q` - check RBTs correctness
- `e` - exit (calls destructors)