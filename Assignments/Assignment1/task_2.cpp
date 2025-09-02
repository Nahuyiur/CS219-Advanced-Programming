#include <bits/stdc++.h>
using namespace std;

int combination(int n, int k) {
    if (k > n - k) {
        k = n - k;  
    }

    int result = 1;
    for (int i = 1; i <= k; ++i) {
        result *= (n - i + 1);  
        result /= i;            
    }

    return result;
}

int main() {
    int n, k;
    cin >> n >> k;

    int ans = combination(n, k);
    cout << ans << endl;

    return 0;
}
