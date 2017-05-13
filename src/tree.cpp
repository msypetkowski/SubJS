#include "tree.h"

TreeBuilder::TreeBuilder()
    :curTreeDepth(0) {
    root = std::make_shared<Node>();
    curNode = root.get();
}

void TreeBuilder::treeNodeStart(const string& nodeName) {
    prefix();
    treeStr += nodeName;
    treeStr += '\n';
    ++curTreeDepth;

    auto newNode = std::make_shared<Node>();
    newNode->parent = curNode;
    newNode->name = nodeName;
    curNode->subNodes.push_back(newNode);
    curNode = newNode.get();
}

void TreeBuilder::treeNodeEnd() {
    --curTreeDepth;

    curNode = curNode->parent;
}

void TreeBuilder::treeNodeAtom(Atom * a) {
    prefix();
    treeStr += a->getStr();
    treeStr += '\n';

    auto newNode = std::make_shared<Node>();
    newNode->data = a;
    curNode->subNodes.push_back(newNode);
}

void TreeBuilder::prefix() {
    for(int i=0; i<curTreeDepth; ++i)
        treeStr += "--";
}
