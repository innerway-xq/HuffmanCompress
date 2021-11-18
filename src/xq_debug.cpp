#include "xq_debug.hpp"

#include "HuffmanTree.hpp"
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

void testword2code(Word2Code_T word2code)
{
    while (1)
    {
        int x;
        cin >> x;
        if (x == 1024)
            break;
        for (int i = 0; i < word2code[(uc)(x)].size(); ++i)
        {
            cout << (char)(word2code[(uc)(x)][i] + '0');
        }
        cout << endl;
    }
}