#include <iostream>
using namespace std;

class Queue
{
private:
    struct Node
    {
        int val;
        Node *next;
    };
    Node *front;
    Node *rear;

public:
    Queue()
    {
        front = NULL;
        rear = NULL;
    }

    Node *creatNode()
    {
        return new Node;
    }

    bool isEmpty()
    {
        return front == NULL;
    }

    void Enqueue(int val)
    {
        //new node created with name of newNode
        Node *newNode = creatNode();

        //value adding
        newNode->val = val;
        newNode->next = NULL;

        //checking if queue is empty
        if (isEmpty())
        {
            front = newNode;
        }
        else
        {
            rear->next = newNode;
        }
        rear = newNode;
    }

    //dequeue function
    int Dequeue()
    {

        if (isEmpty())
        {
            cout << "Error: queue is empity" << endl;
            return -1;
        }
        else
        {

            Node *temp = front;

            int returnVal = temp->val;
            front = temp->next;

            delete temp;
            return returnVal;
        }
    }

    //printing value of queue
    void printQueue()
    {
        if (isEmpty())
        {
            cout << "Error: queue is empity!!" << endl;
            return;
        }

        Node *temp = front;

        while (temp != NULL)
        {

            cout << temp->val << "  ";
            temp = temp->next;
        }
        cout << endl;
    }
};

int main()
{
    Queue q;
    q.Enqueue(1);
    q.Enqueue(2);
    q.Enqueue(3);
    q.Enqueue(4);
    q.Enqueue(5);

    q.Dequeue();
    q.Dequeue();
    q.Dequeue();

    q.printQueue();

    return 0;
}