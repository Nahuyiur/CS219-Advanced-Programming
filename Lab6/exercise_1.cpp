#include <bits/stdc++.h>
using namespace std;
void display(){
    cout<<"Calculator Menu:"<<endl;
    cout<<"1.Add"<<endl;
    cout<<"2.Subtract"<<endl;
    cout<<"3.Multiply"<<endl;
    cout<<"4.Divide"<<endl;
    cout<<"5.Modulus"<<endl;
}
int Add(int a,int b){
    return a+b;
}
int Subtract(int a,int b){
    return a-b;
}
int Multiply(int a,int b){
    return a*b;
}
int Divide(int a,int b){
    return a/b;
}
int Modulus(int a,int b){
    return a%b;
}
int main()
{
    int choice;
    int num1, num2;
    char continueChoice;
    display();
    do
    {
        cout << "Choose operation (1-5): ";
        cin >> choice;
        cout << "Enter first number: ";
        cin >> num1;
        cout << "Enter second number: ";
        cin >> num2;
        int ans;
        switch (choice)
        {
            case 1:
                ans=Add(num1,num2);
                printf("Result: %d\n",ans);
                break;
            case 2:
                ans=Subtract(num1,num2);
                printf("Result: %d\n",ans);
                break;
            case 3:
                ans=Multiply(num1,num2);
                printf("Result: %d\n",ans);
                break;
            case 4:
                ans=Divide(num1,num2);
                printf("Result: %d\n",ans);
                break;
            case 5:
                ans=Modulus(num1,num2);
                printf("Result: %d\n",ans);
                break; 
            default:
                break;       
        }
        cout << "Press Y/y to continue, N/n to exit: ";
        cin >> continueChoice;
    } while (continueChoice == 'Y' || continueChoice == 'y');
    cout << "Done!" << endl;
    return 0;
}