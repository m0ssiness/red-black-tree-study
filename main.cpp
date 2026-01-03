#include <iostream>
#include <chrono>
#include <random>

#define COLOR_RED     "\033[1;31m"
#define COLOR_BLACK   "\033[1;36m"
#define COLOR_RESET   "\033[0m"
#define BOLD   "\033[1;37m"

using namespace std;

class Node {
    Node* lesserChild = nullptr;
    Node* greaterChild = nullptr;
    Node* parent = nullptr;
    int value;

    public:
    Node(){value = 0;}
    Node(int val){value = val;}

    void assignLesserChild(Node* l) {
        lesserChild = l;
    }

    void assignGreaterChild(Node* g) {
        greaterChild = g;
    }

    void assignParent(Node* p) {
        parent = p;
    }

    void assignValue(int val) {
        value = val;
    }

    Node* getLesserChild() {
        return lesserChild;
    }

    Node* getGreaterChild() {
        return greaterChild;
    }

    Node* getParent() {
        return parent;
    }

    int getValue() {
        return value;
    }

    ~Node() {
        lesserChild = nullptr;
        greaterChild = nullptr;
        parent = nullptr;
        value = 0;
    }
};

class NodeRBT {
    NodeRBT* lesserChild = nullptr;
    NodeRBT* greaterChild = nullptr;
    NodeRBT* parent = nullptr;
    int value;
    bool isR;

    public:
    NodeRBT(){isR = true; value = 0;}
    NodeRBT(int val){isR = true; value = val;}

    void assignLesserChild(NodeRBT* l) {
        lesserChild = l;
    }

    void assignGreaterChild(NodeRBT* g) {
        greaterChild = g;
    }

    void assignParent(NodeRBT* p) {
        parent = p;
    }

    void assignValue(int val) {
        value = val;
    }

    void assignRed(bool R) {
        isR = R;
    }

    NodeRBT* getLesserChild() {
        return lesserChild;
    }

    NodeRBT* getGreaterChild() {
        return greaterChild;
    }

    NodeRBT* getParent() {
        return parent;
    }

    int getValue() {
        return value;
    }

    bool isRed() {
        return isR;
    }
};

class BST {
    Node* root = nullptr;

    void __insert(Node* current, Node* parent, int value, bool isCurrentLesserChild) { // in first call - root, nullptr, value, true/false (as at least one recursive iteration is guaranteed due to BST constructor)      
        if (!current) {
            Node* n = new Node(value);
            n->assignParent(parent);
            if (isCurrentLesserChild) parent->assignLesserChild(n);
            else parent->assignGreaterChild(n);
            return;
        }
        value < current->getValue() ? __insert(current->getLesserChild(), current, value, true) : __insert(current->getGreaterChild(), current, value, false);
    }
    Node* __search(Node* current, int value) { // in first call - root, value | returns (Node*) if found, (nullptr) if not found
        if (!current || current->getValue() == value) return current;
        return value < current->getValue() ? __search(current->getLesserChild(), value) : __search(current->getGreaterChild(), value);
    }
    Node* __remove__findSuccessor(Node* successor, bool & isLeftChild) { // in first call - the greater child of the node to remove
        if (!(successor->getLesserChild())) {
            return successor;
        }
        isLeftChild = true;
        return __remove__findSuccessor(successor->getLesserChild(), isLeftChild);
    }
    void __remove(Node* toRemove, char whichChild) { // in first call - node to remove, lesser/greater/root(n) child    
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
                        cout << "Deleting root\n";
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

    public:
    BST () {}
    BST (int val) {
        root = new Node(val);
    }

    Node* getRoot() { // returns (Node*) root of the tree
        return root;
    }

    Node* search(int value) { // in first call - value | returns (Node*) if found, (nullptr) if not found
        return __search(this->getRoot(), value);
    }

    void insert(int value) { // in first call - value
        if(!root) {
            root = new Node(value);
            return;
        }
        if (!search(value)) __insert(this->getRoot(), this->getRoot()->getParent(), value, false);
        else cout << "No insertion of existent elements available\n";
    }

    void traverse(Node* current) { // in first call - node from which it should be traversed
        if (!root) cout << "Tree empty. ";
        if (!current) return;
        traverse(current->getLesserChild());
        cout << current->getValue() << " ";
        traverse(current->getGreaterChild());
    }

    void remove(int value) { // in first call - value
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
        else cout << "No such node available\n";  
    }

    ~BST(){
        Node* n = root;
        while (root) {
            __remove(n, 'n');
            n = root;
        }
    }
};

class RBT {
    NodeRBT* root = nullptr;

    NodeRBT* __search(NodeRBT* current, int value) { // in first call - root, value | returns (NodeRBT*) if found, (nullptr) if not found
        if (!current || current->getValue() == value) return current;
        return value < current->getValue() ? __search(current->getLesserChild(), value) : __search(current->getGreaterChild(), value);
    }

    void __leftRotation(NodeRBT* pivot) { //call - rotation node
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

    void __rightRotation(NodeRBT* pivot) { //call - rotation node
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
    void __rebalance(NodeRBT* current) { // call - node to rebalance around, rebalancing insertion
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
    void __insert(NodeRBT* current, NodeRBT* parent, int value, bool isCurrentLesserChild) { // in first call - root, nullptr, value, true/false      
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

    void __rebalance_removal(NodeRBT* doubleBlackParent, bool isDoubleBlackLesserChild) {
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

    NodeRBT* __remove__findSuccessor(NodeRBT* successor, bool & isLeftChild) { // in first call - the greater child of the node to remove
        if (!(successor->getLesserChild())) {
            return successor;
        }
        isLeftChild = true;
        return __remove__findSuccessor(successor->getLesserChild(), isLeftChild);
    }

    void __remove(NodeRBT* toRemove, char whichChild) { // in first call - node to remove, lesser/greater/root(n) child    
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
                        cout << "Deleting root\n";
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

    public:
    RBT() {};
    RBT(int val) {
        root = new NodeRBT(val);
        root->assignRed(false);
    }

    NodeRBT* getRoot() { // returns (NodeRBT*) root of the tree
        return root;
    }

    NodeRBT* search(int value) { // in first call - value | returns (NodeRBT*) if found, (nullptr) if not found
        return __search(this->getRoot(), value);
    }

    void traverse(NodeRBT* current, int level = 0) { // in first call - node from which it should be traversed
        if (!root) cout << "Tree empty. ";
        if (!current) return;
        traverse(current->getLesserChild(), level + 1);
        cout << (current->isRed() ? COLOR_RED : COLOR_BLACK) << current->getValue() << " " << COLOR_RESET << " | ";
        traverse(current->getGreaterChild(), level + 1);
    }

    void insert(int value) { // in first call - value
        if(!root) {
            root = new NodeRBT(value);
            root->assignRed(false);
            return;
        }
        if (!search(value)) __insert(this->getRoot(), this->getRoot()->getParent(), value, false);
        else cout << "No insertion of existent elements available\n";
    }
    void remove(int value) { // in first call - value
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
        else cout << "No such node available\n";  
    }
    int getBlackHeight() { // returns black height
        NodeRBT* current = root;
        int i = 0;
        while (current) {
            if (!(current->isRed())) ++i;
            current = current->getLesserChild();
        }
        return i + 1;
    }
    bool isRBT(NodeRBT* current, NodeRBT* prev = nullptr, int blackDepth = 1, bool previousRed = false) { // in first call - root | checks whether RBT is correct
        bool temp = current ? current->isRed() : false;
        //cout << blackDepth << " ";
        if(!current) {
            if (!(blackDepth == getBlackHeight())) {
                cout << "\nBLACK DEPTH MISSMATCH | BLACK DEPTH " << blackDepth << " WITH " << getBlackHeight() << " | AT " << prev->getValue() << "\n";

            }
            return (blackDepth == getBlackHeight());
        }
        else if (temp && previousRed) {
            cout << "\nDOUBLE RED AT " << current->getValue() << "\n";
            return false;
        }
        else
            return ((isRBT(current->getLesserChild(), current, blackDepth + !(bool)(temp), temp)) && (isRBT(current->getGreaterChild(), current, blackDepth + !(bool)(temp), temp)));
    }
    ~RBT() {
        NodeRBT* n = root;
        while (root) {
            __remove(n, 'n');
            n = root;
        }
    }
};

int getRandomNumber(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

long long measuredOperation(BST* & tree, char type, int temp = 0) {
    auto start = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();

    switch(type) {
            case 'i' : {
                start = std::chrono::steady_clock::now();
                tree->insert(temp);
                end = std::chrono::steady_clock::now();
                break;
            };
            case 'd' : {
                start = std::chrono::steady_clock::now();
                tree->remove(temp);
                end = std::chrono::steady_clock::now();
                break;
            };
            case 's' : {
                start = std::chrono::steady_clock::now();
                Node* tem = tree->search(temp);
                end = std::chrono::steady_clock::now();
                if(!tem) cout << "\nNo such element\n";
                break;
            };
            case 't' : {
                start = std::chrono::steady_clock::now();
                tree->traverse(tree->getRoot());
                end = std::chrono::steady_clock::now();
                break;
            };
    }
    return (chrono::duration_cast<std::chrono::microseconds>(end-start)).count();
}

long long measuredOperation(RBT* & tree, char type, int temp = 0) {
    auto start = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();

    switch(type) {
            case 'i' : {
                start = std::chrono::steady_clock::now();
                tree->insert(temp);
                end = std::chrono::steady_clock::now();
                break;
            };
            case 'd' : {
                start = std::chrono::steady_clock::now();
                tree->remove(temp);
                end = std::chrono::steady_clock::now();
                break;
            };
            case 's' : {
                start = std::chrono::steady_clock::now();
                NodeRBT* tem = tree->search(temp);
                end = std::chrono::steady_clock::now();
                if(!tem) cout << "\nNo such element\n";
                break;
            };
            case 'b' : {
                start = std::chrono::steady_clock::now();
                cout << "\nBlack height is: " << tree->getBlackHeight() << '\n';
                end = std::chrono::steady_clock::now();
                break;
            };
            case 't' : {
                start = std::chrono::steady_clock::now();
                tree->traverse(tree->getRoot());
                end = std::chrono::steady_clock::now();
                break;
            };
            case 'q' : {
                start = std::chrono::steady_clock::now();
                if (tree->isRBT(tree->getRoot())) cout << "\ncorrect\n";
                else cout << "\nNOT correct\n";
                end = std::chrono::steady_clock::now();
                break;
            };
    }

    return (chrono::duration_cast<std::chrono::microseconds>(end-start)).count();
}

void preset1(BST* & bst, RBT* & rbt) { // CONSEQUENT FILL
    for (int i = 1; i < 10000; ++i) {
        bst->insert(i);
        rbt->insert(i);
    }
}

void preset2(BST* & bst, RBT* & rbt) { // RANDOM FILL
    int a = 0;
    for (int i = 1; i < 100000; ++i) {
        a = getRandomNumber(0, 100000);
        bst->insert(a);
        rbt->insert(a);
    }
}

int main() {
    RBT* RBTtree = new RBT();
    BST* BSTtree = new BST();
    char t = ' ';
    char tree = ' ';
    int n = 0;

    //presets there
    //preset1(BSTtree, RBTtree);
    preset2(BSTtree, RBTtree);
    //presets end

    cout << BOLD << "\nPerform measured opration [input: tree(R - RBT, B - BST), type, number]\ni - insert\nd - delete\ns - search\nt - traverse\nb(RBT only) - get black height\nq(RBT only) - check RBT correctness\ne - exit\n" << COLOR_RESET;

    while (true) {
        cout << "Input: ";
        cin >> tree;
        if (tree == 'e') break;
        cin >> t >> n;
        if (tree == 'B') cout << "\nPerformed " << measuredOperation(BSTtree, t, n) << " microseconds\n";
        else if (tree == 'R') cout << "\nPerformed " << measuredOperation(RBTtree, t, n) << " microseconds\n";
    }


    delete BSTtree;
    delete RBTtree;
    return 0;
}