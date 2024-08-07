#include <iostream>
using namespace std;

class queue
{
    private:
        int front, rear, *qu, maxsize;

    public:
        queue(int size)
        {
            front = -1;
            rear = -1;
            maxsize = size;
            qu = new int[size];
        }

        bool isEmpty()
        {
            // if (front == -1 && rear == -1)
            //     return true;
            // return false;
            return (front == -1 && rear == -1);
        }

        bool isFull()
        {
            // if ((rear + 1) % maxsize == front)
            //     return true;
            // return false;
            return (rear + 1) % maxsize == front ? true : false;
        }

        int show()
        {
            if (!isEmpty())
                return qu[front];
            else
            {
                cout << "queue is empty " << endl;
                return -1;
            }
        }

        void enQueue(int val)
        {
            if (isFull())
            {
                cout << "Error: queue is full " << endl;
                return;
            }
            else if (isEmpty())
            {
                // ++front;     //both do same thing
                // ++rear;
                front = rear = 0;
            }
            else
            {
                rear = (rear + 1) % maxsize;
            }

            qu[rear] = val;
            // cout << " enquied  " << front << "  " << rear << endl;
        }

        int deQueue()
        {

            if (isEmpty())
            {
                cout << "Error: Queue is empty" << endl;
                return -1;
            }

            int temp;

            //old code
            // if (front == rear)
            // {

            //     temp = qu[front];
            //     front = rear = -1;

            //     return temp;
            // }

            // temp = show();
            // front = (front + 1) % maxsize;
            // return temp;

            //new code
            temp = show();

            if (front == rear)
                front = rear = -1;

            else
                front = (front + 1) % maxsize;
            return temp;
        }

        // void print()
        // {        //my implitation
        //     int x = front;
        //     while (x >= 0)
        //     {
        //         if (x <= rear)
        //         {
        //             for (; x <= rear; ++x)
        //             {
        //                 cout << qu[x] << "  ";
        //             }
        //             cout << endl;
        //             x = -1;
        //         }
        //         else
        //         {
        //             cout << qu[x] << "  ";
        //             x = (x + 1) % maxsize;
        //             if (x == rear)
        //             {
        //                 x = -1;
        //             }
        //         }
        //     }
        //     cout << endl;
        // }

        //     void print(){
        //         //mycode school implition
        // 		int count = (rear+maxsize-front)%maxsize + 1;
        // 		cout<<"Queue       : ";
        // 		for(int i = 0; i <count; i++)
        // 		{
        // 			int index = (front+i) % maxsize; // Index of element while travesing circularly from front
        // 			cout<<qu[index]<<" ";
        // 		}
        // 		cout<<"\n\n";
        //     }
        // 

        void print()
        {   //other implitation
            if (isEmpty())
            {
                cout << "Error : queue is empty" << endl;
            }
            else
            {
                    for (int i = front; i != rear; i = (i + 1) % maxsize)
                    {
                        cout << qu[i] << " ";
                    }
                    cout << qu[rear];
                    cout << endl;
            }

        }
    };

    int main()
    {
        queue x(5);

        x.enQueue(1);
        x.enQueue(2);
        x.enQueue(3);
        x.enQueue(4);
        x.print();

        x.deQueue();
        x.deQueue();
        x.print();

        x.enQueue(5);
        x.enQueue(6);
        x.enQueue(7);
        x.enQueue(8);
        x.print();
        x.deQueue();
        x.deQueue();
        x.enQueue(9);
        x.enQueue(10);
        x.deQueue();
        x.print();
        x.deQueue();
        x.print();

        cout << "successfully exicuted!!";
    }