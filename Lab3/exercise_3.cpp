#include <iostream>
#include <cstring> 
#include <limits>

using namespace std;

struct CandyBar {
    char name[20];
    float weight;
    int calories;
};

int main() {
    CandyBar candybars[3];
    string name;
    float weight;
    int calories;

    int id=0;double max=0;
    for(int i=0;i<=2;i++){
        cout << "Enter brand name of a Candybar: ";
        getline(cin, name);

        // 将string复制到char数组中
        strncpy(candybars[i].name, name.c_str(), sizeof(candybars[i].name) - 1);
        candybars[i].name[sizeof(candybars[i].name) - 1] = '\0'; // 确保结尾字符为'\0'

        cout << "Enter weight of the Candybar: ";
        cin >> weight;
        candybars[i].weight = weight;

        cout << "Enter calories (an integer value) in the Candybar: ";
        cin >> calories;
        candybars[i].calories = calories;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if(candybars[i].calories/candybars[i].weight>max){
            max=candybars[i].calories/candybars[i].weight;
            id=i;
        }
    }
    
    cout<<endl;
    cout<<"Display the CandyBar array contents."<<endl;

    for(int i=0;i<=2;i++){
        printf("Brand name: %s\n",candybars[i].name);
        printf("Weight: %g\n",candybars[i].weight);
        printf("Calories: %d\n",candybars[i].calories);
    }
    printf("The greatest calories per weight is:\n");
    printf("Brand name: %s\n",candybars[id].name);
    printf("Calories per weight: %g\n",max);
    return 0;
}
