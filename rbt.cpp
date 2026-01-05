#include <iostream>
#include <stdexcept>
#include "rbt.h"

#define COLOR_RED     "\033[1;31m"
#define COLOR_BLACK   "\033[1;36m"
#define COLOR_RESET   "\033[0m"
#define BOLD   "\033[1;37m"

RBT::RBT() : root(nullptr) {}

RBT::RBT(int val) {
    root = new NodeRBT(val);
    root->assignRed(false);
}


NodeRBT* RBT::__search(NodeRBT* current, int value) { 
    if (!current || current->getValue() == value) return current;
    return value < current->getValue() ? __search(current->getLesserChild(), value) : __search(current->getGreaterChild(), value);
}

void RBT::__leftRotation(NodeRBT* pivot) {
    NodeRBT* newPivot = pivot->getGreaterChild(); // always not nullptr
    NodeRBT* pivotParent = pivot->getParent(); // may be nullptr
    pivot->assignGreaterChild(newPivot->getLesserChild());
    if (newPivot->getLesserChild()) newPivot->getLesserChild()->assignParent(pivot);
    pivot->assignParent(newPivot);
    newPivot->assignParent(pivotParent);
    if (pivotParent && pivot == pivotParent->getLesserChild()) pivotParent->assignLesserChild(newPivot);
    else if (pivotParent && pivot == pivotParent->getGreaterChild()) pivotParent->assignGreaterChild(newPivot);
    else root = newPivot;
    newPivot->assignLesserChild(pivot);
}

void RBT::__rightRotation(NodeRBT* pivot) {
    NodeRBT* newPivot = pivot->getLesserChild(); // always not nullptr
    NodeRBT* pivotParent = pivot->getParent(); // may be nullptr
    pivot->assignLesserChild(newPivot->getGreaterChild());
    if (newPivot->getGreaterChild()) newPivot->getGreaterChild()->assignParent(pivot);
    pivot->assignParent(newPivot);
    newPivot->assignParent(pivotParent);
    if (pivotParent && pivot == pivotParent->getLesserChild()) pivotParent->assignLesserChild(newPivot);
    else if (pivotParent && pivot == pivotParent->getGreaterChild()) pivotParent->assignGreaterChild(newPivot);
    else root = newPivot;
    newPivot->assignGreaterChild(pivot);
}

void RBT::__rebalance(NodeRBT* current) {
    root->assignRed(false);
    if (current == root) return;
    if (!(current->getParent()->isRed())) return;
    bool isParentLesserChild = (current->getParent()->getParent()->getLesserChild() == current->getParent());
    NodeRBT* uncle = isParentLesserChild ? current->getParent()->getParent()->getGreaterChild() : current->getParent()->getParent()->getLesserChild();
    if (uncle && uncle->isRed()) {
        current->getParent()->assignRed(false);
        uncle->assignRed(false);
        current->getParent()->getParent()->assignRed(true);
        __rebalance(current->getParent()->getParent());
    } else {
        NodeRBT* currentGrandparent = current->getParent()->getParent();
        if (isParentLesserChild) {
            if (current->getParent()->getGreaterChild() == current) __leftRotation(current->getParent());
            __rightRotation(currentGrandparent);
            currentGrandparent->assignRed(true);
            currentGrandparent->getParent()->assignRed(false);
        }
        else {
            if (current->getParent()->getLesserChild() == current) __rightRotation(current->getParent());
            __leftRotation(currentGrandparent);
            currentGrandparent->assignRed(true);
            currentGrandparent->getParent()->assignRed(false);
        }
    }
}

void RBT::__insert(NodeRBT* current, NodeRBT* parent, int value, bool isCurrentLesserChild) {
    if (!current) {
        NodeRBT* n = new NodeRBT(value);
        n->assignParent(parent);
        if (isCurrentLesserChild) parent->assignLesserChild(n);
        else parent->assignGreaterChild(n);
        __rebalance(n); // calling rebalance after each insertion
        return;
    }
    value < current->getValue() ? __insert(current->getLesserChild(), current, value, true) : __insert(current->getGreaterChild(), current, value, false);
}

void RBT::__rebalance_removal(NodeRBT* doubleBlackParent, bool isDoubleBlackLesserChild) {
    NodeRBT* sibling = (isDoubleBlackLesserChild ? doubleBlackParent->getGreaterChild() : doubleBlackParent->getLesserChild());
    if (!sibling) return;
    NodeRBT* siblingNearestChild = (isDoubleBlackLesserChild ? sibling->getLesserChild() : sibling->getGreaterChild());
    NodeRBT* siblingFurthestChild = (isDoubleBlackLesserChild ? sibling->getGreaterChild() : sibling->getLesserChild());
    if (!(sibling->isRed())) {
        if (siblingNearestChild && siblingNearestChild->isRed()) {
            if (isDoubleBlackLesserChild) __rightRotation(sibling);
            else __leftRotation(sibling);
            sibling->assignRed(true);
            siblingNearestChild->assignRed(false);
            siblingFurthestChild = sibling;
            sibling = siblingNearestChild;
        }
        if (siblingFurthestChild && siblingFurthestChild->isRed()) {
            if (isDoubleBlackLesserChild) __leftRotation(doubleBlackParent);
            else __rightRotation(doubleBlackParent);
            sibling->assignRed(doubleBlackParent->isRed());
            doubleBlackParent->assignRed(false);
            siblingFurthestChild->assignRed(false);
        } else {
            sibling->assignRed(true);
            if (doubleBlackParent->isRed()) doubleBlackParent->assignRed(false);
            else 
                if (doubleBlackParent->getParent()) __rebalance_removal(doubleBlackParent->getParent(), (doubleBlackParent->getParent()->getLesserChild() == doubleBlackParent));
        }
    } else {
        sibling->assignRed(false);
        doubleBlackParent->assignRed(true);
        if (isDoubleBlackLesserChild) {
            __leftRotation(doubleBlackParent);
        }
        else {
            __rightRotation(doubleBlackParent);
        }
        __rebalance_removal(doubleBlackParent, isDoubleBlackLesserChild);
    }
}

NodeRBT* RBT::__remove__findSuccessor(NodeRBT* successor, bool & isLeftChild) {
    if (!(successor->getLesserChild())) {
        return successor;
    }
    isLeftChild = true;
    return __remove__findSuccessor(successor->getLesserChild(), isLeftChild);
}

void RBT::__remove(NodeRBT* toRemove, char whichChild) {
    // whichChild 'n' => toRemove is a root 
    if (toRemove->getLesserChild() && toRemove->getGreaterChild()) { //case two children
        bool isSuccessorLesserChild = false;
        NodeRBT* successor = __remove__findSuccessor(toRemove->getGreaterChild(), isSuccessorLesserChild);
        NodeRBT* successorParent = successor->getParent();
        if (isSuccessorLesserChild) successor->getParent()->assignLesserChild(successor->getGreaterChild());
        else successor->getParent()->assignGreaterChild(successor->getGreaterChild());
        toRemove->assignValue(successor->getValue());
        //successor is the one to delete now
        if (successor->getGreaterChild()) {
            successor->getGreaterChild()->assignRed(false); // if here - GChild is red in any case, turn black to maintain balance
            successor->getGreaterChild()->assignParent(successorParent);
            delete successor;
        }
        else {
            bool flag = successor->isRed();
            delete successor;
            if (!flag) __rebalance_removal(successorParent, isSuccessorLesserChild);
        }
    } else { // too heavy, to optimize! ---- 
        NodeRBT* toRemoveParent = toRemove->getParent(); 
        if (toRemove->getLesserChild()) { // case one child
            if (whichChild == 'n') {
                root = toRemove->getLesserChild();
                toRemove->getLesserChild()->assignParent(nullptr);
                toRemove->assignLesserChild(nullptr);
            } else if (whichChild == 'l') { 
                    toRemove->getParent()->assignLesserChild(toRemove->getLesserChild());
                    toRemove->getLesserChild()->assignRed(false); // if here - LChild is red in any case, turn black to maintain balance
                    toRemove->getLesserChild()->assignParent(toRemoveParent);
            } else {
                    toRemove->getParent()->assignGreaterChild(toRemove->getLesserChild());
                    toRemove->getLesserChild()->assignRed(false); // if here - LChild is red in any case, turn black to maintain balance
                    toRemove->getLesserChild()->assignParent(toRemoveParent);
            }
            delete toRemove;
        } else if (toRemove->getGreaterChild()) {
            if (whichChild == 'n') {
                root = toRemove->getGreaterChild();
                toRemove->getGreaterChild()->assignParent(nullptr);
                toRemove->assignGreaterChild(nullptr);
            } else if (whichChild == 'l') { 
                    toRemove->getParent()->assignLesserChild(toRemove->getGreaterChild());
                    toRemove->getGreaterChild()->assignRed(toRemove->isRed());
                    toRemove->getGreaterChild()->assignParent(toRemoveParent);
            } else {
                    toRemove->getParent()->assignGreaterChild(toRemove->getGreaterChild());
                    toRemove->getGreaterChild()->assignRed(toRemove->isRed()); 
                    toRemove->getGreaterChild()->assignParent(toRemoveParent);
            }
            delete toRemove;
        } else { // case no children
            if (whichChild == 'n') {
                    root = nullptr;
            } else if (whichChild == 'l') { 
                    toRemove->getParent()->assignLesserChild(nullptr);
                    bool flag = toRemove->isRed();
                    delete toRemove;
                    if (!flag) __rebalance_removal(toRemoveParent, true);
            } else {
                    toRemove->getParent()->assignGreaterChild(nullptr);
                    bool flag = toRemove->isRed();
                    delete toRemove;
                    if (!flag) __rebalance_removal(toRemoveParent, false);
            }
        }
    }
}


NodeRBT* RBT::getRoot() { // returns (NodeRBT*) root of the tree
    return root;
}

NodeRBT* RBT::search(int value) { // in first call - value | returns (NodeRBT*) if found, (nullptr) if not found
    return __search(this->getRoot(), value);
}

void RBT::traverse(NodeRBT* current, int level) { // in first call - node from which it should be traversed | traverses a tree in order (enable level by changing std::cout)
    if (!root) std::cout << "Tree empty. ";
    if (!current) return;
    traverse(current->getLesserChild(), level + 1);
    std::cout << (current->isRed() ? COLOR_RED : COLOR_BLACK) << current->getValue() << " " << COLOR_RESET << " | ";
    traverse(current->getGreaterChild(), level + 1);
}

void RBT::insert(int value) { // in first call - value | inserts a unique! element, for non-unique throws an invalid_argument exception
    if(!root) {
        root = new NodeRBT(value);
        root->assignRed(false);
        return;
    }
    if (!search(value)) __insert(this->getRoot(), this->getRoot()->getParent(), value, false);
    else throw std::invalid_argument("not a unique value");
}

void RBT::remove(int value) { // in first call - value | removes an existent node, for non-existent throws an invalid_argument exception
    NodeRBT* n = search(value);
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

int RBT::getBlackHeight() { // returns black height
    NodeRBT* current = root;
    int i = 0;
    while (current) {
        if (!(current->isRed())) ++i;
        current = current->getLesserChild();
    }
    return i + 1;
}

bool RBT::isRBT(NodeRBT* current, NodeRBT* prev, int blackDepth, bool previousRed) { // in first call - root | checks whether RBT is correct
    bool temp = current ? current->isRed() : false;
    if(!current) {
        if (!(blackDepth == getBlackHeight())) {
            std::cout << "\nBLACK DEPTH MISSMATCH | BLACK DEPTH " << blackDepth << " WITH " << getBlackHeight() << " | AT " << prev->getValue() << "\n";
        }
        return (blackDepth == getBlackHeight());
    }
    else if (temp && previousRed) {
        std::cout << "\nDOUBLE RED AT " << current->getValue() << "\n";
        return false;
    }
    else
        return ((isRBT(current->getLesserChild(), current, blackDepth + !(bool)(temp), temp)) && (isRBT(current->getGreaterChild(), current, blackDepth + !(bool)(temp), temp)));
}


RBT::~RBT() {
    NodeRBT* n = root;
    while (root) {
        __remove(n, 'n');
        n = root;
    }
}
