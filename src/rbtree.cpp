#include <iostream>
#include <sstream>
#include <functional>

#include "rbtree.h"
#include <deque>

using namespace std;

TreeNode::TreeNode(int x) : mValue(x), mLeft(nullptr), mRight(nullptr), mParent(nullptr), mColor(RED)
{
}

TreeNode::~TreeNode()
{
}

void TreeNode::SetColor(COLOR color)
{
    mColor = color;
}
TreeNode *TreeNode::Father()
{
    return mParent;
}
TreeNode *TreeNode::Left()
{
    return mLeft;
}
TreeNode *TreeNode::Right()
{
    return mRight;
}

void TreeNode::LeftRotate()
{
    TreeNode *pfather = mParent;
    // TreeNode* pleft = mLeft;
    TreeNode *pright = mRight;
    TreeNode *prl = mRight->Left();

    if (pfather != nullptr)
    {
        if (pfather->mLeft == this)
        {
            pfather->mLeft = pright;
        }
        else
        {
            pfather->mRight = pright;
        }
    }

    pright->mParent = pfather;

    pright->mLeft = this;
    mParent = pright;

    mRight = prl;
    if (prl != nullptr)
        prl->mParent = this;
    // maybe left;
}

void TreeNode::RightRotate()
{
    TreeNode *pfather = mParent;
    // TreeNode* pleft = mLeft;
    TreeNode *pleft = mLeft;
    TreeNode *plr = mLeft->Right();

    if (pfather != nullptr)
    {
        if (pfather->mLeft == this)
        {
            pfather->mLeft = pleft;
        }
        else
        {
            pfather->mRight = pleft;
        }
    }
    pleft->mParent = pfather;

    pleft->mRight = this;
    mParent = pleft;

    mLeft = plr;
    if (plr != nullptr)
        plr->mParent = this;
    // maybe left;
}

TreeNode *TreeNode::Append(int x)
{
    TreeNode *pnew = Insert(x);
    InsertAdjust(pnew);
    return pnew->GetHead();
}

TreeNode *TreeNode::GetHead()
{
    TreeNode *me = this;
    TreeNode *father = mParent;
    while (father != nullptr)
    {
        me = father;
        father = father->mParent;
    }
    return me;
}

TreeNode *TreeNode::Insert(int x)
{
    TreeNode *pnew = nullptr;
    if (x < mValue)
    {
        if (mLeft == nullptr)
        {
            pnew = new TreeNode(x);
            mLeft = pnew;
            pnew->mParent = this;
        }
        else
        {
            pnew = mLeft->Insert(x);
        }
    }
    else
    {
        if (mRight == nullptr)
        {
            pnew = new TreeNode(x);
            mRight = pnew;
            pnew->mParent = this;
        }
        else
        {
            pnew = mRight->Insert(x);
        }
    }
    return pnew;
}

static TreeNode *ChangeUpperColor(TreeNode *node)
{
    // 父节点变黑，叔叔节点变黑，爷爷节点变红，node节点指向爷爷节点
    TreeNode *grandfather = node->Father()->Father();
    grandfather->mLeft->SetColor(BLACK);
    grandfather->mRight->SetColor(BLACK);

    if (grandfather->mParent != nullptr)
    {
        grandfather->SetColor(RED);
        return grandfather;
    }
    return nullptr;
}

// 叔叔节点是黑色
static void Rotate(TreeNode *node)
{
    TreeNode *const father = node->Father();
    TreeNode *const sf = father->Father();

    int left_rotate = 1;

    if (sf->Left() == father)
    {
        left_rotate = 0;
    }

    int twice = 0;

    if ((father->Left() == node && left_rotate == 1) || (father->Right() == node && left_rotate == 0))
    {
        twice = 1;
    }

    if (twice == 1)
    {
        if (left_rotate == 1)
        {
            father->RightRotate();
        }
        else
        {
            father->LeftRotate();
        }
    }
    if (left_rotate == 0)
    {
        sf->RightRotate();
    }
    else
    {
        sf->LeftRotate();
    }

    sf->SetColor(RED);
    sf->Father()->SetColor(BLACK);
}

void TreeNode::InsertAdjust(TreeNode *newNode)
{
    // 这里我们假设节点已经插入到了合适的位置, 这里只按按照规则进行调整
    // 将新插入节点的颜色进行调整
    //
    newNode->SetColor(RED);
    // 情况1
    if (newNode->Father() == nullptr)
    {
        newNode->SetColor(BLACK);
        return;
    }
    // 情况2
    if (newNode->Father()->mColor == RED)
    {
        TreeNode *node = newNode;
        // 情况3, 4, 5都要对左右孩子区别开
        while (node != nullptr && node->Father()->mColor == RED)
        {
            // 插入的节点在爷爷节点的左子树上
            if (node->Father()->Father()->Left() == node->Father())
            {
                TreeNode *uncle = node->Father()->Father()->Right();
                // 情况3
                if (uncle != nullptr && uncle->mColor == RED)
                {
                    node = ChangeUpperColor(node);
                }
                else
                {
                    // 叔叔节点是黑色
                    Rotate(node);
                    break;
                }
            }
            // 插入的节点在爷爷节点的右子树上
            else
            {
                TreeNode *uncle = node->Father()->Father()->Left();
                // 情况3
                if (uncle != nullptr && uncle->mColor == RED)
                {
                    node = ChangeUpperColor(node);
                }
                else
                {
                    // 叔叔节点是黑色
                    Rotate(node);
                    break;
                }
            }
        }
    }
}

// /*可变长树枝"┌─────┴─────┐"*/
// void OutBranch(int haveLeft, int haveRight, int interval)
// {
//     if (haveLeft)
//     {
//         printf("┌");
//         for (int i = 0; i < pow(2, interval) / 2 - 1; i++)
//             printf("─");
//         if (haveRight)
//         {
//             /*"┌─────┴─────┐"*/
//             printf("┴");
//             for (int i = 0; i < pow(2, interval) / 2 - 1; i++)
//                 printf("─");
//             printf("┐");
//         }
//         else
//         {
//             /*"┌─────┘        "*/
//             printf("┘");
//             for (int i = 0; i < pow(2, interval) / 2 - 1; i++)
//                 printf(" ");
//             printf(" ");
//         }
//     }
//     else
//     {
//         printf(" ");
//             for (int i = 0; i < pow(2, interval) / 2 - 1; i++)
//                 printf(" ");
//         if(haveRight)
//         {
//             /*"      └──────┐"*/
//             printf("└");
//             for (int i = 0; i < pow(2, interval) / 2 - 1; i++)
//                 printf("─");
//             printf("┐");
//         }
//         else
//         {
//             /*"              "*/
//             printf(" ");
//             for (int i = 0; i < pow(2, interval) / 2 - 1; i++)
//                 printf(" ");
//             printf(" ");
//         }
//     }
// }

void TreeNode::GetString(string &str)
{
    stringstream ss;
    if (mColor == BLACK)
    {
        ss << "B";
    }
    else
    {
        ss << "R";
    }
    ss << mValue;
    str = ss.str();
}

void TreeNode::Print()
{
    CalcWidth();
    CalcPos();

    deque<TreeNode *> queue;

    TreeNode *node = this;
    int curline = 0;
    // int pos = 0;
    string snum;
    string line;
    // center pos:
    //  if it is a left tree, pos =  parent_pos - right_sub_tree_width
    //  if it is a right tree pos = parent_pos + left_sub_tree_width
    //  if it is head , pos = left_sub_tree_width
    snum.append(100, ' ');
    do
    {
        if (node->mLine > curline)
        {
            cout << snum << endl;
            // pos = 0;
            snum.clear();
            snum.append(100, ' ');
            curline++;
        }
        string str;
        node->GetString(str);
        snum.replace(node->mPos, 1, str);

        if (node->mLeft != nullptr)
        {
            queue.push_back(node->mLeft);
        }
        if (node->mRight != nullptr)
        {
            queue.push_back(node->mRight);
        }
        node = queue.front();
        if (node)
        {
            queue.pop_front();
        }
    } while (node != nullptr);
    cout << snum << endl;
}

#define BLANK_W 3

void TreeNode::CalcWidth()
{
    mLine = 0;
    if (mParent != nullptr)
    {
        mLine = mParent->mLine + 1;
    }
    if (mLeft != nullptr)
    {
        mLeft->CalcWidth();
    }
    if (mRight != nullptr)
    {
        mRight->CalcWidth();
    }

    if (mLeft != nullptr && mRight != nullptr)
    {
        mWidth = mLeft->mWidth + mRight->mWidth + BLANK_W;
    }
    else if (mLeft != nullptr)
    {
        mWidth = mLeft->mWidth + BLANK_W;
    }
    else if (mRight != nullptr)
    {
        mWidth = mRight->mWidth + BLANK_W;
    }
    else
    {
        stringstream ss;
        ss << mValue;
        mWidth = ss.str().size() + BLANK_W;
    }
}

void TreeNode::CalcPos()
{
    if (mParent == nullptr)
    {
        if (mLeft != nullptr)
        {
            mPos = mLeft->mWidth;
        }
        else
        {
            mPos = 0;
        }
    }
    else
    {
        mPos = mParent->mPos;
        if (mParent->mLeft == this)
        {
            if (mRight)
            {
                mPos -= mRight->mWidth;
            }
            else
            {
                mPos -= 2;
            }
        }
        else
        {
            if (mLeft)
            {
                mPos += mLeft->mWidth;
            }
            else
            {
                mPos += 2;
            }
        }
    }

    if (mLeft)
    {
        mLeft->CalcPos();
    }
    if (mRight)
    {
        mRight->CalcPos();
    }
    // cout<< mValue << " pos:" << mPos<<" w:"<<mWidth<<" line:"<<mLine<<endl;
    // if (mParent != nullptr)
    // {
    //     cout << "parent:" << mParent->mValue << endl;
    // }
}
