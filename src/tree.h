#pragma once
#include "atom.h"

#include <memory>

struct Node {
    string name;
    Atom* data;
    Node* parent;
    std::vector<std::shared_ptr<Node>> subNodes;

    Node():name("ATOM"),data(nullptr),parent(nullptr) {}
};

class TreeBuilder {
    string treeStr;
    std::shared_ptr<Node> root;
    Node* curNode;
    int curTreeDepth;
    void prefix();
public:
    TreeBuilder();
    void treeNodeStart(const string&);
    void treeNodeEnd();
    void treeNodeAtom(Atom*);
    string getTreeStr() { return treeStr; }
    std::shared_ptr<Node> getTreeRoot()const {return root;};
};
