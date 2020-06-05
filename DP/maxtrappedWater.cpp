#include <iostream>
using namespace std;

int vis[100];
int dp[100];

int maxWater(int arr[], int i, int n) {
    
    if (i == n-1) return 0;
    if(vis[i]){
        cout<<"from here"<<endl;
        return dp[i];   
    }
    vis[i] = 1;
    int &maxW = dp[i];
    maxW = 0;
  //  for (int i =0; i<n; i++) {
        for (int j= i+1; j<n; j++) {
            if (min(arr[i], arr[j])*(j-i-1) >= maxW) {
                maxW = min(arr[i], arr[j])*(j-i-1);
            }
        }
    //}
    if (i < n-1)
        maxW = max(maxW, maxWater(arr, i+1, n));
    return maxW;
}

int maxWater2(int arr[], int n){
    int i =0,j=n-1;
    int maxW = 0;
    while(i<j){
        if(arr[i] < arr[j]){
            maxW = max(maxW, (j-i-1)*arr[i]);
            i++;
            continue;
        }
        maxW = max(maxW, (j-i-1)*arr[j]);
        j--;
    }
    return maxW;
}
int main() 
{ 
    int height[] = { 2, 1, 3, 4, 6, 5 }; 
    //int height[] = { 1, 3, 4 };
    int n = sizeof(height) / sizeof(height[0]); 
    cout << maxWater(height,0, n)<<endl;
    cout<< maxWater2(height,n)<<endl;
    return 0; 
} 
