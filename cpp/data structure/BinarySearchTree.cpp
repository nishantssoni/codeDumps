/*  
    code :Binary search tree with class
    operation: insert, find , min , max
    Author: Nishant Shekhar Soni
    Date : 18th june 2020 23:24 pm
    learning source: youtube.com/mycodeschool
    Note:here two versions of functions you found one private which
        uses root pointer and one for public which is simple
*/
#include <iostream>
using namespace std;

class BST
{
private:
    //Node
    struct Node
    {
        int data;
        Node *left;
        Node *right;
    };

    //root node
    Node *rootptr = NULL;

    //get root
    Node *getRoot()
    {
        return rootptr;
    }

    //set root
    void setRoot(Node *temp)
    {
        rootptr = temp;
    }

    //creating a node dinamcally
    Node *creatNode(int val)
    {
        Node *temp = new Node();

        temp->data = val;
        temp->left = NULL;
        temp->right = NULL;

        return temp;
    }

    //inserting a node in tree
    Node *insertNode(int val, Node *root)
    {
        // first case: if tree is empity
        //base case
        if (root == NULL)
        {
            root = creatNode(val);
        }
        else if (val <= root->data)
        {
            //recursive call
            root->left = insertNode(val, root->left);
        }
        else
        {
            //recursive call
            root->right = insertNode(val, root->right);
        }
        return root;
    }

    //private function for search
    bool search(int val, Node *root)
    {
        if (root == NULL)
        {
            return false; //base case
        }

        if (val == root->data)
        {
            return true;
        }

        else if (val <= root->data)
        {
            return search(val, root->left);
        }
        else
        {
            return search(val, root->right);
        }
    }

    //for minimum and maximum value in tree
    int minAndMax(Node *root, char choice)
    {
        //value of choice 'n' for minimum and 'x' for maximum
        if (choice == 'n')
        {
            if (root->left == NULL)
            {
                return root->data;
            }
            return minAndMax(root->left, choice); //for min
        }
        else
        {
            if (root->right == NULL)
            {
                return root->data;
            }
            return minAndMax(root->right, choice); //for max
        }
    }

    int maxOfTwoNumbers(int a , int b){
        return (a >= b)? a : b;
    }

    int heightBST(Node *root)
    {   
        if (root == NULL){
            return -1; //base case
        }else{
            cout<<"data :: "<<root->data<<endl;
        }
        
        int leftHeight = heightBST(root->left);
        int rightHeight = heightBST(root->right);
        
        
        return maxOfTwoNumbers(leftHeight, rightHeight)+1;
    }

public:
    void insert(int val)
    {
        setRoot(insertNode(val, getRoot()));
    }

    bool find(int val)
    {
        return search(val, getRoot());
    }

    int min()
    {
        if (getRoot() == NULL)
        {
            cout << "ERROR :: tree is empity" << endl;
            return -1;
        }
        return minAndMax(getRoot(), 'n');
    }

    int max()
    {
        if (getRoot() == NULL)
        {
            cout << "ERROR :: tree is empity" << endl;
            return -1;
        }
        return minAndMax(getRoot(), 'x');
    }

    int height(){
        return heightBST(getRoot());
    }
};

int main()
{
    BST t;
    t.insert(15);
    t.insert(9);
    t.insert(23);
    t.insert(3);
    t.insert(12);
    t.insert(8);
    t.insert(17);
    t.insert(23);


    cout << "is 8 is there ::  " << t.find(8) << endl;
    cout << "is 23 is there ::  " << t.find(23) << endl;
    cout << "is 12 is there ::  " << t.find(12) << endl;

    cout << "is 4 is there ::  " << t.find(4) << endl;
    cout << "is 644 is there ::  " << t.find(644) << endl;
    cout << "is 836 is there ::  " << t.find(836) << endl;

    cout << "minimum number in the tree is " << t.min() << endl;
    cout << "maximum number in the tree is " << t.max() << endl;

    cout << "Height of a tree is ::  " << t.height() << endl;

    return 0;
}