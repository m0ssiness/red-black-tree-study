#ifndef BST_H
#define BST_H

#include "node_bst.h"

class BST {
        Node* root;

        void __insert(Node*, Node*, int, bool);
        Node* __search(Node*, int);
        Node* __remove__findSuccessor(Node*, bool &);
        void __remove(Node*, char);

    public:
        BST ();
        BST (int);

        Node* getRoot();
        Node* search(int);
        void insert(int);
        void traverse(Node*);
        void remove(int);

        ~BST();
};

#endif