#include "HuffmanTree.hpp"
#include "xqzio.hpp"
#include <iostream>
#include <cstring>
#include <windows.h>
using namespace std;

int main()
{
    xqz_in(".\\sample\\test");
    
    HuffmanForest* x = new HuffmanForest;
    HuffmanTreeNode* nodes_array = new HuffmanTreeNode[256];
    for(int i = 0; i<256; ++i){
        if(word_freq[i]){
            nodes_array[i] = HuffmanTreeNode(word_freq[i], nodes_array + i);
            x->AddNode(nodes_array+i,(uc)i);
        }
    }
    GenerateHuffmanTree(x);
    cout<<"GenerateHuffmanTree finished"<<endl;
    Word2Code_T y = x->GenerateWord2Code();
    while(1){
        uc temp;
        cin>>temp;
        cout<<y.at(temp)<<endl;
    }
    return 0;
}