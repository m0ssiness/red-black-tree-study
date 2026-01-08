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
            if (isCurrentLesserChild) parent->assignLesserChild(n);
            else parent->assignGreaterChild(n);
            return;
        }
        value < current->getValue() ? __insert(current->getLesserChild(), current, value, true) : __insert(current->getGreaterChild(), current, value, false);
}

std::pair <Node*, Node*> BST::__search(Node* current, Node* parent, int value) {
    if (!current || current->getValue() == value) return {current, parent};
    return value < current->getValue() ? __search(current->getLesserChild(), current, value) : __search(current->getGreaterChild(), current, value);
}

std::pair <Node*, Node*> BST::__remove__findSuccessor(Node* successor, Node* parent) {
    if (!(successor->getLesserChild())) {
            return {successor, parent};
    }
    return __remove__findSuccessor(successor->getLesserChild(), successor);
}

void BST::__remove(std::pair <Node*, Node*> toDeletePair) {
    Node* toRemove = toDeletePair.first;
    Node* toRemoveParent = toDeletePair.second;
    if (toRemove->getGreaterChild() && toRemove->getLesserChild()) {
        std::pair <Node*, Node*> successorPair = __remove__findSuccessor(toRemove->getGreaterChild(), toRemove);
        toRemove->assignValue((successorPair.first)->getValue());
        toRemove = successorPair.first;
        toRemoveParent = successorPair.second;
    }
    if (!toRemoveParent) { //requires root change
        root = toRemove->getLesserChild() ? toRemove->getLesserChild() : toRemove->getGreaterChild();
    } else {
        if (toRemoveParent->getLesserChild() == toRemove) toRemoveParent->assignLesserChild(toRemove->getLesserChild() ? toRemove->getLesserChild() : toRemove->getGreaterChild());
        else toRemoveParent->assignGreaterChild(toRemove->getLesserChild() ? toRemove->getLesserChild() : toRemove->getGreaterChild());
    }
    delete toRemove;
}

Node* BST::getRoot() { // returns (Node*) root of the tree
    return root;
}

Node* BST::search(int value) { // in first call - value | returns (Node*) if found, (nullptr) if not found
    return (__search(this->getRoot(), nullptr, value)).first;
}

void BST::insert(int value) { // in first call - value | inserts a unique! element, for non-unique throws an invalid_argument exception
    if(!root) {
        root = new Node(value);
        return;
    }
    if (!(__search(this->getRoot(), nullptr, value).first)) __insert(this->getRoot(), nullptr, value, false);
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
    std::pair <Node*, Node*> toDeletePair = __search(this->getRoot(), nullptr, value);
    if (toDeletePair.first) {
        __remove(toDeletePair);
    }
    else throw std::invalid_argument("not an existent node");
}

BST::~BST(){
    Node* n = root;
    while (root) {
        __remove({n, nullptr});
        n = root;
    }
}