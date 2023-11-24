#include "simple_bptree.h"
#include <cstdio>
#include <algorithm>
#include <queue>

BPTree::BPTree(int iOutDegree) : m_pRoot(nullptr), m_iOutDegree(iOutDegree)
{
}

BPTree::~BPTree()
{
}

int BPTree::Search(int iKey)
{
    BPTreeNode *pTargetLeaf = nullptr;
    int iRet = SearchLeaf(iKey, pTargetLeaf);
    if(iRet)
    {
        
        return iRet;
    }

    int iDataIndex = std::lower_bound(pTargetLeaf->vecKey.begin(), pTargetLeaf->vecKey.end(), iKey) - pTargetLeaf->vecKey.begin(); // 二分查找第1个键 >= iKey
    if(iDataIndex == pTargetLeaf->vecKey.size() || pTargetLeaf->vecKey[iDataIndex] != iKey) // 找不到
    {
        printf("err: iKey=%d not found!\n", iKey);
        return 1;
    }

    return 0;
}

int BPTree::SearchLeaf(int iKey, BPTreeNode * &pTargetLeaf)
{
    if(m_pRoot == nullptr) // 树空
    {
        printf("err: tree is empty! iKey=%d\n", iKey);
        return 1;
    }
    else // 树非空
    {
        BPTreeNode *pNode = m_pRoot;
        while(!(pNode->bIsLeaf)) // 非叶子节点，通常最多3阶
        {
            int iChildIndex = std::lower_bound(pNode->vecKey.begin(), pNode->vecKey.end(), iKey) - pNode->vecKey.begin(); // 二分查找第1个键 >= iKey
            pNode = pNode->vecChild[iChildIndex]; // 子节点继续
        }
        pTargetLeaf = pNode;
    }
    return 0;
}

int BPTree::ShowTree()
{
    if(m_pRoot == nullptr)
    {
        printf("info: tree is empty.\n");
    }
    else // BFS 层级遍历
    {
        int iLevel = 1;
        std::queue<BPTreeNode *> oQueue;
        oQueue.push(m_pRoot);
        while(!oQueue.empty())
        {
            printf("info: level %d:    ", iLevel);
            int iLevelNodes = oQueue.size();

            for(int j = 0; j < iLevelNodes; j++)
            {
                BPTreeNode *pNode = oQueue.front();
                for(int i = 0; i < pNode->vecKey.size(); i++)
                    printf("%d ", pNode->vecKey[i]);
                oQueue.pop();
                if(!(pNode->bIsLeaf))
                {
                    for(int i = 0; i < pNode->vecChild.size(); i++)
                        oQueue.push(pNode->vecChild[i]);
                }
                printf("    ");
            }
            printf("\n");
            iLevel++;
        }
    }
    
    return 0;
}

int BPTree::Insert(int iKey)
{
    if(m_pRoot == nullptr) // 树空，即没有叶子节点
    {
        m_pRoot = new BPTreeNode; // 新建节点
        m_pRoot->bIsLeaf = true; // 叶子节点
        m_pRoot->vecKey.push_back(iKey); // 存键
    }
    else // 树非空，有叶子节点
    {
        BPTreeNode *pNode = m_pRoot;
        BPTreeNode *pParent = nullptr;
        while(!(pNode->bIsLeaf)) // 非叶子节点，通常最多3层
        {
            pParent = pNode; // 父节点
            int iChildIndex = std::lower_bound(pNode->vecKey.begin(), pNode->vecKey.end(), iKey) - pNode->vecKey.begin(); // 二分查找第1个键 >= iKey
            pNode = pNode->vecChild[iChildIndex]; // 子节点继续
        }
        InsertKey(iKey, pNode);
        if(pNode->vecKey.size() >= m_iOutDegree) // 叶子节点溢出
        {
            SplitNode(pNode, pParent); // 拆分节点
        }
    }

    return 0;
}

int BPTree::InsertKey(int iKey, BPTreeNode * pNode)
{
    if(pNode == nullptr)
    {
        printf("err: node is null!\n");
        return -1;
    }
    int i = (int)pNode->vecKey.size() - 1; // 最右
    pNode->vecKey.resize(pNode->vecKey.size() + 1); // 分配空间
    while(i >= 0 && pNode->vecKey[i] > iKey)
    {
        pNode->vecKey[i+1] = pNode->vecKey[i]; // 右移1位
        i--;
    }
    pNode->vecKey[i+1] = iKey;
    return i + 1;
}

void BPTree::SplitNode(BPTreeNode *pNode, BPTreeNode *pParent)
{
    BPTreeNode *pNewNode = new BPTreeNode;
    pNewNode->bIsLeaf = pNode->bIsLeaf;
    pNewNode->vecKey.assign(pNode->vecKey.begin() + m_iOutDegree / 2, pNode->vecKey.end()); // 分一半键到新节点

    pNode->vecKey.resize(m_iOutDegree / 2); // 原子节点删掉一半键
    if(!(pNode->bIsLeaf)) // 非叶子节点
    {
        pNewNode->vecChild.assign(pNode->vecChild.begin() + m_iOutDegree / 2 + 1, pNode->vecChild.end()); // 分一半子节点到新子节点
        pNode->vecChild.resize(m_iOutDegree / 2 + 1); // 原子节点删掉一半子节点
    }

    if(pParent == nullptr)
    {
        pParent = new BPTreeNode;
        pParent->bIsLeaf = false;
        pParent->vecChild.push_back(pNode);
        m_pRoot = pParent; // 更新根节点
    }

    int iSplitIndex = InsertKey(pNewNode->vecKey[0], pParent);

    pParent->vecChild.insert(pParent->vecChild.begin() + iSplitIndex + 1, pNewNode); // 新子节点

    if(pParent->vecKey.size() >= m_iOutDegree) // 父节点溢出
    {
        SplitNode(pParent, FindParent(pParent)); // 继续拆
    }
}

BPTreeNode * BPTree::FindParent(BPTreeNode *pNode)
{
    if(pNode == nullptr || m_pRoot == nullptr)
    {
        return nullptr;
    }
    int iKey = pNode->vecKey[0];
    
    std::vector<BPTreeNode *> vecNodeChain;
    BPTreeNode *pCurrent = m_pRoot;
    while(!(pCurrent->bIsLeaf)) // 非叶子节点，通常最多3层
    {
        vecNodeChain.push_back(pCurrent);
        int iChildIndex = std::lower_bound(pCurrent->vecKey.begin(), pCurrent->vecKey.end(), iKey) - pCurrent->vecKey.begin(); // 二分查找第1个键 >= iKey
        pCurrent = pCurrent->vecChild[iChildIndex]; // 子节点继续
    }
    vecNodeChain.push_back(pCurrent);
    for(int i = vecNodeChain.size() - 1; i >= 1; i--)
    {
        if(vecNodeChain[i] == pNode)
        {
            return vecNodeChain[i - 1];
        }
    }
    return nullptr;
}