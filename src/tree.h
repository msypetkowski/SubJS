#include "atom.h"

class TreeBuilder {
    string treeStr;
    int curTreeDepth;
    void prefix();
public:
    TreeBuilder();
    void treeNodeStart(const string&);
    void treeNodeEnd();
    void treeNodeAtom(Atom*);
    string getTreeStr() { return treeStr; }
};
