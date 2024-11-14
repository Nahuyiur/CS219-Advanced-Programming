#include <iostream>
#include <sstream>
using namespace std;
void mr(int **a, int n) {
    for (int i = 0; i < n; i++) {
        int index = n - 1;
        for (int j = n - 1; j >= 0; j--) {
            if (a[i][j] != 0) {
                a[i][index] = a[i][j];
                if (index != j) {
                    a[i][j] = 0;
                }
                index--;
            }
        }
        for (int j = n - 1; j > 0; j--) {
            if (a[i][j] != 0 && a[i][j] == a[i][j - 1]&&a[i][j]!=2048) {
                a[i][j] *= 2; 
                a[i][j - 1] = 0; 
                j--; 
            }
        }

        int index2 = n - 1;
        for (int j = n - 1; j >= 0; j--) {
            if (a[i][j] != 0) {
                a[i][index2] = a[i][j];
                if (index2 != j) {
                    a[i][j] = 0;
                }
                index2--;
            }
        }
    }
}
void ml(int **a, int n) {
    for (int i = 0; i < n; i++) {
        int index = 0;
        for (int j = 0; j < n; j++) {
            if (a[i][j] != 0) {
                a[i][index] = a[i][j];
                if (index != j) {
                    a[i][j] = 0;
                }
                index++;
            }
        }

        for (int j = 0; j < n-1; j++) {
            if (a[i][j] != 0 && a[i][j] == a[i][j + 1]&&a[i][j]!=2048) {
                a[i][j] *= 2;
                a[i][j + 1] = 0; 
                j++; 
            }
        }

        int index2 = 0;
        for (int j = 0; j < n; j++) {
            if (a[i][j] != 0) {
                a[i][index2] = a[i][j];
                if (index2 != j) {
                    a[i][j] = 0;
                }
                index2++;
            }
        }
    }
}
void mu(int **a, int n) {
    for (int i = 0; i < n; i++) {
        int index = 0;
        for (int j = 0; j < n; j++) {
            if (a[j][i] != 0) {
                a[index][i] = a[j][i];
                if (index != j) {
                    a[j][i] = 0;
                }
                index++;
            }
        }

        for (int j = 0; j < n-1; j++) {
            if (a[j][i] != 0 && a[j][i] == a[j+1][i]&&a[j][i]!=2048) {
                a[j][i] *= 2;
                a[j+1][i] = 0; 
                j++; 
            }
        }
        int index2 = 0;
        for (int j = 0; j < n; j++) {
            if (a[j][i] != 0) {
                a[index2][i] = a[j][i];
                if (index2 != j) {
                    a[j][i] = 0;
                }
                index2++;
            }
        }
    }
}
void md(int **a, int n) {
    for (int i = 0; i < n; i++) {
        int index = n-1;
        for (int j = n-1; j >= 0; j--) {
            if (a[j][i] != 0) {
                a[index][i] = a[j][i];
                if (index != j) {
                    a[j][i] = 0;
                }
                index--;
            }
        }

        for (int j = n-1; j > 0; j--) {
            if (a[j][i] != 0 && a[j][i] == a[j-1][i]&&a[j][i]!=2048) {
                a[j][i] *= 2;
                a[j-1][i] = 0; 
                j--; 
            }
        }
        int index2 = n-1;
        for (int j = n-1; j >= 0; j--) {
            if (a[j][i] != 0) {
                a[index2][i] = a[j][i];
                if (index2 != j) {
                    a[j][i] = 0;
                }
                index2--;
            }
        }
    }
}
void changedir(string str, int **a, int n)
{
    if(str=="right")mr(a, n);
    if(str=="left")ml(a,n);
    if(str=="up")mu(a,n);
    if(str=="down")md(a,n);
}
int main()
{
    int n;
    cin >> n;

    int **a = new int *[n];
    for (int i = 0; i < n; i++)
    {
        a[i] = new int[n];
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> a[i][j];
        }
    }
    cin.ignore(); 

    string line;
    getline(cin,line);
    stringstream ss(line);

    string dir;
    while (ss>>dir)
    {
        if (dir.empty())
            break;
        changedir(dir, a, n);
       // cout<<dir<<endl;
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << a[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}