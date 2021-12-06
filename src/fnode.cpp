#include "fnode.hpp"
#include <algorithm>
#include <iostream>

FNode::FNode(std::string N, int T)
{
    type = T;
    name = N;
}

bool FNode::addChild(FNode *node)
{
    if (type == 1)
        return false;
    std::vector<FNode *>::iterator it = find(children.begin(), children.end(), node);
    if (it != children.end())
        return false;
    children.push_back(node);
    return true;
}

void FNode::printDirTree(int h = 0)
{
    std::cout << name << std::endl;
    if (type)
        return;
    for (std::vector<FNode *>::iterator it = children.begin(); it != children.end(); ++it)
    {
        if ((*it)->type)
        {
            for (int i = 1; i <= h; ++i)
                std::cout << "|   ";
            std::cout << "|---";
            (*it)->printDirTree(h + 1);
        }
        else
        {
            for (int i = 1; i <= h; ++i)
                std::cout << "|   ";
            std::cout << "|---";
            (*it)->printDirTree(h + 1);
        }
    }
}
FNode* FNode::findChild(std::string x)
{
    for(FNode* i : children){
        if(i->name == x) return i;
    }
    return 0;
}
