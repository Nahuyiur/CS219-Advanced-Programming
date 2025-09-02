#include <iostream>
using namespace std;
typedef unsigned long Item;

class Stack {
private:
    enum { MAX = 10 }; 
    Item* pitems;      
    int size;       
    int top;   

public:
    Stack(int n = MAX) : size(n), top(0) {
        pitems = new Item[size];
    }

    Stack(const Stack& st) : size(st.size), top(st.top) {
        pitems = new Item[size];
        for (int i = 0; i < top; ++i) {
            pitems[i] = st.pitems[i];
        }
    }

    ~Stack() {
        delete[] pitems;
    }

    bool isempty() const {
        return top == 0;
    }

    bool isfull() const {
        return top == size;
    }

    bool push(const Item& item) {
        if (isfull()) {
            return false;
        }
        pitems[top++] = item;
        return true;
    }


    bool pop(Item& item) {
        if (isempty()) {
            return false;
        }
        item = pitems[--top];
        return true;
    }

    Stack& operator=(const Stack& st) {
        if (this == &st) {
            return *this;
        }
        delete[] pitems;
        size = st.size;
        top = st.top;
        pitems = new Item[size];
        for (int i = 0; i < top; ++i) {
            pitems[i] = st.pitems[i];
        }
        return *this;
    }
};

int main() {
    Stack s1; 
    s1.push(10);
    s1.push(20);
    s1.push(30);

    cout << "Popping from s1: ";
    Item poppedItem;
    while (s1.pop(poppedItem)) {
        cout << poppedItem << " ";
    }
    cout << endl;

    Stack s2 = s1;
    s2.push(40);
    s2.push(50);

    cout << "Popping from s2: ";
    while (s2.pop(poppedItem)) {
        cout << poppedItem << " ";
    }
    cout << endl;

    Stack s3(20);
    s3.push(100);
    s3.push(200);
    s3 = s2;

    cout << "Popping from s3: ";
    while (s3.pop(poppedItem)) {
        cout << poppedItem << " ";
    }
    cout << endl;

    return 0;
}