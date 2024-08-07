//NISHANT SHEKHAR SONI
//Linked list and adding a value in nth position
#include<iostream>
using namespace std;

//node
struct node{
int data;
node *nstrt;
};

//head
node *head;

//creat node
node* creatNode(){
    return new node;
}

void insertNodeInNthPosition(int val , int position){  
    node *temp1 = creatNode();
    temp1->data = val;              
    temp1->nstrt = NULL;                
                                   
    if(position == 1){
        temp1->nstrt = head;
        head = temp1;
        return;
    }

// for nth node we have to go n-2 node 
// because n-2 contain  the address of nth node
    node *temp2 = head;
    for (int i = 0; i < position-2; i++)
    {
        temp2 = temp2 ->nstrt;
    }
        temp1->nstrt = temp2->nstrt;
        temp2->nstrt = temp1; 
    
}



void deleteInNthPosition(int position){
    
    node *temp1 = head;

    if(position == 1){
        head = head ->nstrt;
        delete temp1;
        return;
    }

    node *temp2;

    for (int i = 0; i < position-2; i++)
    {
        temp1 = temp1->nstrt;
    }
    temp2 = temp1 ->nstrt;
    temp1 ->nstrt = temp2 ->nstrt;
    delete temp2;
    
}

void reverse(){
    node *curr,*prv,*nxt;

    prv = NULL;
    curr = head;
    while (curr!=NULL)
    {
        nxt = curr ->nstrt;
        curr->nstrt = prv;
        prv = curr;
        curr = nxt;
    }
    head = prv;
    
}

void reccureReverse(node *prv, node *curr){ 
    
       if(curr == NULL){ //base case
        head = prv;
        return;
    }
    node *nxt = curr->nstrt;
    curr ->nstrt = prv;

    reccureReverse(curr,nxt);
}

void print(){
    node *temp = head;
    while(temp != NULL){
        cout<<temp->data<<endl;
        temp = temp ->nstrt;
        
    }
    cout<<endl<<endl;
}

int main()
{
    head =NULL;

    insertNodeInNthPosition(2,1);
    insertNodeInNthPosition(3,2);

    insertNodeInNthPosition(4,1);
    insertNodeInNthPosition(5,2);
    print();

    deleteInNthPosition(1);
    reverse();
    print();

    reccureReverse(NULL,head);
    print();
}