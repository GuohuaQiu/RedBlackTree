#pragma once
#include <sstream>
using namespace std;

typedef enum{
    RED = 1,
    BLACK = 0
}COLOR;

class TreeNode
{
private:
    TreeNode *Insert(int x);
    void InsertAdjust(TreeNode* newNode);

public:
    int mValue;
    COLOR mColor;
    TreeNode *mLeft;
    TreeNode *mRight;
    TreeNode *mParent;
    TreeNode(int x);
    
    ~TreeNode();
    void LeftRotate();
    void RightRotate();
    TreeNode* GetHead();

    TreeNode* Append(int x);

    void SetColor(COLOR color);
    TreeNode *Father();
    TreeNode *Left();
    TreeNode *Right();
    void Print();
    void CalcWidth();
    void CalcPos();
    void GetString(string& str);

public:
    int mWidth;
    int mPos;//center pos.
    int mLine;
};