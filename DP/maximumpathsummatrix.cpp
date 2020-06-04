#include <iostream>
using namespace std;
#define N 3 
#define M 3
int vis[N][M];
int dp[N][M];

int findMaxPath(int arr[N][M],int i,int j) {
    if (i==N-1 && j==M-1)
        return arr[N-1][M-1];
        
    if (vis[i][j]) return dp[i][j];
    vis[i][j] = 1;
    
    int &maxsum = dp[i][j];
    
    if (i<N-1 && j<M-1){
        maxsum += arr[i][j] + max(findMaxPath(arr,i+1,j), findMaxPath(arr, i, j+1));
    } else if (i == N-1) {
        maxsum += arr[i][j] + findMaxPath(arr,i,j+1);
    } else {
        maxsum += arr[i][j] +  findMaxPath(arr, i+1,j);
    }
    return maxsum;
}

int main() 
{ 
      
    int a[N][M];
    /*= { { 10, 10, 2, 0, 20, 4 }, 
                    { 1, 0, 0, 30, 2, 5 }, 
                    { 0, 10, 4, 0, 2, 0 }, 
                    { 1, 0, 2, 20, 0, 4 } }; */
              
    

    a[0][0] = 500; 
    a[0][1] = 100; 
    a[0][2] = 230; 
    a[1][0] = 1000; 
    a[1][1] = 300; 
    a[1][2] = 100; 
    a[2][0] = 200; 
    a[2][1] = 1000; 
    a[2][2] = 200;
    cout << findMaxPath(a,0,0) << endl; 
    return 0; 
} 
