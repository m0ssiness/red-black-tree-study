#ifndef NODEBST_H
#define NODEBST_H

class Node {
    private:
        Node* lesserChild;
        Node* greaterChild;
        Node* parent;
        int value;

    public:
        Node();
        Node(int);
        
        void assignLesserChild(Node*);
        void assignGreaterChild(Node*);
        void assignParent(Node*);
        void assignValue(int);
        Node* getLesserChild();
        Node* getGreaterChild();
        Node* getParent();
        int getValue();

        ~Node();
};

inline Node::Node() : lesserChild(nullptr), greaterChild(nullptr), parent(nullptr), value(0) {}

inline Node::Node(int v) : lesserChild(nullptr), greaterChild(nullptr), parent(nullptr), value(v) {}

inline void Node::assignLesserChild(Node* l) {lesserChild = l;}
inline void Node::assignGreaterChild(Node* g) {greaterChild = g;}
inline void Node::assignParent(Node* p) {parent = p;}
inline void Node::assignValue(int val) {value = val;}

inline Node* Node::getLesserChild() {return lesserChild;}
inline Node* Node::getGreaterChild() {return greaterChild;}
inline Node* Node::getParent() {return parent;}
inline int Node::getValue() {return value;}

inline Node::~Node() {
    lesserChild = nullptr;
    greaterChild = nullptr;
    parent = nullptr;
    value = 0;
}

#endif