#ifndef NODEBST_H
#define NODEBST_H

class Node {
    private:
        Node* lesserChild;
        Node* greaterChild;
        int value;

    public:
        Node();
        Node(int);
        
        void assignLesserChild(Node*);
        void assignGreaterChild(Node*);
        void assignValue(int);
        Node* getLesserChild();
        Node* getGreaterChild();
        int getValue();

        ~Node();
};

inline Node::Node() : lesserChild(nullptr), greaterChild(nullptr), value(0) {}

inline Node::Node(int v) : lesserChild(nullptr), greaterChild(nullptr), value(v) {}

inline void Node::assignLesserChild(Node* l) {lesserChild = l;}
inline void Node::assignGreaterChild(Node* g) {greaterChild = g;}
inline void Node::assignValue(int val) {value = val;}

inline Node* Node::getLesserChild() {return lesserChild;}
inline Node* Node::getGreaterChild() {return greaterChild;}
inline int Node::getValue() {return value;}

inline Node::~Node() {
    lesserChild = nullptr;
    greaterChild = nullptr;
    value = 0;
}

#endif