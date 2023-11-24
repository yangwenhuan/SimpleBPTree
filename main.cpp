#include "simple_bptree.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    BPTree oBPTree(32);

    for(int i = 0; i < 500; i++)
        oBPTree.Insert(i);
    oBPTree.ShowTree();

    return 0;
}