#include "CodeTree.hpp"
#include "HuffmanTree.hpp"

#include <string>
#include <iostream>
using std::vector;
using std::string;
using std::pair;

vector<CodeTreeNode> codetreenodes;

void GenerateCodeTree(){
    codetreenodes.push_back(CodeTreeNode());
    for(pair<string,uc> i : code2word){
        int p = 0;
        for(int j = 0; j < i.first.size(); ++j){
            if(codetreenodes.at(p).child[i.first[j]]){
                p = codetreenodes.at(p).child[i.first[j]];
            }
            else
            {
                codetreenodes.at(p).child[i.first[j]] = codetreenodes.size();
                codetreenodes.push_back(CodeTreeNode());
                p = codetreenodes.at(p).child[i.first[j]];
            }
            if(j == i.first.size()-1){
                codetreenodes.at(p).word = i.second;
            }
        }
    }
    // int p=0;
    // while(1){
    //     int i;
    //     std::cin>>i;
    //     p = codetreenodes.at(p).child[i];
    //     if (codetreenodes.at(p).word){
    //         std::cout << codetreenodes.at(p).word << std::endl;
    //         p = 0;
    //     }
    // }
}