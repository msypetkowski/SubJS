#include "tree.h"

TreeBuilder::TreeBuilder()
    :curTreeDepth(0) {
}

void TreeBuilder::treeNodeStart(const string& nodeName) {
    prefix();
    treeStr += nodeName;
    treeStr += '\n';
    ++curTreeDepth;
}

void TreeBuilder::treeNodeEnd() {
    --curTreeDepth;
}

void TreeBuilder::treeNodeAtom(Atom * a) {
    prefix();
    treeStr += a->getStr();
    treeStr += '\n';
}

void TreeBuilder::prefix() {
    for(int i=0; i<curTreeDepth; ++i)
        treeStr += '.';
}
