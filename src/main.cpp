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
    HuffmanTreeNode temp1 = HuffmanTreeNode(32,nullptr,0);
    x->AddNode(&temp1,'C');
    
    GenerateHuffmanTree(x);
    HuffmanMap_T y = x->GenerateWord2Code();
    while(1){
        unsigned char temp;
        cin>>temp;
        cout<<y.at(uchar2string(temp))<<endl;
    }
    return 0;
}