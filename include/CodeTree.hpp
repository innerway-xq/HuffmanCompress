#ifndef CODETREE_H
#define CODETREE_H

#define uc unsigned char

#include <vector>

class CodeTreeNode
{
public:
    uc word = 0;
    int child[2] = {0,0}; // 0 - left, 1 - right
};
extern std::vector<CodeTreeNode> codetreenodes;
void GenerateCodeTree();

#endif