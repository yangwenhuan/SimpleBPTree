# pragma once

#include <vector>

using namespace std;

struct BPTreeNode
{
    bool bIsLeaf; // 是否叶子节点
    std::vector<int> vecKey; // 键数组
    std::vector<BPTreeNode *> vecChild; // 子节点
};

class BPTree
{
public:
    BPTree(int iOutDegree);
    ~BPTree();

    int Search(int iKey); // 查找 select ... where field1 = iKey
    int ShowTree(); // 查找 select ...

    int Insert(int iKey); // 插入 insert into table (field1) values (key)

private:
    int SearchLeaf(int iKey, BPTreeNode * &pTargetLeaf); // 查找key所属的节点
    void SplitNode(BPTreeNode *pNode, BPTreeNode *pParent); // 节点满，自底向上拆分节点
    int InsertKey(int iKey, BPTreeNode * pNode);
    BPTreeNode * FindParent(BPTreeNode *pNode);

private:
    BPTreeNode *m_pRoot; // 根节点
    int m_iOutDegree; // 节点出度，比节点键数大1
};