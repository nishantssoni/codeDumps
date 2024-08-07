#include<iostream>
using namespace std;

//node
struct node{
    int info;
    struct node *pre, *nstrt;
};

//head and last pointer
node *start = NULL;
node *last   = NULL;

//creat node
 node *creatNode(){
    node *temp = new node;
    return temp;
}

//insert node
void insertNode(int in){

    //node creation
    node *temp,*prevTemp,*n;
    n = creatNode();

    //assign
    last = n;           //setting end node
    n->pre =  NULL;
    n->nstrt = NULL;
    n->info = in;

    if(start == NULL){
       
       start = n;
    
    }
    
    else{
        temp = start;
        prevTemp = temp;
        
        while(temp->nstrt != NULL){
            
            prevTemp = temp->nstrt;
            temp = temp->nstrt;

        }
        
        temp -> nstrt = n;
        n -> pre = prevTemp;
    }
}

//print list 
void printList(){
    if (start == NULL)
    {
        cout<<"your dont have a list"<<endl;
    }else
    {   node *temp;
        temp = start;

        while(temp != NULL){
            cout<<temp->info<<endl;
            temp = temp->nstrt;
           
        }
    }
}

// print list reverse
void printListReverse(){
    if (start == NULL)
    {
        cout<<"your dont have a list"<<endl;
    }
    else{
        node *temp;
        temp = last;
        
        while(temp != NULL){
            cout<<temp->info<<endl;
            temp = temp->pre;
            
        }
    }
}

void deleteFromFirst(){
    node *temp = start;
    if(start == NULL){
        cout<<" can\'t delete you don\'n have any list item"<<endl;
    }else{
        if(start->nstrt == NULL){
            start = NULL;
            delete temp;
        }else{
        
        start = start ->nstrt;
        start ->pre = NULL;
        delete temp;
        }
    }
}

int main(){

    insertNode(1);
    insertNode(2);
    insertNode(3);
    insertNode(4);

    printList();
    cout<<"\n"<<endl;
    printListReverse();

   deleteFromFirst();
   deleteFromFirst();
   printList();
     cout<<"\n"<<endl;
   printListReverse();

    
    
    return 0;
}