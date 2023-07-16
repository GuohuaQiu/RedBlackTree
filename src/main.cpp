#include <iostream>
#include <functional>
#include "rbtree.h"

using namespace std;

int main(int argc , char* argv[]){

    if (argc < 2)
    {
        cout << "no input!" << endl;
        return -1;
    }

    stringstream ss;
    TreeNode* head = nullptr;

    cout<< argc<<endl;

    for (int i = 1; i < argc; i++)
    {
        int x;
        ss.clear();
        ss << argv[i];
        ss >> x;
        if (i == 1)
        {
            head = new TreeNode(x);
            head->SetColor(BLACK);
        }
        else
        {
            head = head->Append(x);
        }
    }

    head->Print();
    return 0;
}