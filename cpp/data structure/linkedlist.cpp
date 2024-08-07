//NISHANT SHEKHAR SONI
//LINKED LIST
#include <iostream>
using namespace std;

struct node //node
{
    int info;
    struct node *link;
};

struct node *start = NULL; //starting pointer

struct node *creatNode() //new node
{
    struct node *temp;
    temp = (struct node *)malloc(sizeof(struct node)); //dynamic memory allocation
    return temp;
}

void insertNode(int in) //add node at last
{
    struct node *temp, *p;

    temp = creatNode();
    temp->info = in;
    temp->link = NULL;

    if (start == NULL) //if program doesent have node
    {
        start = temp;
    }
    else
    {
        p = start; //if there is alredy node
        while (p->link != NULL)
        {
            p = p->link;
        }
        p->link = temp;
    }
}

void deletAtStart()
{
    if (start == NULL)
    {
        cout << "you don't have any node" << endl;
    }
    else
    {
        struct node *temp;
        temp = start;
        //temp->link = start;  //getting address of first node for deletion
        start = start->link; //now start pointing to first node *link pointer which means 2nd node
        free(temp);          //deleting first node
    }
}

void printList()
{
    if (start == NULL)
    {
        cout << "you don't have list " << endl;
    }
    else
    {
        struct node *temp = start;
        while (temp != NULL) 
        {
            cout << temp->info << endl;
            temp = temp->link;
        }
    }
}

int main()
{
     int choice = 1;

    while (choice != 4)
    {   cout<<"--------------------------------------------------------";
        cout<<endl<<"1. insert element at last"<<endl;
            
        cout<<endl<<"2. delete first element "<<endl;
        
        cout<<endl<<"3. print list"<<endl;
        
        cout<<endl<<"4. strit  "<<endl<<endl;
        
        
        cout << "enter any choice and press enter  ::  ";
        cin >> choice;
        cout<<"\n";
        
       

        switch (choice)
        {
        case 1:
            int a;
            cout << " enter the ist item :: ";
            cin >> a;
            insertNode(a);
            break;
        case 2:
            deletAtStart();
            break;
        case 3:
            cout << endl;
            printList();
            break;
        case 4:
            choice = 4;
            continue;

        default:
            cout << "chose somthing sutable " << endl;
            break;
        }
        cout<<endl<<endl;
    }
}