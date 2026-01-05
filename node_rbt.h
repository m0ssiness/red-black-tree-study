#ifndef NODERBT_H
#define NODERBT_H

class NodeRBT {
    private:
        NodeRBT* lesserChild;
        NodeRBT* greaterChild;
        NodeRBT* parent;
        int value;
        bool isR;

    public:
        NodeRBT();
        NodeRBT(int);
        
        void assignLesserChild(NodeRBT*);
        void assignGreaterChild(NodeRBT*);
        void assignParent(NodeRBT*);
        void assignValue(int);
        void assignRed(bool);
        NodeRBT* getLesserChild();
        NodeRBT* getGreaterChild();
        NodeRBT* getParent();
        int getValue();
        bool isRed();

        ~NodeRBT();
};

inline NodeRBT::NodeRBT() : lesserChild(nullptr), greaterChild(nullptr), parent(nullptr), value(0), isR(true) {}

inline NodeRBT::NodeRBT(int v) : lesserChild(nullptr), greaterChild(nullptr), parent(nullptr), value(v), isR(true) {}

inline void NodeRBT::assignLesserChild(NodeRBT* l) {lesserChild = l;}
inline void NodeRBT::assignGreaterChild(NodeRBT* g) {greaterChild = g;}
inline void NodeRBT::assignParent(NodeRBT* p) {parent = p;}
inline void NodeRBT::assignValue(int val) {value = val;}
inline void NodeRBT::assignRed(bool R) {isR = R;}

inline NodeRBT* NodeRBT::getLesserChild() {return lesserChild;}
inline NodeRBT* NodeRBT::getGreaterChild() {return greaterChild;}
inline NodeRBT* NodeRBT::getParent() {return parent;}
inline int NodeRBT::getValue() {return value;}
inline bool NodeRBT::isRed() {return isR;}

inline NodeRBT::~NodeRBT() {
    lesserChild = nullptr;
    greaterChild = nullptr;
    parent = nullptr;
    value = 0;
    isR = false;
}

#endif