#include <iostream>
#include <chrono>
#include <random>

#include "bst.h"
#include "rbt.h"

#define COLOR_RESET   "\033[0m"
#define BOLD   "\033[1;37m"

using namespace std;

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
    return (chrono::duration_cast<std::chrono::nanoseconds>(end-start)).count();
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

    return (chrono::duration_cast<std::chrono::nanoseconds>(end-start)).count();
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
        a = getRandomNumber(0, 1000000);
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
    preset1(BSTtree, RBTtree);
    //preset2(BSTtree, RBTtree);
    //presets end

    cout << BOLD << "\nPerform measured opration [input: tree(R - RBT, B - BST), type, number]\ni - insert\nd - delete\ns - search\nt - traverse\nb(RBT only) - get black height\nq(RBT only) - check RBT correctness\ne - exit\n" << COLOR_RESET;

    while (true) {
        cout << "Input: ";
        cin >> tree;
        if (tree == 'e') break;
        cin >> t >> n;
        if (tree == 'B') cout << "\nPerformed " << measuredOperation(BSTtree, t, n) << " nanoseconds\n";
        else if (tree == 'R') cout << "\nPerformed " << measuredOperation(RBTtree, t, n) << " nanoseconds\n";
    }


    delete BSTtree;
    delete RBTtree;
    return 0;
}