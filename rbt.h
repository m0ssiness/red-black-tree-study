#ifndef RBT_H
#define RBT_H

#include "node_rbt.h"

class RBT {
    NodeRBT* root;

    NodeRBT* __search(NodeRBT*, int);
    void __leftRotation(NodeRBT*);
    void __rightRotation(NodeRBT*);
    void __rebalance(NodeRBT*);
    void __insert(NodeRBT*, NodeRBT*, int, bool);
    void __rebalance_removal(NodeRBT*, bool);
    NodeRBT* __remove__findSuccessor(NodeRBT*, bool &);
    void __remove(NodeRBT*, char);

    public:

    RBT();
    RBT(int);

    NodeRBT* getRoot();
    NodeRBT* search(int);
    void traverse(NodeRBT*, int = 0);
    void insert(int);
    void remove(int);
    int getBlackHeight();
    bool isRBT(NodeRBT*, NodeRBT* = nullptr, int = 1, bool = false);

    ~RBT();
};

#endif