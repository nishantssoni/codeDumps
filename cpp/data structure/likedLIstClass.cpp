//NISHANT SHEKHAR SONI
//Linked list in class
//unfinished loop interface is left
#include<iostream>
using namespace std;

class LinkedList{

    int counter;
    
    //node
    struct node{
    int data;
    node *nstrt;
    };

    node *head;

    node* creatNode(){
        ++counter;
        return new node;
    }

    public:
    
    LinkedList(){       //constructer
         head =NULL;
         counter = 0;
     }
  
    void printLength(){  //length of a functioncounter is
        cout<<" length of list :: "<<counter<<endl;
    }

    node* getHead(){       //return head 
        return head;
    }

    void push(int val){
        
        node *newNode = creatNode();
        newNode->data = val;
        newNode->nstrt = NULL;

        if(head == NULL){
            head = newNode;
            return;
        }
        
        node *temp = head;
        while (temp->nstrt!= NULL)
        {
            temp = temp ->nstrt;
        }
        temp->nstrt = newNode;
        
    }

    int pop(){
        int popValue;
        node *deleteNode;
        node *temp = head;
        for (int i = 0; i < (counter-2); i++)
        {
            temp = temp ->nstrt;
        }

        deleteNode = temp->nstrt;
        temp->nstrt =NULL;
        delete deleteNode;
        --counter;   
    
    }

    void insertNodeInNthPosition(int val , int position){
        
        if(position>(counter+1)){
            cout<<"invalid move, node can only be added from 1 <= x <="<<counter+1<<endl;
            return;
        } 
       
        node *temp1 = creatNode();
        temp1->data = val;              
        temp1->nstrt = NULL;                
                                   
        if(position == 1){
            temp1->nstrt = head;
            head = temp1;
            return;
        }

        node *temp2 = head;
        for (int i = 0; i < position-2; i++)
        {
            temp2 = temp2 ->nstrt;
        }
        temp1->nstrt = temp2->nstrt;
        temp2->nstrt = temp1; 
    }

    void deleteInNthPosition(int position){

        if(position>(counter)){
            cout<<"invalid move, node can only be deleted from 1 <= x <="<<counter<<endl;
            return;
        }
    
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
        --counter;
    
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

    void reccureReverse2(node *p){
        if(p->nstrt == NULL){
            head = p;
            return;
        }
        
        reccureReverse2(p->nstrt);

        node *q = p->nstrt;
        q -> nstrt = p;
        p -> nstrt = NULL;
    }

    void reccurePrint(node *p){
        if(p == NULL)   //base case
            return;
        
        reccurePrint(p->nstrt);
        
        cout<<p->data<<endl;
    }

    void print(){
        node *temp = head;
        while(temp != NULL){
            cout<<temp->data<<endl;
            temp = temp ->nstrt;
        }
        cout<<endl;
    }

};


int main(){
    
    LinkedList a;
    a.push(8);
    a.insertNodeInNthPosition(2,1);
    a.insertNodeInNthPosition(3,3);
    a.insertNodeInNthPosition(4,2);
    a.insertNodeInNthPosition(5,3);
    a.insertNodeInNthPosition(6,2);
    a.pop();
    a.printLength();
    a.insertNodeInNthPosition(77,7);
    a.reccureReverse(NULL,a.getHead());
    a.deleteInNthPosition(7);
    a.reverse();
    a.print();
    a.printLength();
    cout<<""<<endl;
    a.reccurePrint(a.getHead());
    cout<<""<<endl;
    a.reccureReverse2(a.getHead());
    a.print();

    int choice = 0, num,position;

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
            
            cout << " enter the list item :: ";
            cin >> num;
            a.push(num);
            break;
        
        case 2:
            cout<<"enter a element in nth position"<<endl;
            cout << " enter the list item :: ";cin>>num;
            cout << " enter the list position :: ";cin>>position;
            a.insertNodeInNthPosition(num , position);
            break;
        case 3:
            cout << endl;
            
            break;
        case 4:
            
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
    
    return 0;
}