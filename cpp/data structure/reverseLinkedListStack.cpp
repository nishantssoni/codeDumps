//NISHANT SHEKHAR SONI
//reverse LINKED LIST using stack
#include <iostream>
#include<stack>
using namespace std;

struct node //node
{
    int info;
    struct node *link;
};

struct node *start = NULL; //starting pointer
static int counter = 0;

struct node *creatNode() //new node
{   ++counter;
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
    {   --counter;
        struct node *temp;
        temp = start;
        //temp->link = start;  //getting address of first node for deletion
        start = start->link; //now start pointing to first node *link pointer which means 2nd node
        free(temp);          //deleting first node
    }
}

void reverseListUsingStack(){
    if(start == NULL){
        cout<<"list is empty"<<endl;
        return;
    }
    stack<node*> st;
    node *temp = start;
    while(temp!= NULL){
        st.push(temp);
        temp = temp->link;
    }

    temp = st.top();
    start = temp;
    st.pop();

    while(!st.empty())
    {
        temp->link = st.top();

        st.pop();

        temp = temp->link;
    }
    temp->link = NULL;

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

    while (choice != 5)
    {   cout<<"--------------------------------------------------------";
        cout<<endl<<"1. insert element at last"<<endl;
            
        cout<<endl<<"2. delete first element "<<endl;
        
        cout<<endl<<"3. print list"<<endl;
        
        cout<<endl<<"4. reverse list"<<endl;
        
        cout<<endl<<"5. strit  "<<endl<<endl;
        
        
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
            reverseListUsingStack();
            break;
        
        case 5:
            choice = 5;
            break;

        default:
            cout << "chose somthing sutable " << endl;
            break;
        }
        cout<<endl<<endl;
    }
}