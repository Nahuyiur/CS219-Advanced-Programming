#include<bits/stdc++.h>
using namespace std;
bool isNumber(char* str){
    for(int i=0;str[i]!='\0';i++){
        if(!isdigit(str[i]))return false;
    }
    return true;
}
int fill_array(double arr[],int size){
    double value;
    int real_length=0;
    for (int i = 0; i < size; i++)
    {
        printf("Enter value #%d :",i+1);
        if(cin>>value){
            arr[i]=value;
            real_length++;
        }else{
            break;
        }
    }
    return real_length;
    
}
void show_array(double *arr,int size){
    for(int i=0;i<size;i++){
        cout<<*arr<<" ";
        arr++;
    }
    cout<<endl;
}
void reverse_array(double *arr, int size) {
    if (size <= 1) return; 

    double temp = arr[0];
    arr[0] = arr[size - 1];
    arr[size - 1] = temp;

    reverse_array(arr + 1, size - 2);
}
int main(){
    int size;

    printf("Enter the size of an array:");
    cin>>size;
    double* a=new double[size];
    if(size <= 0){
        cerr << "Error: Size must be a positive integer." << endl;
        return 1;
    }

    int real_size=fill_array(a,size);

    cout<<"The original array is: ";
    show_array(a,real_size);

    cout<<"The reversed array is: ";
    reverse_array(a,real_size);
    show_array(a,real_size);
    delete [] a;
}