#include <iostream>
#include <stdexcept>
#include "bst.h"

BST::BST() : root(nullptr) {}

BST::BST(int v) {
    root = new Node (v);
}

void BST::__insert(Node* current, Node* parent, int value, bool isCurrentLesserChild) {
    if (!current) {
            Node* n = new Node(value);
            n->assignParent(parent);
            if (isCurrentLesserChild) parent->assignLesserChild(n);
            else parent->assignGreaterChild(n);
            return;
        }
        value < current->getValue() ? __insert(current->getLesserChild(), current, value, true) : __insert(current->getGreaterChild(), current, value, false);
}

Node* BST::__search(Node* current, int value) {
    if (!current || current->getValue() == value) return current;
    return value < current->getValue() ? __search(current->getLesserChild(), value) : __search(current->getGreaterChild(), value);
}

Node* BST::__remove__findSuccessor(Node* successor, bool & isLeftChild) {
    if (!(successor->getLesserChild())) {
            return successor;
    }
    isLeftChild = true;
    return __remove__findSuccessor(successor->getLesserChild(), isLeftChild);
}

void BST::__remove(Node* toRemove, char whichChild) {
    // whichChild 'n' => toRemove is a root 
    if (toRemove->getLesserChild() && toRemove->getGreaterChild()) { //case two children
        bool isSuccessorLesserChild = false;
        Node* successor = __remove__findSuccessor(toRemove->getGreaterChild(), isSuccessorLesserChild);
        Node* successorParent = successor->getParent();
        if (isSuccessorLesserChild) successor->getParent()->assignLesserChild(successor->getGreaterChild());
        else successor->getParent()->assignGreaterChild(successor->getGreaterChild());
        if (successor->getGreaterChild()) successor->getGreaterChild()->assignParent(successorParent);
        toRemove->assignValue(successor->getValue());
        //successor is the one to delete now
        delete successor;
    } else { // too heavy, to optimize! ---- 
        Node* toRemoveParent = toRemove->getParent(); 
        if (toRemove->getLesserChild()) { // case one child
            if (whichChild == 'n') {
                root = toRemove->getLesserChild();
                toRemove->getLesserChild()->assignParent(nullptr);
                toRemove->assignLesserChild(nullptr);
            } else if (whichChild == 'l') { 
                toRemove->getParent()->assignLesserChild(toRemove->getLesserChild());
                toRemove->getLesserChild()->assignParent(toRemoveParent);
            } else {
                    toRemove->getParent()->assignGreaterChild(toRemove->getLesserChild());
                    toRemove->getLesserChild()->assignParent(toRemoveParent);
            }
        } else if (toRemove->getGreaterChild()) {
            if (whichChild == 'n') {
                root = toRemove->getGreaterChild();
                toRemove->getGreaterChild()->assignParent(nullptr);
                toRemove->assignGreaterChild(nullptr);
            } else if (whichChild == 'l') { 
                    toRemove->getParent()->assignLesserChild(toRemove->getGreaterChild());
                    toRemove->getGreaterChild()->assignParent(toRemoveParent);
            } else {
                    toRemove->getParent()->assignGreaterChild(toRemove->getGreaterChild());
                    toRemove->getGreaterChild()->assignParent(toRemoveParent);
            }
        } else { // case no children
            if (whichChild == 'n') {
                    root = nullptr;
            } else if (whichChild == 'l') { 
                    toRemove->getParent()->assignLesserChild(nullptr);
            } else {
                    toRemove->getParent()->assignGreaterChild(nullptr);
            }
        }
        delete toRemove;
    }
}

Node* BST::getRoot() { // returns (Node*) root of the tree
    return root;
}

Node* BST::search(int value) { // in first call - value | returns (Node*) if found, (nullptr) if not found
    return __search(this->getRoot(), value);
}

void BST::insert(int value) { // in first call - value | inserts a unique! element, for non-unique throws an invalid_argument exception
    if(!root) {
        root = new Node(value);
        return;
    }
    if (!search(value)) __insert(this->getRoot(), this->getRoot()->getParent(), value, false);
    else throw std::invalid_argument("not a unique value");
}

void BST::traverse(Node* current) { // in first call - node from which it should be traversed (you might want to use tree->getRoot()) | traverses a tree in order
    if (!root) std::cout << "Tree empty. ";
    if (!current) return;
    traverse(current->getLesserChild());
    std::cout << current->getValue() << " ";
    traverse(current->getGreaterChild());
}

void BST::remove(int value) { // in first call - value | removes an existent node, for non-existent throws an invalid_argument exception
    Node* n = search(value);
    if (n) {
        char whichChild = ' ';
        if (n->getParent()) {
            whichChild = (n == n->getParent()->getLesserChild()) ?  'l' : 'r';
        } else {
            whichChild = 'n';
        }
        __remove(n, whichChild);
    }
    else throw std::invalid_argument("not an existent node");
}

BST::~BST(){
    Node* n = root;
    while (root) {
        __remove(n, 'n');
        n = root;
    }
}