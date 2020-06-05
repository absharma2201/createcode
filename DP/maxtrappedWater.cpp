#include <iostream>
using namespace std;

int vis[n];
int dp[n];

int maxWater(int arr[], int i, int n) {
    int maxW = 0;
    if (i == n-1) return 0;
  //  for (int i =0; i<n; i++) {
        for (int j= i+1; j<n; j++) {
            if (min(arr[i], arr[j])*(j-i-1) >= maxW) {
                maxW = min(arr[i], arr[j])*(j-i-1);
            }
        }
    //}
    maxW = max(maxW, maxWater(arr, i+1, n));
    return maxW;
}

int main() 
{ 
    //int height[] = { 2, 1, 3, 4, 6, 5 }; 
    int height[] = { 1, 3, 4 };
    int n = sizeof(height) / sizeof(height[0]); 
    cout << maxWater(height,0, n); 
    return 0; 
} 
