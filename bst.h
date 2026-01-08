#ifndef BST_H
#define BST_H

#include "node_bst.h"

class BST {
        Node* root;

        void __insert(Node*, Node*, int, bool);
        std::pair <Node*, Node*> __search(Node*, Node*, int);
        std::pair <Node*, Node*> __remove__findSuccessor(Node*, Node*);
        void __remove(std::pair <Node*, Node*>);

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